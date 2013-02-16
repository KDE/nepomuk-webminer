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

#ifndef REGEXPDIALOG_H
#define REGEXPDIALOG_H

#include <QtGui/QDialog>

#include "resourceextractor/filenameanalyzer.h"

class QListWidgetItem;

namespace Ui {
class RegExpDialog;
}

namespace NepomukWebMiner
{

/**
 * @brief A dialog to add/edit any RegExpData object
 *
 * Allows to change the regular expression, if the full path or just the filename will be used
 * and what capturing group of the regular expression represent which data string.
 *
 * Also checks that the regular expression and the capturing group selection is correct.
 */
class RegExpDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RegExpDialog(QWidget *parent = 0);
    ~RegExpDialog();
    
    void setRegExpData(const Extractor::RegExpData &data);
    NepomukWebMiner::Extractor::RegExpData regExpData() const;

public slots:
    void accept();

private slots:
    void updateMatchExample();
    void showHelp();

private:
    void setupUi();
    Ui::RegExpDialog *ui;

    QListWidgetItem *title;
    QListWidgetItem *show;
    QListWidgetItem *season;
    QListWidgetItem *episode;
    QListWidgetItem *person;
    QListWidgetItem *album;
    QListWidgetItem *year;
    QListWidgetItem *track;
};
}
#endif // REGEXPDIALOG_H
