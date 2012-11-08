/*
   Copyright (C) 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "metadataextractorservice.h"

#include <Nepomuk2/ResourceWatcher>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/File>
#include <Nepomuk2/Vocabulary/NMM>
#include "ontology/nbib.h"

#include <KDE/KStandardDirs>
#include <KDE/KDirNotify>
#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <KDE/KDebug>

#include <QtCore/QProcess>
#include <QtCore/QFile>

using namespace Nepomuk2::Vocabulary;

class MetaDataExtractorServicePrivate
{
public:
    Nepomuk2::ResourceWatcher* videoWatcher;
    Nepomuk2::ResourceWatcher* documentWatcher;
    Nepomuk2::ResourceWatcher* musicWatcher;

    int runningProcesses;
    QStringList processQueue;
};

MetaDataExtractorService::MetaDataExtractorService(QObject *parent, const QVariantList &)
    : Nepomuk2::Service(parent)
    , d_ptr(new MetaDataExtractorServicePrivate)
{
    Q_D(MetaDataExtractorService);

    d->runningProcesses = 0;
    d->videoWatcher = 0;
    d->documentWatcher = 0;
    d->musicWatcher = 0;

    KConfig config("nepomukmetadataextractorrc");
    KConfigGroup serviceGroup(&config, "Service");

    bool firstRun = serviceGroup.readEntry("FirstRun", true);

    // This disabled the service on first run
    // Normally when the service is installed it will be enabled by default.
    // This would expose private data to the internet and might not be what a user expects
    // when he installs the metadata extractor, to avoid this the extractor must be enabled
    // explicitly in the KCM config settings
    if (firstRun) {
        KConfig serverConfig(QLatin1String("nepomukserverrc"));
        KConfigGroup serverGroup(&serverConfig, QLatin1String("Service-metadataextractorservice"));
        serverGroup.writeEntry(QLatin1String("autostart"), false);
        serverGroup.sync();

        serviceGroup.writeEntry("FirstRun", false);
        serviceGroup.sync();

        QDBusInterface service("org.kde.nepomuk.services.metadataextractorservice", "/servicecontrol",
                               "org.kde.nepomuk.ServiceControl");
        service.call("shutdown");

        return;
    }

    bool videoServiceEnabled = serviceGroup.readEntry("VideoServiceEnabled", true);
    bool musicServiceEnabled = serviceGroup.readEntry("MusicServiceEnabled", true);
    bool documentServiceEnabled = serviceGroup.readEntry("DocumentServiceEnabled", false);

    // set up the watcher for newly created nfo:Video resources
    if (videoServiceEnabled) {
        d->videoWatcher = new Nepomuk2::ResourceWatcher(this);
        d->videoWatcher->addType(NFO::Video());
        connect(d->videoWatcher, SIGNAL(resourceCreated(Nepomuk2::Resource, QList<QUrl>)),
                this, SLOT(slotVideoResourceCreated(Nepomuk2::Resource, QList<QUrl>)));
        d->videoWatcher->start();
    }

    // set up the watcher for newly created documents
    if (documentServiceEnabled) {
        d->documentWatcher = new Nepomuk2::ResourceWatcher(this);
        d->documentWatcher->addType(NFO::PaginatedTextDocument());
        connect(d->documentWatcher, SIGNAL(resourceCreated(Nepomuk2::Resource, QList<QUrl>)),
                this, SLOT(slotDocumentResourceCreated(Nepomuk2::Resource, QList<QUrl>)));
        d->documentWatcher->start();
    }

    // set up the watcher for newly created music files
    if (musicServiceEnabled) {
        d->musicWatcher = new Nepomuk2::ResourceWatcher(this);
        d->musicWatcher->addType(NFO::Audio());
        connect(d->musicWatcher, SIGNAL(resourceCreated(Nepomuk2::Resource, QList<QUrl>)),
                this, SLOT(slotMusicResourceCreated(Nepomuk2::Resource, QList<QUrl>)));
        d->musicWatcher->start();
    }

    d->processQueue = serviceGroup.readEntry("notfinishedqueue", QStringList());

    startNextProcess();
}

MetaDataExtractorService::~MetaDataExtractorService()
{
    Q_D(MetaDataExtractorService);

    if (d->videoWatcher) {
        d->videoWatcher->stop();
        delete d->videoWatcher;
    }
    if (d->documentWatcher) {
        d->documentWatcher->stop();
        delete d->documentWatcher;
    }
    if (d->musicWatcher) {
        d->musicWatcher->stop();
        delete d->musicWatcher;
    }

    KConfig config("nepomukmetadataextractorrc");
    KConfigGroup serviceGroup(&config, "Service");

    serviceGroup.writeEntry("notfinishedqueue", d->processQueue);
    serviceGroup.sync();
    config.sync();
}

void MetaDataExtractorService::slotVideoResourceCreated(const Nepomuk2::Resource &res, const QList<QUrl> &types)
{
    Q_UNUSED(types);

    // here we check if the resource we added is actually a file
    // and that it does not already have tvshow / movie information
    // if it already has such information, it is very likely, that the resource was created via
    // the metadata extractor or other program with full information
    // no need to fetch more than necessary to reduce the work load

    if (res.isFile()) {

        if (res.hasType(Nepomuk2::Vocabulary::NMM::Movie()) || res.hasType(Nepomuk2::Vocabulary::NMM::TVShow())) {
            return;
        }

        const QString path = res.toFile().url().toLocalFile();
        if (QFile::exists(path)) {
            Q_D(MetaDataExtractorService);
            d->processQueue.append(path);
            startNextProcess();
        }
    }
}

void MetaDataExtractorService::slotDocumentResourceCreated(const Nepomuk2::Resource &res, const QList<QUrl> &types)
{
    Q_UNUSED(types);

    // here we check if the resource we added is actually a file
    // and that it does not already have publication information
    // if it already has such information, it is very likely, that the resource was created via
    // the metadata extractor or other program with full information
    // no need to fetch more than necessary to reduce the work load

    if (res.isFile()) {

        if (res.hasProperty(Nepomuk2::Vocabulary::NBIB::publishedAs())) {
            return;
        }

        const QString path = res.toFile().url().toLocalFile();
        if (QFile::exists(path)) {
            Q_D(MetaDataExtractorService);
            d->processQueue.append(path);
            startNextProcess();
        }
    }
}

void MetaDataExtractorService::slotMusicResourceCreated(const Nepomuk2::Resource& res, const QList<QUrl>& types)
{
    Q_UNUSED(types);

    if (res.isFile()) {

        const QString path = res.toFile().url().toLocalFile();
        if (QFile::exists(path)) {
            Q_D(MetaDataExtractorService);
            d->processQueue.append(path);
            startNextProcess();
        }
    }
}

void MetaDataExtractorService::processFinished(int returnCode, QProcess::ExitStatus status)
{
    Q_UNUSED(returnCode);
    Q_UNUSED(status);

    sender()->deleteLater(); // delete the calling QProcess again

    Q_D(MetaDataExtractorService);

    d->runningProcesses--;
    startNextProcess();
}

void MetaDataExtractorService::startNextProcess()
{
    Q_D(MetaDataExtractorService);

    KConfig config("nepomukmetadataextractorrc");
    KConfigGroup serviceGroup(&config, "Service");

    int maxProcesses = serviceGroup.readEntry("SimultaneousCalls", 3);

    kDebug() << "Current running Processes:" << d->runningProcesses << " || Process Queue: " << d->processQueue.size();

    // only start the process if we haven't reached the maximum queue number and there are still entries in the queue left
    if (d->runningProcesses < maxProcesses && !d->processQueue.isEmpty()) {
        QString path = d->processQueue.takeFirst();

        kDebug() << "Calling" << KStandardDirs::findExe(QLatin1String("metadataextractor")) << path;
        d->runningProcesses++;

        QProcess *p = new QProcess();
        connect(p, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));
        connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));

        connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
        p->start(KStandardDirs::findExe(QLatin1String("metadataextractor")),
                 QStringList() << QLatin1String("-auto") << QLatin1String("-force") << path);

        startNextProcess();
    }
}

void MetaDataExtractorService::processOutput()
{
    QProcess *p = qobject_cast<QProcess *>(sender());
    kDebug() << p->readAllStandardError();
}

#include <kpluginfactory.h>
#include <kpluginloader.h>

NEPOMUK_EXPORT_SERVICE(MetaDataExtractorService, "metadataextractorservice")

#include "metadataextractorservice.moc"
