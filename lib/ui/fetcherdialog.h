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

#include "nepomukmetadataextractor_export.h"
#include "ui_fetcherdialog.h"

#include <QtCore/QModelIndex>

namespace Ui {
    class FetcherDialog;
}

class QTextDocument;

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class ExtractorFactory;
    class WebExtractor;
    class ResourceExtractor;
    class MetaDataParameters;
}

namespace Dialog {
    class SearchResultsModel;

/**
  * @brief Main dialog to show the current progress and some buttons to interact with
  *
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT FetcherDialog : public QDialog, private Ui::FetcherDialog
{
    Q_OBJECT

public:
    explicit FetcherDialog(QWidget *parent = 0);
    ~FetcherDialog();

    void setInitialPathOrFile( const KUrl &url );
    void setForceUpdate(bool update);
    void setTvShowMode(bool tvshowmode);

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

    void saveMetaData();

    void cancelClose();

    void addToProgressLog(const QString &status);
    void showProgressLog();

private:
    void setupCurrentResourceToLookup();
    void fillEngineList(const QString &category);
    void showItemDetails();

    void busyFetching();
    void finishedFetching();

private:
    NepomukMetaDataExtractor::Extractor::ResourceExtractor *m_re;
    NepomukMetaDataExtractor::Extractor::ExtractorFactory *m_ef;
    NepomukMetaDataExtractor::Extractor::WebExtractor *m_webextractor;
    NepomukMetaDataExtractor::Extractor::MetaDataParameters *m_currentItemToupdate;

    int m_currentResource;
    bool tvshowmode;

    SearchResultsModel *m_resultModel;

    QTextDocument *m_progressLog;
};
}
}

#endif // FETCHERDIALOG_H
