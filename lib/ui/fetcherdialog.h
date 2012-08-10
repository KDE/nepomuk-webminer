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

#ifndef FETCHERDIALOG_H
#define FETCHERDIALOG_H

#include "fetcher.h"

#include <QtGui/QDialog>
#include <QtCore/QModelIndex>

#include <KDE/KUrl>
#include "nepomukmetadataextractor_export.h"


// for the fetcherdialog.ui file
#include "ui_fetcherdialog.h"
namespace Ui {
    class FetcherDialog;
}

namespace NepomukMetaDataExtractor {
namespace UI {
    class FetcherDialogPrivate;

/**
  * @brief Main dialog to show the current progress and some buttons to interact with
  *
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT FetcherDialog : public QDialog, public Fetcher, private Ui::FetcherDialog
{
    Q_OBJECT

public:
    explicit FetcherDialog(QWidget *parent = 0);
    ~FetcherDialog();

private slots:
    void resourceFetchingDone();
    void selectNextResourceToLookUp();
    void selectPreviousResourceToLookUp();
    void resourceTypeSelectionChanged(int selection);

    void startSearch();
    void selectSearchEntry( QVariantList searchResults );
    void searchEntrySelected(const QModelIndex &current, const QModelIndex &previous);

    void showSearchParameters();
    void openDetailsLink(const QString &url);

    void fetchMoreDetails();
    void fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails);

    void saveMetaDataSlot();

    void cancelClose();

    void errorInScriptExecution(const QString &error);
    void addToProgressLog(const QString &status);
    void showProgressLog();

private:
    void setupCurrentResourceToLookup();
    void fillEngineList(const QString &category);
    void showItemDetails();

    void busyFetching();
    void finishedFetching();

private:
    Q_DECLARE_PRIVATE(FetcherDialog)
    FetcherDialogPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // FETCHERDIALOG_H
