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

#include "searchresultsmodel.h"

#include <QtCore/QRegExp>


namespace NepomukMetaDataExtractor
{
namespace UI
{
class SearchResultsModelPrivate
{
public:
    QVariantList searchResults; /**< Holds the list of all search results from the python plugin */
};
}
}

NepomukMetaDataExtractor::UI::SearchResultsModel::SearchResultsModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new NepomukMetaDataExtractor::UI::SearchResultsModelPrivate)
{
}

void NepomukMetaDataExtractor::UI::SearchResultsModel::setSearchResults(const QVariantList & searchResults)
{
    Q_D(SearchResultsModel);
    beginInsertRows(QModelIndex(), d->searchResults.size(), d->searchResults.size() + searchResults.size());
    d->searchResults = searchResults;
    endInsertRows();
}

QVariantMap NepomukMetaDataExtractor::UI::SearchResultsModel::searchResultEntry(const QModelIndex & index) const
{
    if (index.row() >= 0 && index.row() < d_ptr->searchResults.size()) {
        return d_ptr->searchResults.at(index.row()).toMap();
    } else {
        return QVariantMap();
    }
}

void NepomukMetaDataExtractor::UI::SearchResultsModel::clear()
{
    Q_D(SearchResultsModel);
    beginRemoveRows(QModelIndex(), 0, d->searchResults.size());
    d->searchResults.clear();
    endRemoveRows();
}

int NepomukMetaDataExtractor::UI::SearchResultsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);

    return d_ptr->searchResults.size();
}

QVariant NepomukMetaDataExtractor::UI::SearchResultsModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole) {
        QString title = d_ptr->searchResults.at(index.row()).toMap().value("title").toString();
        title.remove(QRegExp("<[^>]*>")); // sometimes titles from the search have html tags attached, which will be removed here

        return title;
    } else if (role == Qt::UserRole) {
        return d_ptr->searchResults.at(index.row()).toMap().value("details");
    }

    return QVariant();
}
