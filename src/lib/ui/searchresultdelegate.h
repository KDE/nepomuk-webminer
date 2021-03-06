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

#ifndef SEARCHRESULTDELEGATE_H
#define SEARCHRESULTDELEGATE_H

#include <QtGui/QStyledItemDelegate>

namespace NepomukWebMiner
{
namespace UI
{

/**
  * @brief Delegate to show the search result information in a nice way.
  *
  * Displays the @c title in bold font and the @c details in a smaller italic font below it
  */
class SearchResultDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SearchResultDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
};
}
}

#endif // SEARCHRESULTDELEGATE_H
