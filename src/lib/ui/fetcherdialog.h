/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#ifndef FETCHERDIALOG_H
#define FETCHERDIALOG_H

#include "fetcher.h"

#include <QtGui/QDialog>
#include <QtCore/QModelIndex>

#include <KDE/KUrl>
#include "nepomukwebminer_export.h"


// for the fetcherdialog.ui file
#include "ui_fetcherdialog.h"
namespace Ui
{
class FetcherDialog;
}

namespace NepomukWebMiner
{
namespace UI
{
class FetcherDialogPrivate;

/**
  * @brief Dialog to interactively @c search / @c fetch and @c save metadata for a list of resources
  *
  * Some examples how to use this class:
@code
QUrl someFolder = ...
NepomukWebMiner::UI::FetcherDialog fd;

fd.setTvShowMode( true );
fd.setTvShowNamesInFolders( true );
fd.addFetcherPath( someFolder );
fd.exec();
@endcode

@code
QList<Nepomuk2::Resource> resources = ...
NepomukWebMiner::UI::FetcherDialog fd;

fd.setForceUpdate( true );
fd.addFetcherResource( someFolder );
fd.exec();
@endcode
  */
class NEPOMUKWEBMINER_EXPORT FetcherDialog : public QDialog, public Fetcher, private Ui::FetcherDialog
{
    Q_OBJECT

public:
    explicit FetcherDialog(QWidget *parent = 0);
    ~FetcherDialog();

    bool startFetching(const KUrl url);

private Q_SLOTS:
    /**
     * @brief Starts the KHelpCenter with the metadata extarctor docbook
     */
    void openHelp();

    /**
     * @brief Called from the ResourceExtractor when all files/resources are parsed and the fetching can begin
     */
    void resourceFetchingDone();

    /**
     * @brief Takes the next MetaDataParameters in the list after the user clicked on the next button
     *
     * @see setupCurrentResourceToLookup
     */
    void selectNextResourceToLookUp();

    /**
     * @brief Takes the previous MetaDataParameters in the list after the user clicked on the previous button
     *
     * @see setupCurrentResourceToLookup
     */
    void selectPreviousResourceToLookUp();

    /**
     * @brief When the tvshow/movie combobox changes its selection
     * @param selection new selection
     */
    void resourceTypeSelectionChanged(int selection);

    /**
     * @brief takes the current search parameter and the currently selected WebExtractor plugin and search for items that fit
     */
    void startSearch();

    /**
     * @brief Called from the plugins with the list of all found search results
     *
     * @param searchResults list of entries that fit the search parameters
     */
    void searchResultList(QVariantList searchResultList);

    /**
     * @brief Called when the user clicks on a search entry in the list and display its details
     *
     * @param current current selected entry
     * @param previous last selected entry
     */
    void searchEntrySelected(const QModelIndex &current, const QModelIndex &previous);

    /**
     * @brief Open a dialog to change the search parameters in more detail
     *
     * Not all search parameters are shown in the main ui, here it is possible to add/change more parameters
     * to ge tbetter results
     */
    void showSearchParameters();

    /**
     * @brief Opens the url with additional details of the item in the default webbrowser
     *
     * @param url web url with more details
     */
    void openDetailsLink(const QString &url);

    /**
     * @brief Starts the metadata fetching of the current selected WebExtractor
     *
     * @see fetchedItemDetails
     */
    void fetchMoreDetails();

    /**
     * @brief Fetch more details and automatically save them afterwards
     *
     * @see fetchMoreDetails
     * @see fetchedItemDetails
     * @see saveMetaDataSlot
     */
    void fetchMoreAndSave();

    /**
     * @brief Called by the WebExtractor to add the fetched @c metadata to the current file
     *
     * @param resourceType the resourcetype as retrieved from the plugin
     * @param itemDetails the map with all metadata
     */
    void fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails);

    /**
     * @brief Saves the current fetched metadata for the current shown item
     */
    void saveMetaDataSlot();

    /**
     * @brief Close the dialog and cancel the metadata fetching
     */
    void cancelClose();

    /**
     * @brief Opens a Dialog to show a critical erro in one of the plugins
     *
     * @param error the error message
     */
    void errorInScriptExecution(const QString &error);

    /**
     * @brief Adds the <i>status message</i> to the progress log
     *
     * @param status the message that will be added
     */
    void addToProgressLog(const QString &status);

    /**
     * @brief Opens a dialog to show some additional information about the fetcher progress
     */
    void showProgressLog();

    /**
     * @brief Opens the Settings dialog to change the KConfig values for the extractor
     */
    void openSettings();

private:
    /**
     * @brief Takes the current selected MetaDataParameters object and enables/disables some gui parts depending on the resourcetype
     *
     * The list of available search plugins will also be changed accordingly
     * Calls showItemDetails afterwards
     */
    void setupCurrentResourceToLookup();

    /**
     * @brief Fills the KComboBox with the list of available WebExtractors for the given category
     * @param category selected category @c publication, @c movie, @c tvshow, @c music
     */
    void fillEngineList(const QString &category);

    /**
     * @brief Displays some item details in the MetaDataWidget
     */
    void showItemDetails();

    /**
     * @brief Disabled various buttons and adds a busy mouse cursor
     */
    void busyFetching();

    /**
     * @brief Enables the buttons again and shows the normal mouse cursor
     */
    void finishedFetching();

private:
    Q_DECLARE_PRIVATE(FetcherDialog)
    FetcherDialogPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // FETCHERDIALOG_H
