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

#ifndef FETCHER_H
#define FETCHER_H

#include <Nepomuk2/Resource>

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class ExtractorFactory;
    class ResourceExtractor;
    class MetaDataParameters;
}

namespace UI {
    class FetcherPrivate;


class NEPOMUKMETADATAEXTRACTOR_EXPORT Fetcher
{
public:
    explicit Fetcher();
    virtual ~Fetcher();

    /**
     * @brief Sets the folder path or file name of the resources which will be processed
     *
     * @param url folder path or file name/path
     */
    void addFetcherPath( const KUrl &url );
    void addFetcherResource( const KUrl &url );
    void addFetcherResource(const Nepomuk2::Resource &resource);
    void addFetcherResource(const QList<Nepomuk2::Resource> &resources);

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

protected:
    NepomukMetaDataExtractor::Extractor::ResourceExtractor *resourceExtractor();
    NepomukMetaDataExtractor::Extractor::ExtractorFactory  *extractorFactory();

    void addResourceUriToMetaData( NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp );
    void saveMetaData(const NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp) const;

private:
    Q_DECLARE_PRIVATE(Fetcher)
    FetcherPrivate *const d_ptr; /**< d-pointer for this class */
};

}
}

#endif // FETCHER_H
