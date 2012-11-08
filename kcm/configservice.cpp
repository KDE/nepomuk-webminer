/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#include "configservice.h"
#include "ui_configservice.h"

#include <mdesettings.h>

#include <KDE/KConfig>
#include <KDE/KConfigGroup>

#include <QtDBus/QDBusServiceWatcher>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtCore/QProcess>

using namespace NepomukMetaDataExtractor;

ConfigService::ConfigService(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigService)
{
    ui->setupUi(this);

    connect(ui->enableService, SIGNAL(clicked(bool)), this, SLOT(serviceEnabled(bool)));

    m_watcher = new QDBusServiceWatcher(this);
    m_watcher->addWatchedService(QLatin1String("org.kde.nepomuk.services.metadataextractorservice"));
    m_watcher->setConnection(QDBusConnection::sessionBus());
    m_watcher->setWatchMode(QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration);

    connect(m_watcher, SIGNAL(serviceRegistered(QString)), this, SLOT(serviceRegistered()));
    connect(m_watcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(serviceUnregistered()));

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.nepomuk.services.metadataextractorservice")) {
        ui->enableService->setChecked(true);
    }

    ui->kintspinbox->setValue(MDESettings::simultaneousCalls());
    connect(ui->kintspinbox, SIGNAL(valueChanged(int)), this, SLOT(changeSettings()));

    ui->documentServiceBox->setChecked(MDESettings::documentServiceEnabled());
    connect(ui->documentServiceBox, SIGNAL(toggled(bool)), this, SLOT(changeSettings()));

    ui->musicServiceBox->setChecked(MDESettings::musicServiceEnabled());
    connect(ui->musicServiceBox, SIGNAL(toggled(bool)), this, SLOT(changeSettings()));

    ui->videoServiceBox->setChecked(MDESettings::videoServiceEnabled());
    connect(ui->videoServiceBox, SIGNAL(toggled(bool)), this, SLOT(changeSettings()));
}

ConfigService::~ConfigService()
{
    delete ui;
    delete m_watcher;
}

void ConfigService::serviceEnabled(bool enabled)
{
    if (enabled) {
        QProcess::startDetached(QLatin1String("nepomukservicestub metadataextractorservice"));
    } else {
        QDBusInterface service("org.kde.nepomuk.services.metadataextractorservice", "/servicecontrol",
                               "org.kde.nepomuk.ServiceControl");
        service.call("shutdown");
    }

    KConfig config(QLatin1String("nepomukserverrc"));
    KConfigGroup generalGroup(&config, QLatin1String("Service-metadataextractorservice"));
    generalGroup.writeEntry(QLatin1String("autostart"), enabled);
    generalGroup.sync();
}

void ConfigService::serviceRegistered()
{
    ui->enableService->setChecked(true);
}

void ConfigService::serviceUnregistered()
{
    ui->enableService->setChecked(false);
}

void ConfigService::changeSettings()
{
    emit configChanged(true);
}

void ConfigService::saveConfig()
{
    MDESettings::setSimultaneousCalls(ui->kintspinbox->value());
    MDESettings::setDocumentServiceEnabled(ui->documentServiceBox->isChecked());
    MDESettings::setMusicServiceEnabled(ui->musicServiceBox->isChecked());
    MDESettings::setVideoServiceEnabled(ui->videoServiceBox->isChecked());

    MDESettings::self()->writeConfig();
}

void ConfigService::resetConfig()
{
    ui->kintspinbox->setValue(MDESettings::simultaneousCalls());

    ui->documentServiceBox->setChecked(MDESettings::documentServiceEnabled());
    ui->musicServiceBox->setChecked(MDESettings::musicServiceEnabled());
    ui->videoServiceBox->setChecked(MDESettings::videoServiceEnabled());
}
