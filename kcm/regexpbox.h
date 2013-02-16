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

#ifndef REGEXPBOX_H
#define REGEXPBOX_H

#include <QtGui/QWidget>

#include "resourceextractor/filenameanalyzer.h"

namespace Ui {
class RegExpBox;
}

namespace NepomukWebMiner
{
class RegExpModel;

/**
 * @brief Widget to show and manipulate the list of RegExpData objects for one filetype.
 *
 * Displays via the RegExpModel the list of regular expressions and allows to change the order,
 * as well as adding/removing/editing them.
 */
class RegExpBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit RegExpBox(QWidget *parent = 0);
    ~RegExpBox();

    void setRegExpData(const QList<NepomukWebMiner::Extractor::RegExpData> &data);
    QList<NepomukWebMiner::Extractor::RegExpData> regExpData() const;
    void clear();

signals:
    void configChanged();

private slots:
    void addRegExp();
    void editCurrent();
    void removeCurrent();
    void moveCurrentUp();
    void moveCurrentDown();
    
private:
    Ui::RegExpBox *ui;
    RegExpModel *m_model;
};

}
#endif // REGEXPBOX_H
