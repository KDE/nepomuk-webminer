/*
 * Copyright 2012 Lim Yuen Hoe <yuenhoe@hotmail.com>
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

#include "pluginlist.h"
#include "ui_pluginlist.h"

#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <KDE/KMessageBox>
#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

PluginList::PluginList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginList)
{
    extractorFactory = 0;
    selectedPlugin = 0;
    ui->setupUi(this);
}

PluginList::~PluginList()
{
    delete ui;
}

void PluginList::setExtractorFactory(NepomukMetaDataExtractor::Extractor::ExtractorFactory  *ef)
{
    extractorFactory = ef;
    setupUi();
}

void PluginList::setupUi()
{
    // music list
    QList<WebExtractor::Info> engines = extractorFactory->listAvailablePlugins( "music" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        QListWidgetItem *item = new QListWidgetItem(QIcon(iconPath), engine.name);
        item->setData(5, engine.identifier);
        ui->pluginList->addItem(item);
    }

    // publication list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "publication" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        QListWidgetItem *item = new QListWidgetItem(QIcon(iconPath), engine.name);
        item->setData(5, engine.identifier);
        ui->pluginList->addItem(item);
    }

    // movie list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "movie" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        QListWidgetItem *item = new QListWidgetItem(QIcon(iconPath), engine.name);
        item->setData(5, engine.identifier);
        ui->pluginList->addItem(item);
    }

    // tvshow list
    engines.clear();
    engines = extractorFactory->listAvailablePlugins( "tvshow" );

    foreach(const WebExtractor::Info &engine, engines) {
        QFileInfo fileInfo(engine.file);
        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        QListWidgetItem *item = new QListWidgetItem(QIcon(iconPath), engine.name);
        item->setData(5, engine.identifier);
        ui->pluginList->addItem(item);
    }

    connect(ui->pluginList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateButtons(QListWidgetItem*)) );
    connect(ui->infoButton, SIGNAL(clicked()), this, SLOT(showInfo()));
    connect(ui->configButton, SIGNAL(clicked()), this, SLOT(showConfig()));
    ui->infoButton->setDisabled(true);
    ui->configButton->setDisabled(true);
}

void PluginList::updateButtons(QListWidgetItem* item) {
    selectedPlugin = extractorFactory->getExtractor(item->data(5).toString()); // the factory automatically reuses existing instances
    ui->infoButton->setDisabled(false);
    if (selectedPlugin->info().hasConfig) {
        ui->configButton->setDisabled(false);
    } else {
        ui->configButton->setDisabled(true);
    }
}

void PluginList::showInfo() {
    //TODO: more interesting info dialog?
    if (!selectedPlugin) {
        return; // nothing to show
    }
    QString description = "";
    WebExtractor::Info info = selectedPlugin->info();
    description += info.description;
    description += "\n\n";
    description += i18n("Author: %1", info.author) + "\n";
    description += i18n("Email: %1", info.email) + "\n";
    KMessageBox::information(this, description, i18n("About %1", info.name));
}

void PluginList::showConfig() {
    if (!selectedPlugin) {
        return; // nothing to show
    }
    QTimer::singleShot(0, selectedPlugin, SLOT(showConfigDialog()));
}