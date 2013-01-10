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

#include "configfetcher.h"
#include "ui_configfetcher.h"

#include <mdesettings.h>

#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include <KDE/KConfig>
#include <KDE/KConfigGroup>

#include <QtDBus/QDBusServiceWatcher>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtCore/QProcess>

#include <KDE/KDebug>

using namespace NepomukWebMiner;
using namespace Extractor;

ConfigFetcher::ConfigFetcher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigFetcher)
{
    ui->setupUi(this);
}

ConfigFetcher::~ConfigFetcher()
{
    delete ui;
}

void ConfigFetcher::setExtractorFactory(NepomukWebMiner::Extractor::ExtractorFactory  *ef)
{
    extractorFactory = ef;
    setupUi();
}

void ConfigFetcher::serviceEnabled(bool enabled)
{
    if (enabled) {
        //if we check it here, we can be sure this is not the first run
        // as we only disable the service again on first run (to not fetch metdata behidn the users back after
        // installation, this is not needed here)
        MDESettings::setFirstRun(false);
        MDESettings::self()->writeConfig();

        QProcess::startDetached(QLatin1String("nepomukservicestub nepomuk-webminerservice"));
    } else {
        QDBusInterface service("org.kde.nepomuk.services.nepomuk-webminerservice", "/servicecontrol",
                               "org.kde.nepomuk.ServiceControl");
        service.call("shutdown");
    }

    KConfig config(QLatin1String("nepomukserverrc"));
    KConfigGroup generalGroup(&config, QLatin1String("Service-nepomuk-webminerservice"));
    generalGroup.writeEntry(QLatin1String("autostart"), enabled);
    generalGroup.sync();
}

void ConfigFetcher::serviceRegistered()
{
    ui->enableService->setChecked(true);
}

void ConfigFetcher::serviceUnregistered()
{
    ui->enableService->setChecked(false);
}

void ConfigFetcher::updateConfiguration()
{
    emit configChanged(true);
}

void ConfigFetcher::saveConfig()
{
    MDESettings::setDownloadBanner(ui->kcfg_DownloadBanner->isChecked());
    MDESettings::setDownloadReferences(ui->kcfg_DownloadReferences->isChecked());
    MDESettings::setSaveBannerInResourceFolder(ui->kcfg_SaveBannerInResourceFolder->isChecked());

    int curIndex = ui->musicPlugin->currentIndex();
    MDESettings::setFavoriteMusicPlugin(ui->musicPlugin->itemData(curIndex).toString());

    curIndex = ui->publicationPlugin->currentIndex();
    MDESettings::setFavoritePublicationPlugin(ui->publicationPlugin->itemData(curIndex).toString());

    curIndex = ui->moviePlugin->currentIndex();
    MDESettings::setFavoriteMoviePlugin(ui->moviePlugin->itemData(curIndex).toString());

    curIndex = ui->tvshowPlugin->currentIndex();
    MDESettings::setFavoriteTvShowPlugin(ui->tvshowPlugin->itemData(curIndex).toString());

    MDESettings::setDocumentServiceEnabled(ui->kcfg_FetchDocuments->isChecked());
    MDESettings::setVideoServiceEnabled(ui->kcfg_FetchVideos->isChecked());
    MDESettings::setMusicServiceEnabled(ui->kcfg_FetchMusic->isChecked());

    MDESettings::self()->writeConfig();
}

void ConfigFetcher::resetConfig()
{
    ui->kcfg_DownloadBanner->setChecked(MDESettings::downloadBanner());
    ui->kcfg_DownloadReferences->setChecked(MDESettings::downloadReferences());
    ui->kcfg_SaveBannerInResourceFolder->setChecked(MDESettings::saveBannerInResourceFolder());

    ui->musicPlugin->setCurrentIndex(ui->musicPlugin->findData(MDESettings::favoriteMusicPlugin()));
    ui->publicationPlugin->setCurrentIndex(ui->publicationPlugin->findData(MDESettings::favoritePublicationPlugin()));
    ui->moviePlugin->setCurrentIndex(ui->moviePlugin->findData(MDESettings::favoriteMoviePlugin()));
    ui->tvshowPlugin->setCurrentIndex(ui->tvshowPlugin->findData(MDESettings::favoriteTvShowPlugin()));

    ui->kcfg_FetchDocuments->setChecked( MDESettings::documentServiceEnabled());
    ui->kcfg_FetchVideos->setChecked( MDESettings::videoServiceEnabled());
    ui->kcfg_FetchMusic->setChecked( MDESettings::musicServiceEnabled());
}

