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

namespace Ui {
    class FetcherDialog;
}

class MetaDataFetcher;
class MetaDataParameters;

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
    void setBulkDownload(bool bulk);

private slots:
    void setProgressInfo(const QString &status);
    void setProgress(int current, int max);
    void fileFetchingDone();
    void searchlookUpFinished();
    void selectSearchEntry( MetaDataParameters *mdp, QVariantList searchResults);

    void selectEngine();
    void startSearch();
    void cancelClose();
    void showNextFileTypeInfoString();

private:
    Ui::FetcherDialog *ui;

    MetaDataFetcher *m_mdf;
    QStringList m_fileTypesToFetch;
    bool m_bulkDownload;
};

#endif // FETCHERDIALOG_H
