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

#include <QtGui/QDialog>
#include <KDE/KUrl>

#include "metadataextractor_export.h"

#include <QtCore/QModelIndex>

namespace Ui {
    class FetcherDialog;
}

class MetaDataFetcher;
class MetaDataParameters;
class SearchResultsModel;
class QTextDocument;

/**
  * @brief Main dialog to show the current progress and some buttons to interact with
  *
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT FetcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FetcherDialog(QWidget *parent = 0);
    ~FetcherDialog();

    void setInitialPathOrFile( const KUrl &url );
    void setForceUpdate(bool update);

private slots:
    void fileFetchingDone();
    void selectNextResourceToLookUp();
    void selectPreviousResourceToLookUp();

    void startSearch();
    void selectSearchEntry( MetaDataParameters *mdp, QVariantList searchResults);
    void searchEntrySelected(const QModelIndex &current, const QModelIndex &previous);

    void showSearchParameters();
    void openDetailsLink(const QString &url);

    void fetchMoreDetails();
    void fetchedItemDetails(MetaDataParameters *mdp, QVariantMap itemDetails);

    void saveMetaData();

    void cancelClose();

    void addProgressInfo(const QString &status);
    void showProgressLog();

private:
    void fillEngineList(const QString &category);
    void showItemDetails();

    void busyFetching();
    void finishedFetching();

private:
    Ui::FetcherDialog *ui;
    MetaDataFetcher *m_mdf;

    QStringList m_categoriesToFetch;
    int m_currentCategory;
    int m_currentResource;

    SearchResultsModel *m_resultModel;

    QTextDocument *m_progressLog;
};

#endif // FETCHERDIALOG_H
