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

#ifndef PLUGINLIST_H
#define PLUGINLIST_H

#include <QWidget>

class QListWidgetItem;

namespace Ui {
class PluginList;
}

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class ExtractorFactory;
    class WebExtractor;
}

/**
 * @brief Shows a list of all available plugins and their description.
 *
 * If a plugin has a configuration interface, it can be shown here
 */
class PluginList : public QWidget
{
    Q_OBJECT

public:
    explicit PluginList(QWidget *parent = 0);
    ~PluginList();

    void setExtractorFactory(NepomukMetaDataExtractor::Extractor::ExtractorFactory  *ef);

private slots:
    void updateButtons(QListWidgetItem*);
    void showInfo();
    void showConfig();

private:
    void setupUi();
    Ui::PluginList *ui;
    NepomukMetaDataExtractor::Extractor::ExtractorFactory  *extractorFactory;
    NepomukMetaDataExtractor::Extractor::WebExtractor  *selectedPlugin;

};

}

#endif // PLUGINLIST_H
