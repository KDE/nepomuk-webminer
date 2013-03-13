/*
 * Copyright 2012 Lim Yuen Hoe <yuenhoe@hotmail.com>
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

#include "pluginlist.h"
#include "ui_pluginlist.h"

#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include <KDE/KUrlLabel>
#include <KDE/KDebug>
#include <KDE/Kross/Manager>
#include <KDE/KTextEdit>

#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <QtCore/QPointer>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QFormLayout>
#include <QtGui/QBrush>
#include <QtGui/QColor>

using namespace NepomukWebMiner;
using namespace Extractor;

PluginList::PluginList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginList)
{
    extractorFactory = 0;
    ui->setupUi(this);
}

PluginList::~PluginList()
{
    delete ui;
}

void PluginList::setExtractorFactory(NepomukWebMiner::Extractor::ExtractorFactory  *ef)
{
    extractorFactory = ef;
    setupUi();
}

void PluginList::setupUi()
{
    QList<WebExtractor::Info> engines = extractorFactory->listAvailablePlugins("music");
    engines.append(extractorFactory->listAvailablePlugins("publication"));
    engines.append(extractorFactory->listAvailablePlugins("movie"));
    engines.append(extractorFactory->listAvailablePlugins("tvshow"));

    failedPlugins = extractorFactory->listFailedPlugins("music");
    failedPlugins.append(extractorFactory->listFailedPlugins("publication"));
    failedPlugins.append(extractorFactory->listFailedPlugins("movie"));
    failedPlugins.append(extractorFactory->listFailedPlugins("tvshow"));

    foreach (const WebExtractor::Info & engine, engines) {
        if (!ui->pluginList->findItems(engine.name, Qt::MatchExactly).isEmpty()) {
            continue; // already added
        }

        //QFileInfo fileInfo(engine.file);
        //QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        QListWidgetItem *item = new QListWidgetItem(KIcon(engine.icon), engine.name);
        item->setData(5, engine.identifier);
        ui->pluginList->addItem(item);
    }

    // also show failed plugins, but make it obvious they are erroneous
    foreach (const WebExtractor::Info & engine, failedPlugins) {
        if (!ui->pluginList->findItems(engine.name, Qt::MatchExactly).isEmpty()) {
            continue; // already added
        }

        QListWidgetItem *item = new QListWidgetItem(KIcon("dialog-error"), engine.name);
        item->setBackground(QBrush(QColor(255,200,200)));
        item->setData(5, engine.identifier);
        ui->pluginList->addItem(item);
    }

    QString itext = i18n("Available Kross interpreters: <b>%1</b><br />"
                         "If you cannot find a plugin that should be here, "
                         "you may need to install the interpreter for the "
                         "necessary language first.",
                         Kross::Manager::self().interpreters().join(", "));
    ui->interpreterLabel->setText(itext);

    connect(ui->pluginList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateButtons(QListWidgetItem*)));
    connect(ui->infoButton, SIGNAL(clicked()), this, SLOT(showInfo()));
    connect(ui->configButton, SIGNAL(clicked()), this, SLOT(showConfig()));
    ui->infoButton->setDisabled(true);
    ui->configButton->setDisabled(true);
}

void PluginList::updateButtons(QListWidgetItem* item)
{
    selectedPlugin = item->data(5).toString();
    ui->infoButton->setDisabled(false);
    // check and handle failed plugin
    foreach (const WebExtractor::Info & engine, failedPlugins) {
        if (engine.identifier == selectedPlugin) {
            ui->configButton->setDisabled(true);
            return;
        }
    }
    // handle loaded plugin
    WebExtractor *thePlugin = extractorFactory->getExtractor(selectedPlugin); // the factory automatically reuses existing instances
    if (thePlugin->info().hasConfig) {
        ui->configButton->setDisabled(false);
    } else {
        ui->configButton->setDisabled(true);
    }
}

void PluginList::showInfo()
{
    if (selectedPlugin.isNull()) {
        return; // nothing to show
    }

    WebExtractor::Info info;
    WebExtractor *thePlugin = 0;
    // check if it's a failed plugin
    bool failed = false;
    foreach (const WebExtractor::Info & engine, failedPlugins) {
        if (engine.identifier == selectedPlugin) {
            info = engine;
            failed = true;
        }
    }
    if (!failed) {
        thePlugin = extractorFactory->getExtractor(selectedPlugin);
        info = thePlugin->info();
    }
    QPointer<QDialog> dlg = new QDialog(this);

    dlg->setWindowTitle(i18n("About %1", info.name));
    QFormLayout *formLayout = new QFormLayout(this);

    formLayout->addRow(QLatin1String("<b>") + i18n("Name:") + QLatin1String("</b>"), new QLabel(info.name));
    formLayout->addRow(QLatin1String("<b>") + i18n("Link:") + QLatin1String("</b>"), new KUrlLabel(info.homepage));
    QLabel *desc = new QLabel(info.description);
    desc->setWordWrap(true);
    formLayout->addRow(QLatin1String("<b>") + i18n("Description:") + QLatin1String("</b>"), desc);
    formLayout->addRow(QLatin1String("<b>") + i18n("Author:") + QLatin1String("</b>"), new QLabel(info.author));
    formLayout->addRow(QLatin1String("<b>") + i18n("Email:") + QLatin1String("</b>"), new QLabel(info.email));
    if (failed) {
        KTextEdit *err = new KTextEdit(info.error);
        err->setReadOnly(true);
        formLayout->addRow(QLatin1String("<b style='color:#f00'>") + i18n("Error:") + QLatin1String("</b>"), err);
    }
    dlg->setLayout(formLayout);

    dlg->exec();
    delete dlg;
}

void PluginList::showConfig()
{
    if (selectedPlugin.isNull()) {
        return; // nothing to show
    }

    WebExtractor *thePlugin = extractorFactory->getExtractor(selectedPlugin);
    QTimer::singleShot(0, thePlugin, SLOT(showConfigDialog()));
}
