/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
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

#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor;
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

void ConfigFetcher::setExtractorFactory(NepomukMetaDataExtractor::Extractor::ExtractorFactory  *ef)
{
    extractorFactory = ef;
    setupUi();
}

void ConfigFetcher::updateConfiguration()
{
    emit configChanged(true);
}

void ConfigFetcher::saveConfig()
{
    MDESettings::setDownloadBanner( ui->kcfg_DownloadBanner->isChecked() );
    MDESettings::setDownloadReferences( ui->kcfg_DownloadReferences->isChecked() );
    MDESettings::setSaveBannerInResourceFolder( ui->kcfg_SaveBannerInResourceFolder->isChecked() );

    int curIndex = ui->musicPlugin->currentIndex();
    MDESettings::setFavoriteMusicPlugin( ui->musicPlugin->itemData(curIndex).toString() );

    curIndex = ui->publicationPlugin->currentIndex();
    MDESettings::setFavoritePublicationPlugin( ui->publicationPlugin->itemData(curIndex).toString() );

    curIndex = ui->moviePlugin->currentIndex();
    MDESettings::setFavoriteMoviePlugin( ui->moviePlugin->itemData(curIndex).toString() );

    curIndex = ui->tvshowPlugin->currentIndex();
    MDESettings::setFavoriteTvShowPlugin( ui->tvshowPlugin->itemData(curIndex).toString() );

    MDESettings::setCheckNextPlugin( ui->checkOtherPlugins->isChecked() );

    MDESettings::self()->writeConfig();
}

void ConfigFetcher::resetConfig()
{
    ui->kcfg_DownloadBanner->setChecked( MDESettings::downloadBanner());
    ui->kcfg_DownloadReferences->setChecked( MDESettings::downloadReferences());
    ui->kcfg_SaveBannerInResourceFolder->setChecked( MDESettings::saveBannerInResourceFolder());

    ui->musicPlugin->setCurrentIndex(ui->musicPlugin->findData(MDESettings::favoriteMusicPlugin()));
    ui->publicationPlugin->setCurrentIndex(ui->publicationPlugin->findData(MDESettings::favoritePublicationPlugin()));
    ui->moviePlugin->setCurrentIndex(ui->moviePlugin->findData(MDESettings::favoriteMoviePlugin()));
    ui->tvshowPlugin->setCurrentIndex(ui->tvshowPlugin->findData(MDESettings::favoriteTvShowPlugin()));

    ui->checkOtherPlugins->setChecked( MDESettings::checkNextPlugin() );
}

void ConfigFetcher::setupUi()
{
    ui->kcfg_DownloadBanner->setChecked( MDESettings::downloadBanner());
    connect(ui->kcfg_DownloadBanner, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()) );
    ui->kcfg_DownloadReferences->setChecked( MDESettings::downloadReferences());
    connect(ui->kcfg_DownloadReferences, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()) );
    ui->kcfg_SaveBannerInResourceFolder->setChecked( MDESettings::saveBannerInResourceFolder());
    connect(ui->kcfg_SaveBannerInResourceFolder, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()) );

    // music list
    QList<WebExtractor::Info> engines = extractorFactory->listAvailablePlugins( "music" );

    foreach(const WebExtractor::Info &engine, engines) {
        ui->musicPlugin->addItem(QIcon( engine.icon ),engine.name,engine.identifier);
    }

    // publication list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "publication" );

    foreach(const WebExtractor::Info &engine, engines) {
        ui->publicationPlugin->addItem(QIcon( engine.icon ),engine.name,engine.identifier);
    }

    // movie list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "movie" );

    foreach(const WebExtractor::Info &engine, engines) {
        ui->moviePlugin->addItem(QIcon( engine.icon ),engine.name,engine.identifier);
    }

    // tvshow list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "tvshow" );

    foreach(const WebExtractor::Info &engine, engines) {
        ui->tvshowPlugin->addItem(QIcon( engine.icon ),engine.name,engine.identifier);
    }

    ui->musicPlugin->setCurrentIndex(ui->musicPlugin->findData(MDESettings::favoriteMusicPlugin()));
    ui->publicationPlugin->setCurrentIndex(ui->publicationPlugin->findData(MDESettings::favoritePublicationPlugin()));
    ui->moviePlugin->setCurrentIndex(ui->moviePlugin->findData(MDESettings::favoriteMoviePlugin()));
    ui->tvshowPlugin->setCurrentIndex(ui->tvshowPlugin->findData(MDESettings::favoriteTvShowPlugin()));

    ui->checkOtherPlugins->setChecked( MDESettings::checkNextPlugin() );

    connect(ui->musicPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );
    connect(ui->publicationPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );
    connect(ui->moviePlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );
    connect(ui->tvshowPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );

    connect(ui->checkOtherPlugins, SIGNAL(toggled(bool)), this, SLOT(updateConfiguration()) );
}
