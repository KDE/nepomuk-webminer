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

#include "searchresultsmodel.h"

#include <QtCore/QRegExp>

SearchResultsModel::SearchResultsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void SearchResultsModel::setSearchResults(const QVariantList & searchResults)
{
    beginInsertRows(QModelIndex(), m_searchResults.size(), m_searchResults.size() + searchResults.size());
    m_searchResults = searchResults;
    endInsertRows();
}

int SearchResultsModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);

    return m_searchResults.size();
}

QVariant SearchResultsModel::data ( const QModelIndex & index, int role ) const
{
    if( role == Qt::DisplayRole) {
        QString title = m_searchResults.at(index.row()).toMap().value("title").toString();
        title.remove(QRegExp("<[^>]*>"));

        return title;
    }
    else if( role == Qt::UserRole) {
        return m_searchResults.at(index.row()).toMap().value("details");
    }

    return QVariant();
}
