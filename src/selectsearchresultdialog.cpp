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

#include "selectsearchresultdialog.h"
#include "ui_selectsearchresultdialog.h"

#include "searchresultsmodel.h"
#include "searchresultdelegate.h"

SelectSearchResultDialog::SelectSearchResultDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectSearchResultDialog)
{
    ui->setupUi(this);

    m_resultModel = new SearchResultsModel(this);
    ui->listView->setModel( m_resultModel );

    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView->setItemDelegate(new SearchResultDelegate);
}

SelectSearchResultDialog::~SelectSearchResultDialog()
{
    delete m_resultModel;
    delete ui;
}

void SelectSearchResultDialog::setMetaDataParameters(const MetaDataParameters & mdp)
{
    ui->labelFileName->setText(mdp.resourceUri.fileName());
    ui->labelTitle->setText(mdp.searchTitle);
}

void SelectSearchResultDialog::setSearchResults(const QVariantList &searchResults)
{
    m_resultModel->setSearchResults( searchResults );
}

int SelectSearchResultDialog::selectedEntry()
{
    return ui->listView->currentIndex().row();
}