void ConfigFetcher::setupUi()
{
    //######################
    //# general settings
    ui->kcfg_DownloadBanner->setChecked(MDESettings::downloadBanner());
    connect(ui->kcfg_DownloadBanner, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()));
    ui->kcfg_DownloadReferences->setChecked(MDESettings::downloadReferences());
    connect(ui->kcfg_DownloadReferences, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()));
    ui->kcfg_SaveBannerInResourceFolder->setChecked(MDESettings::saveBannerInResourceFolder());
    connect(ui->kcfg_SaveBannerInResourceFolder, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()));

    //######################
    //# Plugin selection
    // music list
    QList<WebExtractor::Info> engines = extractorFactory->listAvailablePlugins("music");

    foreach (const WebExtractor::Info & engine, engines) {
        ui->musicPlugin->addItem(QIcon(engine.icon), engine.name, engine.identifier);
    }

    // publication list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins("publication");

    foreach (const WebExtractor::Info & engine, engines) {
        ui->publicationPlugin->addItem(QIcon(engine.icon), engine.name, engine.identifier);
    }

    // movie list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins("movie");

    foreach (const WebExtractor::Info & engine, engines) {
        ui->moviePlugin->addItem(QIcon(engine.icon), engine.name, engine.identifier);
    }

    // tvshow list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins("tvshow");

    foreach (const WebExtractor::Info & engine, engines) {
        ui->tvshowPlugin->addItem(QIcon(engine.icon), engine.name, engine.identifier);
    }

    ui->musicPlugin->setCurrentIndex(ui->musicPlugin->findData(MDESettings::favoriteMusicPlugin()));
    ui->publicationPlugin->setCurrentIndex(ui->publicationPlugin->findData(MDESettings::favoritePublicationPlugin()));
    ui->moviePlugin->setCurrentIndex(ui->moviePlugin->findData(MDESettings::favoriteMoviePlugin()));
    ui->tvshowPlugin->setCurrentIndex(ui->tvshowPlugin->findData(MDESettings::favoriteTvShowPlugin()));

    connect(ui->musicPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()));
    connect(ui->publicationPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()));
    connect(ui->moviePlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()));
    connect(ui->tvshowPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()));

    //######################
    //# Background service
    connect(ui->enableService, SIGNAL(clicked(bool)), this, SLOT(serviceEnabled(bool)));

    m_watcher = new QDBusServiceWatcher(this);
    m_watcher->addWatchedService(QLatin1String("org.kde.nepomuk.services.nepomuk-webminerservice"));
    m_watcher->setConnection(QDBusConnection::sessionBus());
    m_watcher->setWatchMode(QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration);

    connect(m_watcher, SIGNAL(serviceRegistered(QString)), this, SLOT(serviceRegistered()));
    connect(m_watcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(serviceUnregistered()));

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.nepomuk.services.nepomuk-webminerservice")) {
        ui->enableService->setChecked(true);
    }

    ui->kcfg_FetchDocuments->setChecked( MDESettings::documentServiceEnabled());
    ui->kcfg_FetchVideos->setChecked( MDESettings::videoServiceEnabled());
    ui->kcfg_FetchMusic->setChecked( MDESettings::musicServiceEnabled());

    connect(ui->kcfg_FetchDocuments, SIGNAL(clicked(bool)), this, SLOT(updateConfiguration()));
    connect(ui->kcfg_FetchVideos, SIGNAL(clicked(bool)), this, SLOT(updateConfiguration()));
    connect(ui->kcfg_FetchMusic, SIGNAL(clicked(bool)), this, SLOT(updateConfiguration()));
}
