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

#ifndef TVSHOWPIPE_H
#define TVSHOWPIPE_H

#include "nepomukpipe.h"

#include "nmm/tvseason.h"
#include "nmm/tvshow.h"

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Pipe {
/**
  * @brief Pipes a @c QVariantMap with <i>TV Shows</i> information into @c Nempomuk
  *
  * The QVariantMap with the information contains several other QVariantMaps.
  * For the purpose of single episodes this seem overly complicated, but this helps to simply
  * bulk import complete seasons or even series in one go without quering the web resource all
  * over again.
  *
  * This takes care of the special case that tv shows always come in large series that are all connected
  * to each other.
  *
  * This Pipe class generates the correct Nepomuk structure, splits person names and add them as subresource,
  * defines the subresource relationship between Show/Series/Episode and downloads the posters/banners from
  * the internet to a user defined folder.
  *
  * The QVariantMap should have this keys:
  *
  * @li @c title
  * @li @c overview
  * @li @c imdbid - optional imdb id
  * @li @c banner
  * @li @c poster
  * @li @c seasons - QVariantList containing more QVariantMaps with series info
  *
  * The QVariantMap containing the season information looks like this:
  *
  * @li @c number
  * @li @c banner
  * @li @c poster
  * @li @c episodes - QVariantList containing more QVariantMaps with episodes info
  *
  * The QVariantmap containing the episode information looks like this:
  * @li @c resourceuri - the file url or nepomuk uri of the resource this data should be added to
  * @li @c title
  * @li @c overview
  * @li @c number
  * @li @c banner
  * @li @c poster
  * @li @c firstaired - use yyyy or yyy-mm-dd
  * @li @c genres - split by ;
  * @li @c director - split by ;
  * @li @c writer - split by ;
  * @li @c actors - split by ;
  * @li @c seealso - url where we got the metadata from
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT TvShowPipe : public NepomukPipe
{
public:
    TvShowPipe(QObject *parent = 0);

    void pipeImport(const QVariantMap &tvshowEntry);

private:
    /**
      * @brief Helper function to generate the basic episode details
      *
      * @param episodeInfo the episode detals as retrieved from the internet
      * @param season the created season SimpleResource
      */
    Nepomuk2::NMM::TVShow createEpisode(const QVariantMap &episodeInfo, const Nepomuk2::NMM::TVSeason &season);
};
}
}

#endif // TVSHOWPIPE_H
