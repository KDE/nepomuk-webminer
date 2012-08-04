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

#ifndef SEARCHRESULTSMODEL_H
#define SEARCHRESULTSMODEL_H

#include <QtCore/QAbstractListModel>

namespace NepomukMetaDataExtractor {
namespace UI {
/**
  * @brief ListModel to present the @c search @c result @c entries in a nice way
  *
  * Uses the @c title and @c details key of the @c QVariantMap from the python call
  */
class SearchResultsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SearchResultsModel(QObject *parent = 0);

    void setSearchResults(const QVariantList & searchResults);
    QVariantMap searchResultEntry( const QModelIndex & index );
    void clear( );

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const ;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

private:
    QVariantList m_searchResults;
};
}
}

#endif // SEARCHRESULTSMODEL_H
