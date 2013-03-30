/*
 * Copyright 2013 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#ifndef METADATABASEWIDGET_H
#define METADATABASEWIDGET_H

#include <QtGui/QWidget>
#include <QtCore/QVariantMap>

namespace NepomukWebMiner
{
namespace UI
{
/**
 * @brief Base Class for all Widget that implement custom metadata visualization/editing
 *
 * Each of these Widgets must implement the abstarct methods from this class.
 * The idea is, to visualize the metadata i na special way to allow the user to
 * see what the data means and to edit them as they see it would fit.
 *
 * Before the data is saved, the metaData() function is called that retrieves the
 * correct and final metadata.
 */
class MetaDataBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MetaDataBaseWidget(QWidget *parent = 0);
    virtual ~MetaDataBaseWidget();

    /**
     * @brief Sets the Metadata variantmap to visualize it
     *
     * @param metadata current metadata map
     */
    virtual void setMetaData(const QVariantMap &metadata) = 0;

    /**
     * @brief Returns teh metadata that will be used before saving the file
     *
     * @return user extended metadata
     */
    virtual QVariantMap metaData() const = 0;
};
}
}

#endif // METADATABASEWIDGET_H
