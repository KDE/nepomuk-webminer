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
#include "sro/nbib.h"

#include <QtCore/QProcess>
#include <QtCore/QFile>

#include <KDE/KStandardDirs>
#include <KDE/KDebug>
#include <KDE/KDirNotify>

using namespace Nepomuk2::Vocabulary;

MetaDataExtractorService::MetaDataExtractorService(QObject *parent, const QVariantList &)
    : Nepomuk2::Service(parent)
{
    // set up the watcher for newly created nfo:Video resources
    m_videoWatcher = new Nepomuk2::ResourceWatcher(this);
    m_videoWatcher->addType(NFO::Video());
    connect(m_videoWatcher, SIGNAL(resourceCreated(Nepomuk2::Resource,QList<QUrl>)),
            this, SLOT(slotVideoResourceCreated(Nepomuk2::Resource,QList<QUrl>)));
    m_videoWatcher->start();

    // set up the watcher for newly created documents
    m_documentWatcher = new Nepomuk2::ResourceWatcher(this);
    m_documentWatcher->addType(NFO::PaginatedTextDocument());
    connect(m_documentWatcher, SIGNAL(resourceCreated(Nepomuk2::Resource,QList<QUrl>)),
            this, SLOT(slotDocumentResourceCreated(Nepomuk2::Resource)));
    connect(m_documentWatcher, SIGNAL(resourceTypeAdded(Nepomuk2::Resource,Nepomuk2::Types::Class)),
            this, SLOT(slotDocumentResourceCreated(Nepomuk2::Resource)));
    m_documentWatcher->start();
}

MetaDataExtractorService::~MetaDataExtractorService()
{
    m_videoWatcher->stop();
    m_documentWatcher->stop();
    delete m_videoWatcher;
    delete m_documentWatcher;
}

void MetaDataExtractorService::slotVideoResourceCreated(const Nepomuk2::Resource &res, const QList<QUrl> &types)
{
    Q_UNUSED(types);

    // here we check if the resource we added is actually a file
    // and that it does not already have tvshow / movie information
    // if it already has such information, it is very likely, that the resource was created via
    // the metadata extractor or other program with full information
    // no need to fetch more than necessary to reduce the work load

    if(res.isFile()) {

        if(res.hasType(Nepomuk2::Vocabulary::NMM::Movie()) || res.hasType(Nepomuk2::Vocabulary::NMM::TVShow())) {
            return;
        }

        const QString path = res.toFile().url().toLocalFile();
        if(QFile::exists(path)) {
            kDebug() << "Calling" << KStandardDirs::findExe(QLatin1String("metadataextractor")) << path;

            QProcess::startDetached(KStandardDirs::findExe(QLatin1String("metadataextractor")),
                                    QStringList() << QLatin1String("-auto") << QLatin1String("-force") << path);
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

    if(res.isFile()) {

        if(res.hasProperty(Nepomuk2::Vocabulary::NBIB::publishedAs())) {
            return;
        }

        const QString path = res.toFile().url().toLocalFile();
        if(QFile::exists(path)) {
            kDebug() << "Calling" << KStandardDirs::findExe(QLatin1String("metadataextractor")) << path;

            QProcess::startDetached(KStandardDirs::findExe(QLatin1String("metadataextractor")),
                                    QStringList() << QLatin1String("-auto") << QLatin1String("-force") << path);
        }
    }
}

#include <kpluginfactory.h>
#include <kpluginloader.h>

NEPOMUK_EXPORT_SERVICE( MetaDataExtractorService, "metadataextractorservice" )

#include "metadataextractorservice.moc"
