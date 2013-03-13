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

#include "regexpmodel.h"

#include <KDE/KLocale>
#include <KDE/KDebug>

NepomukWebMiner::RegExpModel::RegExpModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int	NepomukWebMiner::RegExpModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);

    return m_modelData.size();
}

int	NepomukWebMiner::RegExpModel::columnCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);

    return 2;
}

QVariant NepomukWebMiner::RegExpModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    Q_UNUSED(orientation);
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal) {
        if(section == 0) {
            return i18n("Example");
        }
        if(section == 1) {
            return i18n("Regular Expression");
        }
        else {
            return QVariant();
        }
    }
    else {
        return QString::number(section+1);
    }
}

QVariant NepomukWebMiner::RegExpModel::data ( const QModelIndex & index, int role ) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_modelData.size() || index.row() < 0 || index.column() > 1 || index.column() < 0) {
        return QVariant();
    }

    if(index.column() == 1) {
        return m_modelData.at(index.row()).regExp.pattern();
    }
    else {
        return m_modelData.at(index.row()).example;
    }
}

void NepomukWebMiner::RegExpModel::addRegExpData(const Extractor::RegExpData &data)
{
    beginInsertRows(QModelIndex(), m_modelData.size(), m_modelData.size() + 1);
    m_modelData << data;
    endInsertRows();
}

void NepomukWebMiner::RegExpModel::setRegExpData(const QList<Extractor::RegExpData> &data)
{
    clear();

    beginInsertRows(QModelIndex(), m_modelData.size(), m_modelData.size() + data.size()-1);
    m_modelData << data;
    endInsertRows();
}

void NepomukWebMiner::RegExpModel::replaceRegExpData(int index, const Extractor::RegExpData &data)
{
    removeData(index);

    beginInsertRows(QModelIndex(), index, index);
    m_modelData.insert(index, data);
    endInsertRows();
}

NepomukWebMiner::Extractor::RegExpData NepomukWebMiner::RegExpModel::regExpData(int index) const
{
    if(index >= 0 && index < m_modelData.size()) {
        return m_modelData.at(index);
    }
    else {
        return Extractor::RegExpData();
    }
}

QList<NepomukWebMiner::Extractor::RegExpData> NepomukWebMiner::RegExpModel::allRegExpData() const
{
    return m_modelData;
}

void NepomukWebMiner::RegExpModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_modelData.size()-1 );
    m_modelData.clear();
    endRemoveRows();
}

void NepomukWebMiner::RegExpModel::removeData(int index)
{
    if(index >= 0 && index < m_modelData.size()) {
        beginRemoveRows(QModelIndex(), index, index );
        m_modelData.removeAt(index);
        endRemoveRows();
    }
}

void NepomukWebMiner::RegExpModel::moveUp(int index)
{
    if(index > 0 && index < m_modelData.size()) {
        Extractor::RegExpData tmp;

        // remove
        beginRemoveRows(QModelIndex(), index, index );
        tmp = m_modelData.takeAt(index);
        endRemoveRows();

        // and insert before again
        beginInsertRows(QModelIndex(), index-1, index-1);
        m_modelData.insert(index-1, tmp);
        endInsertRows();
    }
}

void NepomukWebMiner::RegExpModel::moveDown(int index)
{
    if(index >= 0 && index < m_modelData.size() - 1) {
        Extractor::RegExpData tmp;

        // remove
        beginRemoveRows(QModelIndex(), index, index );
        tmp = m_modelData.takeAt(index);
        endRemoveRows();

        // and insert before again
        beginInsertRows(QModelIndex(), index+1, index+1);
        m_modelData.insert(index+1, tmp);
        endInsertRows();
    }
}
