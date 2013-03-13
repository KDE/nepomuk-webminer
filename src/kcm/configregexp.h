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

#ifndef CONFIGREGEXP_H
#define CONFIGREGEXP_H

#include <QtGui/QWidget>

namespace Ui {
class ConfigRegExp;
}

namespace NepomukWebMiner
{

/**
 * @brief Lists all configuration for the filename analyzing.
 *
 * Here the user can change the list of strings that will be removed from any filename and
 * all the regular expressions that will try to extract additional data from a filename for
 * movies, tvshows, documents or music files.
 */
class ConfigRegExp : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfigRegExp(QWidget *parent = 0);
    ~ConfigRegExp();

signals:
    void configChanged();

public slots:
    void saveConfig();
    void loadConfig();

private:
    void setupUi();

    Ui::ConfigRegExp *ui;
};
}

#endif // CONFIGREGEXP_H
