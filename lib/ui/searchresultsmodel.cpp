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

NepomukMetaDataExtractor::Dialog::SearchResultsModel::SearchResultsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void NepomukMetaDataExtractor::Dialog::SearchResultsModel::setSearchResults(const QVariantList & searchResults)
{
    beginInsertRows(QModelIndex(), m_searchResults.size(), m_searchResults.size() + searchResults.size());
    m_searchResults = searchResults;
    endInsertRows();
}

QVariantMap NepomukMetaDataExtractor::Dialog::SearchResultsModel::searchResultEntry( const QModelIndex & index )
{
    if( index.row() >= 0 && index.row() < m_searchResults.size()) {
        return m_searchResults.at(index.row()).toMap();
    }
    else {
        return QVariantMap();
    }
}

void NepomukMetaDataExtractor::Dialog::SearchResultsModel::clear( )
{
    beginRemoveRows(QModelIndex(), 0, m_searchResults.size());
    m_searchResults.clear();
    endRemoveRows();
}

int NepomukMetaDataExtractor::Dialog::SearchResultsModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);

    return m_searchResults.size();
}

QVariant NepomukMetaDataExtractor::Dialog::SearchResultsModel::data ( const QModelIndex & index, int role ) const
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
