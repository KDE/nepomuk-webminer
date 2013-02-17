/*
 * Copyright (C) 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nepomukwebminerservice.h"

#include "indexscheduler.h"
#include "mdesettings.h"
#include "webmineradaptor.h"

#include <QtDBus/QDBusInterface>

#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <KDE/KDebug>

class NepomukWebMinerServicePrivate
{
public:
    IndexScheduler *indexScheduler;
    WebMinerAdaptor *dBusAdaptor;

    // watch for the fileindexer going up/down send fileIndexingDone
    QDBusServiceWatcher* fileIndexerWatcher;
    QDBusInterface *dBusFileIndexer;
};

NepomukWebMinerService::NepomukWebMinerService(QObject *parent, const QVariantList &)
    : Nepomuk2::Service(parent)
    , d_ptr(new NepomukWebMinerServicePrivate)
{
    Q_D(NepomukWebMinerService);

    // This disabled the service on first run
    // Normally when the service is installed it will be enabled by default.
    // This would expose private data to the internet and might not be what a user expects
    // when he installs the metadata extractor, to avoid this the extractor must be enabled
    // explicitly in the KCM config settings
    if ( MDESettings::firstRun() ) {
        KConfig serverConfig(QLatin1String("nepomukserverrc"));
        KConfigGroup serverGroup(&serverConfig, QLatin1String("Service-nepomuk-webminerservice"));
        serverGroup.writeEntry(QLatin1String("autostart"), false);
        serverGroup.sync();

        MDESettings::setFirstRun(false);
        MDESettings::self()->writeConfig();

        QDBusInterface service("org.kde.nepomuk.services.nepomuk-webminerservice", "/servicecontrol",
                               "org.kde.nepomuk.ServiceControl");
        service.call("shutdown");

        return;
    }

    d->indexScheduler = new IndexScheduler( this );

    connect( d->indexScheduler, SIGNAL(statusStringChanged()), this, SLOT(generateStatus()) );

    connect( d->indexScheduler, SIGNAL(indexingStarted()), this, SIGNAL(indexingStarted()) );
    connect( d->indexScheduler, SIGNAL(indexingStopped()), this, SIGNAL(indexingStopped()) );

    // export on dbus
    d->dBusAdaptor = new WebMinerAdaptor( this );
    QDBusConnection::sessionBus().registerObject( QLatin1String( "/WebMiner" ), this );

    d->dBusFileIndexer = 0;
    // watch for the file indexer service to come up and go down
    d->fileIndexerWatcher = new QDBusServiceWatcher( "org.kde.nepomuk.services.nepomukfileindexer", QDBusConnection::sessionBus(),
                                                     QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration,
                                                     this );
    connect( d->fileIndexerWatcher, SIGNAL(serviceRegistered(QString)), this, SLOT(fileIndexerEnabled()) );
    connect( d->fileIndexerWatcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(fileIndexerDisabled()) );

     if ( QDBusConnection::sessionBus().interface()->isServiceRegistered( "org.kde.nepomuk.services.nepomukfileindexer" ) ) {
        fileIndexerEnabled();
     }
     else {
         fileIndexerDisabled();
     }
}

NepomukWebMinerService::~NepomukWebMinerService()
{
    Q_D(NepomukWebMinerService);

    QDBusConnection::sessionBus().unregisterObject( QLatin1String( "/WebMiner" ) );
}

int NepomukWebMinerService::status() const
{
    int state = 0; // default idle
    if(isIndexing()) {
        state = 1;
    }
    else if(isSuspended()) {
        state = 2;
    }

    return state;
}

bool NepomukWebMinerService::isIndexing() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->isIndexing();
}

bool NepomukWebMinerService::isSuspended() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->isSuspended();
}

void NepomukWebMinerService::suspend() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->suspend();
}

void NepomukWebMinerService::resume() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->resume();
}

void NepomukWebMinerService::indexManually(const QString &fileOrFolder )
{
    Q_D(NepomukWebMinerService);

    kDebug() << "index manually";
    return d->indexScheduler->indexManually(QUrl(fileOrFolder));
}

QString NepomukWebMinerService::currentFile() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->currentUrl().toString();
}

QString NepomukWebMinerService::statusMessage() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->userStatusString();
}

void NepomukWebMinerService::generateStatus()
{
    int state = 0;
    if(isSuspended()) {
        state = 2;
    }
    else if(isIndexing()) {
        state = 1;
    }

    QString msg = statusMessage();

    kDebug() << "send message" << state << msg;

    emit status(state, msg);
}

void NepomukWebMinerService::fileIndexerEnabled()
{
    Q_D(NepomukWebMinerService);

    d->dBusFileIndexer = new QDBusInterface(QLatin1String("org.kde.nepomuk.services.nepomukfileindexer"),
                                           QLatin1String("/nepomukfileindexer"),
                                           QLatin1String("org.kde.nepomuk.FileIndexer"),
                                           QDBusConnection::sessionBus());

    connect( d->dBusFileIndexer, SIGNAL(fileIndexingDone()), this, SLOT(startIndexing()) );
}

void NepomukWebMinerService::fileIndexerDisabled()
{
    Q_D(NepomukWebMinerService);

    delete d->dBusFileIndexer;
    d->dBusFileIndexer = 0;
}

void NepomukWebMinerService::startIndexing()
{
    Q_D(NepomukWebMinerService);

    d->indexScheduler->slotScheduleIndexing();
}

#include <kpluginfactory.h>
#include <kpluginloader.h>

NEPOMUK_EXPORT_SERVICE(NepomukWebMinerService, "nepomuk-webminerservice")

#include "nepomukwebminerservice.moc"
