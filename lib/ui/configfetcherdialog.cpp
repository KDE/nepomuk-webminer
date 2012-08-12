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

#include "configfetcherdialog.h"
#include "ui_configfetcherdialog.h"

#include <mdesettings.h>

#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include <QtCore/QFileInfo>

#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

ConfigFetcherDialog::ConfigFetcherDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigFetcherDialog)
{
    ui->setupUi(this);
}

ConfigFetcherDialog::~ConfigFetcherDialog()
{
    delete ui;
}

void ConfigFetcherDialog::setExtractorFactory(NepomukMetaDataExtractor::Extractor::ExtractorFactory  *ef)
{
    extractorFactory = ef;
    setupUi();
}

void ConfigFetcherDialog::updateConfiguration()
{
    kDebug() << "updateConfiguration";
    int curIndex = ui->musicPlugin->currentIndex();
    MDESettings::setFavoriteMusicPlugin( ui->musicPlugin->itemData(curIndex).toString() );

    curIndex = ui->publicationPlugin->currentIndex();
    MDESettings::setFavoritePublicationPlugin( ui->publicationPlugin->itemData(curIndex).toString() );

    curIndex = ui->moviePlugin->currentIndex();
    MDESettings::setFavoriteMoviePlugin( ui->moviePlugin->itemData(curIndex).toString() );

    curIndex = ui->tvshowPlugin->currentIndex();
    MDESettings::setFavoriteTvShowPlugin( ui->tvshowPlugin->itemData(curIndex).toString() );
}

void ConfigFetcherDialog::setupUi()
{
    // music list
    QList<WebExtractor::Info> engines = extractorFactory->listAvailablePlugins( "music" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        ui->musicPlugin->addItem(QIcon( iconPath ),engine.name,engine.identifier);
    }

    // publication list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "publication" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        ui->publicationPlugin->addItem(QIcon( iconPath ),engine.name,engine.identifier);
    }

    // movie list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "movie" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        ui->moviePlugin->addItem(QIcon( iconPath ),engine.name,engine.identifier);
    }

    // tvshow list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "tvshow" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        ui->tvshowPlugin->addItem(QIcon( iconPath ),engine.name,engine.identifier);
    }

    ui->musicPlugin->setCurrentIndex(ui->musicPlugin->findData(MDESettings::favoriteMusicPlugin()));
    ui->publicationPlugin->setCurrentIndex(ui->publicationPlugin->findData(MDESettings::favoritePublicationPlugin()));
    ui->moviePlugin->setCurrentIndex(ui->moviePlugin->findData(MDESettings::favoriteMoviePlugin()));
    ui->tvshowPlugin->setCurrentIndex(ui->tvshowPlugin->findData(MDESettings::favoriteTvShowPlugin()));

    //FIXME: where to connect this to make the apply button appear?
//    connect(ui->musicPlugin, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()) );
//    connect(ui->publicationPlugin, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()) );
//    connect(ui->moviePlugin, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()) );
//    connect(ui->tvshowPlugin, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()) );

    connect(ui->musicPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );
    connect(ui->publicationPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );
    connect(ui->moviePlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );
    connect(ui->tvshowPlugin, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfiguration()) );

}
