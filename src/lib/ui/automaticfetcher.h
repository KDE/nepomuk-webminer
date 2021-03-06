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

#ifndef AUTOMATICFETCHER_H
#define AUTOMATICFETCHER_H

#include "fetcher.h"

#include <QtCore/QObject>
#include <KDE/KUrl>

#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Extractor
{
class ExtractorFactory;
class WebExtractor;
class ResourceExtractor;
class MetaDataParameters;
}

namespace UI
{
class AutomaticFetcherPrivate;

/**
 * @brief Automatic background fetcher for the meta data
 *
 * Unlike the @c FetcherDialog this one operates in the background without any user interaction / verification
 *
 * It always takes the first found search result and the first available search engine as the meta data for the resource
 *
 * Searchresults are prefiltered to avoid completely wrong results
 * @see levenshteinDistance
 *
 * Some examples how to use this class:
@code
QUrl someFolder = ...
NepomukWebMiner::UI::AutomaticFetcher af;

af.addFetcherPath( someFolder );
af.startUrlFetcher();
@endcode

@code
QList<Nepomuk2::Resource> resources = ...
NepomukWebMiner::UI::AutomaticFetcher af;

af.addFetcherResource( someFolder );
af.startUrlFetcher();
@endcode
 */
class NEPOMUKWEBMINER_EXPORT AutomaticFetcher : public QObject, public Fetcher
{
    Q_OBJECT
public:
    /**
     * @brief Standard Constructor
     *
     * @param parent some parent object
     */
    explicit AutomaticFetcher(QObject *parent = 0);
    ~AutomaticFetcher();

    /**
     * @brief adds the url where the item data can be found
     *
     * This will fetch and add meta data to the nepomuk storage without connecting it to a file
     * @param url the url that can be parsed by one of the plugins
     */
    void addFetcherUrl(const KUrl& url);

public Q_SLOTS:
    /**
     * @brief Starts the meta data fetching
     *
     * Iterates over searchNextItem() as long as unprocessed MetaDataParameter fields are available
     * For each file:
     * @li search for a list of possible matches
     * @li select a good entry in selectSearchEntry
     * @li download the meta data from the selected entry
     * @li pipe the returned QVariantMap to the NepomukPipe to create the correctresources in fetchedItemDetails() and
     */
    void startFetcher();

    /**
     * @brief Starts the fetcher in url fetch only mode
     *
     * This calls extractItem() directly and does not use the search function first, as the url
     * given is already the correct item to fetch.
     */
    void startUrlFetcher();

Q_SIGNALS:
    /**
     * @brief Emitted when all files are handled, no matter if metadata could be found or not
     */
    void finished();

private Q_SLOTS:
    /**
     * @brief Take the next available resoruce and start the meta data search or stop the fetcher here
     */
    void searchNextItem();

    /**
     * @brief called from the python plugins to give a list of availabe search matches
     *
     * @param searchResults lis tof all matching items
     *
     */
    void selectSearchEntry(QVariantList searchResults);

    /**
     * @brief Processes the returned otem data from the python plugins
     *
     * In the case of the tvshows additional chanegs are necessary, usually the resource-uri for the file is just added
     * at the toplevel of the QVariantMap as tvShows are a nested construct (Series/Season/Episode) the resource-uri must be added
     * to the Episode not the toplevel Series information
     *
     * @param resourceType what kind of resource type was returned
     * @param itemDetails a QVariantmap with all important meta data
     */
    void fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails);

    /**
     * @brief prints out kDebug messages for any error in the Python file
     *
     * @param error python error
     */
    void errorInScriptExecution(const QString &error);

    /**
     * @brief Outputs some logging information from the scripts int othe console
     *
     * @param msg the logging text
     */
    void log(const QString &msg);

private:
    Q_DECLARE_PRIVATE(AutomaticFetcher)
    AutomaticFetcherPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // AUTOMATICFETCHER_H
