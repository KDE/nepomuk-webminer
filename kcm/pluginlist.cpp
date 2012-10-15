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

#include <KDE/KUrlLabel>
#include <KDE/KDebug>

#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <QtCore/QPointer>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QFormLayout>

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
    engines.append( extractorFactory->listAvailablePlugins( "publication" ) );
    engines.append( extractorFactory->listAvailablePlugins( "movie" ) );
    engines.append( extractorFactory->listAvailablePlugins( "tvshow" ) );

    foreach(const WebExtractor::Info &engine, engines) {
        if( !ui->pluginList->findItems(engine.name, Qt::MatchExactly).isEmpty()) {
            continue; // already added
        }

        //QFileInfo fileInfo(engine.file);
        //QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        QListWidgetItem *item = new QListWidgetItem(KIcon(engine.icon), engine.name);
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
    if (!selectedPlugin) {
        return; // nothing to show
    }

    WebExtractor::Info info = selectedPlugin->info();
    QPointer<QDialog> dlg = new QDialog( this );

    dlg->setWindowTitle(i18n("About %1", info.name) );
    QFormLayout *formLayout = new QFormLayout(this);

    formLayout->addRow(QLatin1String("<b>") + i18n("Name:") + QLatin1String("</b>"), new QLabel(info.name));
    formLayout->addRow(QLatin1String("<b>") + i18n("Link:") + QLatin1String("</b>"), new KUrlLabel(info.homepage));
    QLabel *desc = new QLabel(info.description);
    desc->setWordWrap( true );
    formLayout->addRow(QLatin1String("<b>") + i18n("Description:") + QLatin1String("</b>"), desc);
    formLayout->addRow(QLatin1String("<b>") + i18n("Author:") + QLatin1String("</b>"), new QLabel(info.author));
    formLayout->addRow(QLatin1String("<b>") + i18n("Email:") + QLatin1String("</b>"), new QLabel(info.email));
    dlg->setLayout(formLayout);

    dlg->exec();
    delete dlg;
}

void PluginList::showConfig() {
    if (!selectedPlugin) {
        return; // nothing to show
    }

    QTimer::singleShot(0, selectedPlugin, SLOT(showConfigDialog()));
}
