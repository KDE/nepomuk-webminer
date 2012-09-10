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

#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QtGui/QWidget>

#include <QtCore/QVariantList>
#include <QtCore/QModelIndex>

class QStandardItem;

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class MetaDataParameters;
}

namespace UI {
    class MetaDataWidgetPrivate;

/**
 * @brief Small widget to show the metadata downloaded from the web
 *
 * The metdata is presented in a TreeView
 *
 * @todo TODO: make the data editable (need to find out how to manipulate the QVariantMap with the nested structed in a good way)
 * @todo TODO: reenable KTabWidget and show also the current resource data and the difference between old/new data
 * @todo TODO: allow to remove metadata via right click menu
 */
class MetaDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MetaDataWidget(QWidget *parent = 0);
    
    /**
     * @brief Tell the Widget which metadata will be shown
     *
     * @param mdp the object that will be shown
     */
    void setMetaDataParameter(const NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp);

    /**
     * @brief Shows a spinner icon to indicate that the data fetching is in progress
     * @param busy @arg true show spinner
     *             @arg false hdie spinner
     */
    void setBusy(bool busy);

private:
    void setupWidget();

    void setUpNewDataWidget();
    void insertIntoTree(QStandardItem *root, const QVariantMap &data);

    /**
     * @brief Little helper function to display a translated string instead of the QMap key values used internally
     *
     * @param key the key that will be translated
     * @return the translated key
     */
    QString i18nKey(const QString &key);

    Q_DECLARE_PRIVATE(MetaDataWidget)
    MetaDataWidgetPrivate *const d_ptr; /**< d-pointer for this class */
    
};
}
}

#endif // METADATAWIDGET_H
