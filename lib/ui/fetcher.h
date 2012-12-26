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

#ifndef FETCHER_H
#define FETCHER_H

#include <Nepomuk2/Resource>

#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Extractor
{
class ExtractorFactory;
class ResourceExtractor;
class MetaDataParameters;
}

namespace UI
{
class FetcherPrivate;

/**
 * @brief Base class for all fetcher implementations.
 *
 * Defines some useful functions every fetcher needs.
 * Calls the resource extractor to get additional information.
 * Keeps a list of all items that have to be checked.
 */
class NEPOMUKWEBMINER_EXPORT Fetcher
{
public:
    explicit Fetcher();
    virtual ~Fetcher();

    /**
     * @brief Sets the folder path or file name of the resources which will be processed
     *
     * @param url folder path or file name/path
     */
    void addFetcherPath(const KUrl &url);

    /**
     * @brief Adds a Nepomuk Resource to the list of items that will be checked
     *
     * @param url nepomuk resource uri
     */
    void addFetcherResource(const KUrl &url);

    /**
     * @brief Adds a Nepomuk Resource to the list of items that will be checked
     *
     * @param resource nepomuk resource
     */
    void addFetcherResource(const Nepomuk2::Resource &resource);

    /**
     * @brief Adds a list Nepomuk Resources to the list of items that will be checked
     *
     * @param resources nepomuk resource list
     */
    void addFetcherResource(const QList<Nepomuk2::Resource> &resources);

    /**
     * @brief Forces the fetcher to include resources which already have meta data
     *
     * This is false by default and will skip any files that already have meta data attached
     * Will just do a type check (if a video has @c NMM:TvShow or @c NMM:Movie it is assumed there is meta data available, as it would be
     * @c NFO:Video otherwise
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
     *                       @arg false only check filename for tvshows (default)
     */
    void setTvShowNamesInFolders(bool useFolderNames);

    /**
     * @brief Adds a hint to the VideoExtractor filename parser to improve name detection
     *
     * @param tvshowMode @arg true video files are all movies not tvshows
     *                   @arg false video files are movies and/or tvshows (default)
     */
    void setMovieMode(bool movieMode);

    /**
     * @brief Returns the ResourceExtractor that keeps a list of all resource which need to be checked
     *
     * @return the ResourceExtractor
     */
    NepomukWebMiner::Extractor::ResourceExtractor *resourceExtractor();

    /**
     * @brief Returns the ExtractorFactory that handles all available plugins which can be used to fetch metadata from the web
     *
     * @return the ExtractorFactory
     */
    NepomukWebMiner::Extractor::ExtractorFactory  *extractorFactory();

    void updateIndexingLevel( const QUrl& uri, int level );

protected:
    /**
     * @brief Little helper function for the correct resourceuri handling of tvshows
     *
     * This is necessary for the TvShow pipe, because the resourceuri of the connected file is extected to be in the episode
     * layer of the nested QVariantMap rather than the toplevel.
     *
     * This allows to use the tvshow pipe with a QVariantMap that adds many episodes conected to many different files at once
     * @param mdp the MetaDataParameters which should be updated
     */
    void addResourceUriToMetaData(NepomukWebMiner::Extractor::MetaDataParameters *mdp);

    /**
     * @brief Calls the correct NepomukPipe to save the QVariantMap into nepomuk
     *
     * @param mdp the MetaDataParameters which should be saved
     */
    void saveMetaData(Extractor::MetaDataParameters *mdp) const;

    /**
     * @brief Sets a distance between the returned result and the actual search
     *
     * Used to sort/filter the returned search result and help to get a better
     * automatic detection.
     *
     * Items with a distance greater than @p skipValue will be skipped in the returned list
     *
     * @param searchResults actual list of returned search results
     * @param currentItem current item including current search parameters
     * @param skipValue items with a distance greater than the skipvalue will be removed from the returned list
     *
     * @return sorted searchResults, with best result first
     */
    QVariantList setLevenshteinDistance(const QVariantList &searchResults, NepomukWebMiner::Extractor::MetaDataParameters *currentItem, uint skipValue);

    /**
     * @brief calculates the levenshteinDistance
     *
     * @param s1
     * @param s2
     *
     * @see http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance
     *
     * @return distance between s1 and s2
     */
    uint levenshteinDistance(const QString &s1, const QString & s2);

private:
    Q_DECLARE_PRIVATE(Fetcher)
    FetcherPrivate *const d_ptr; /**< d-pointer for this class */
};

}
}

#endif // FETCHER_H
