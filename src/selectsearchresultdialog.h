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

#ifndef SELECTSEARCHRESULTDIALOG_H
#define SELECTSEARCHRESULTDIALOG_H

#include <QtGui/QDialog>

#include "metadataparameters.h"

namespace Ui {
    class SelectSearchResultDialog;
}

class SearchResultsModel;

/**
  * @brief A dialog tha presnts all found search results with the @c title and some @c details
  *
  * The user can select the entry he wish to use or hit cancel and the current file will be skipped
  */
class SelectSearchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSearchResultDialog(QWidget *parent = 0);
    ~SelectSearchResultDialog();

    void setMetaDataParameters(const MetaDataParameters & mdp);
    void setSearchResults(const QVariantList &searchResults);

    int selectedEntry();

private:
    Ui::SelectSearchResultDialog *ui;
    SearchResultsModel *m_resultModel;
};

#endif // SELECTSEARCHRESULTDIALOG_H