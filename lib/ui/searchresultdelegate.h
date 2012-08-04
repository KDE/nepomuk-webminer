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

#ifndef SEARCHRESULTDELEGATE_H
#define SEARCHRESULTDELEGATE_H

#include <QtGui/QStyledItemDelegate>

namespace NepomukMetaDataExtractor {
namespace UI {
/**
  * @brief Delegate to show the search result information in a nice way.
  *
  * Displays the title in bold font and the details in a smaller italic font below it
  */
class SearchResultDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SearchResultDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const;

    QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};
}
}

#endif // SEARCHRESULTDELEGATE_H
