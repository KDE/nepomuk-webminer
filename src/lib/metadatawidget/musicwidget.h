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

#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include "metadatabasewidget.h"

namespace NepomukWebMiner
{
namespace UI
{
class MusicWidgetPrivate;

/**
 * @brief Shows metadata for music resources
 */
class MusicWidget : public MetaDataBaseWidget
{
    Q_OBJECT
    
public:
    explicit MusicWidget(QWidget *parent = 0);
    ~MusicWidget();

    /**
     * @brief Sets the Metadata variantmap to visualize it
     *
     * @param metadata current metadata map
     */
    void setMetaData(const QVariantMap &musicAlbum);

    /**
     * @brief Returns the metadata that will be used before saving the file
     *
     * @return user extended metadata
     */
    QVariantMap metaData() const;

private:
    Q_DECLARE_PRIVATE(MusicWidget)
    MusicWidgetPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // MUSICWIDGET_H
