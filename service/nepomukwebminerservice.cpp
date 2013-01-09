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

#include <QtDBus/QDBusInterface>

#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <KDE/KDebug>

class NepomukWebMinerServicePrivate
{
public:
    IndexScheduler *indexScheduler;
};

NepomukWebMinerService::NepomukWebMinerService(QObject *parent, const QVariantList &)
    : Nepomuk2::Service(parent)
    , d_ptr(new NepomukWebMinerServicePrivate)
{
    Q_D(NepomukWebMinerService);

    KConfig config("nepomukwebminerrc");
    KConfigGroup serviceGroup(&config, "Service");

    bool firstRun = serviceGroup.readEntry("FirstRun", true);

    // This disabled the service on first run
    // Normally when the service is installed it will be enabled by default.
    // This would expose private data to the internet and might not be what a user expects
    // when he installs the metadata extractor, to avoid this the extractor must be enabled
    // explicitly in the KCM config settings
    if (firstRun) {
        KConfig serverConfig(QLatin1String("nepomukserverrc"));
        KConfigGroup serverGroup(&serverConfig, QLatin1String("Service-nepomuk-webminerservice"));
        serverGroup.writeEntry(QLatin1String("autostart"), false);
        serverGroup.sync();

        serviceGroup.writeEntry("FirstRun", false);
        serviceGroup.sync();

        QDBusInterface service("org.kde.nepomuk.services.nepomuk-webminerservice", "/servicecontrol",
                               "org.kde.nepomuk.ServiceControl");
        service.call("shutdown");

        return;
    }

    d->indexScheduler = new IndexScheduler( this );

    connect( d->indexScheduler, SIGNAL(statusStringChanged()), this, SIGNAL(statusStringChanged()) );

    connect( this, SIGNAL( statusStringChanged() ), this, SIGNAL( statusChanged() ) );
    connect( d->indexScheduler, SIGNAL( indexingStarted() ), this, SIGNAL( indexingStarted() ) );
    connect( d->indexScheduler, SIGNAL( indexingStopped() ), this, SIGNAL( indexingStopped() ) );
    connect( d->indexScheduler, SIGNAL( indexingFolder(QString) ), this, SIGNAL( indexingFolder(QString) ) );
}

NepomukWebMinerService::~NepomukWebMinerService()
{
    Q_D(NepomukWebMinerService);

    delete d->indexScheduler;
}

bool NepomukWebMinerService::isSuspended() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->isSuspended();
}

bool NepomukWebMinerService::isIndexing() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->isIndexing();
}

QString NepomukWebMinerService::userStatusString() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->userStatusString();
}

QUrl NepomukWebMinerService::currentUrl() const
{
    Q_D(const NepomukWebMinerService);

    return d->indexScheduler->currentUrl();
}



#include <kpluginfactory.h>
#include <kpluginloader.h>

NEPOMUK_EXPORT_SERVICE(NepomukWebMinerService, "nepomuk-webminerservice")

#include "nepomukwebminerservice.moc"
