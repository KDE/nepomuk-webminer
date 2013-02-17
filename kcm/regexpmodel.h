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

#ifndef REGEXPMODEL_H
#define REGEXPMODEL_H

#include <QtCore/QAbstractTableModel>

#include "resourceextractor/filenameanalyzer.h"

namespace NepomukWebMiner
{

/**
 * @brief Listmodel to hold all existing NepomukWebMiner::Extractor::RegExpData objects
 *
 * The Qt::DisplayRole will show the regular expression pattern
 */
class RegExpModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RegExpModel(QObject *parent = 0);
    
    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    void addRegExpData(const Extractor::RegExpData &data);
    void setRegExpData(const QList<Extractor::RegExpData> &data);
    void replaceRegExpData(int index, const Extractor::RegExpData &data);
    void removeData(int index);

    NepomukWebMiner::Extractor::RegExpData regExpData(int index) const;
    QList<NepomukWebMiner::Extractor::RegExpData> allRegExpData() const;

    void clear();

    /**
     * @brief Changes the order of the data
     *
     * @param index the data that will move one row up
     */
    void moveUp(int index);

    /**
     * @brief Changes the order of the data
     *
     * @param index the data that will move one row down
     */
    void moveDown(int index);

private:
    QList<Extractor::RegExpData> m_modelData;
};
}

#endif // REGEXPMODEL_H