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

#ifndef AUTOMATICFETCHER_H
#define AUTOMATICFETCHER_H

#include <QObject>
#include <KDE/KUrl>

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class ExtractorFactory;
    class WebExtractor;
    class ResourceExtractor;
    class MetaDataParameters;
}

namespace UI {

/**
 * @brief Automatic background fetcher for the meta data
 *
 * Unlike the @c FetcherDialog this one operates in the background without any user interaction / verification
 *
 * It always takes the first found search result and the first available search engine as the meta data for the resource
 *
 * @todo TODO: implement some logic to select the best possible search result not just the first found
 * @todo TODO: implement KConfig to specify the the preferred search engine for a resourcetype rather than useing the first one
 * @todo TODO: implement multiple searches, if the first fails, try a different engine instead of failing
 */
class NEPOMUKMETADATAEXTRACTOR_EXPORT AutomaticFetcher : public QObject
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
     * @brief Sets the folder path or file name of the resources which will be processed
     *
     * @param url folder path or file name/path
     */
    void setInitialPathOrFile( const KUrl &url );

    /**
     * @brief Forces the fetcher to include resources which already have meta data
     *
     * This is false by default and will skip any files that already have meta data attached
     * Will just do a type check (if a video has NMM:TvShow or NMM:Movie it is assumed there is meta data available, as it would be
     * NFO:Video otherwise
     *
     * @param update @arg true refetch metadata also for existing data
     *               @arg false skip files with existing meta data
     */
    void setForceUpdate(bool update);

    /**
     * @brief Adds a hint to the VideoExtractor filename parser to improve name detection
     *
     * @param tvshowMode @arg true video files are all tvShows not movies
     *                   @arg false video files are movies and/or tvshows (default)
     */
    void setTvShowMode(bool tvshowMode);

    /**
     * @brief Adds a hint to the VideoExtractor filename parser to improve name detection
     *
     * @param useFolderNames @arg true check folder names for tvshows too
     *                   @arg false only check filename for tvshows (default)
     */
    void setTvShowNamesInFolders(bool useFolderNames);

    /**
     * @brief Adds a hint to the VideoExtractor filename parser to improve name detection
     *
     * @param tvshowMode @arg true video files are all movies not tvshows
     *                   @arg false video files are movies and/or tvshows (default)
     */
    void setMovieMode(bool movieMode);

public slots:
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

    void startUrlFetcher(const QUrl &itemUrl);

private slots:
    /**
     * @brief Take the next available resoruce and start the meta data search or stop the fetcher here
     */
    void searchNextItem();

    /**
     * @brief called from the python plugins to give a list of availabe search matches
     *
     * @param searchResults lis tof all matching items
     *
     * @todo TODO: don'#'t just use first search result, add additional checks to avoid adding wrong data
     */
    void selectSearchEntry( QVariantList searchResults );

    /**
     * @brief Processes the returned otem data from the python plugins
     *
     * In the case of the tvshows additional chanegs are necessary, usually the resource-uri for the file is just added
     * at the toplevel of the QVariantMap as tvShows are a nested construct (Series/Season/Episode) the resource-uri must be added
     * to the Episode not the toplevel Series information
     *
     * @param resourceType what kind of resource type was returned
     * @param itemDetails a QVariantmap with all important meta data
     *
     */
    void fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails);

    /**
     * @brief calles the correct NepomukPipe for the current resourceType to import the data into Nepomuk
     */
    void saveMetaData();

    /**
     * @brief prints out kDebug messages for any error in the Python file
     * @param error python error
     */
    void errorInScriptExecution(const QString &error);

private:
    NepomukMetaDataExtractor::Extractor::ResourceExtractor *m_re;
    NepomukMetaDataExtractor::Extractor::ExtractorFactory *m_ef;
    NepomukMetaDataExtractor::Extractor::WebExtractor *m_webextractor;
    NepomukMetaDataExtractor::Extractor::MetaDataParameters *m_currentItemToupdate;
};
}
}

#endif // AUTOMATICFETCHER_H
