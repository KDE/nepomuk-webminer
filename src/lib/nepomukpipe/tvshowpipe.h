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

#ifndef TVSHOWPIPE_H
#define TVSHOWPIPE_H

#include "nepomukpipe.h"

#include "nmm/tvseason.h"
#include "nmm/tvshow.h"

#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Pipe
{

class TvShowPipePrivate;

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
  * @li @c title - the name of the show
  * @li @c overview - longer description about the show
  * @li @c imdbid - optional imdb id
  * @li @c banner - url of the banner location
  * @li @c poster - url of the poster location
  * @li @c seasons - QVariantList containing more QVariantMaps with the @c season info
  *
  * The QVariantMap containing the season information looks like this:
  *
  * @li @c number - the season number
  * @li @c banner - url of the banner location
  * @li @c poster - url of the poster location
  * @li @c episodes - QVariantList containing more QVariantMaps with @c episodes info
  *
  * The QVariantmap containing the episode information looks like this:
  * @li @c resourceuri - the file url or nepomuk uri of the resource this data should be added to
  * @li @c title - the title of this episode
  * @li @c overview - longer description of the episode
  * @li @c number - the episode number
  * @li @c banner - url of the banner location
  * @li @c poster - url of the poster location
  * @li @c firstaired - use yyyy or yyy-mm-dd
  * @li @c genres - split by ;
  * @li @c director - split by ;
  * @li @c writer - split by ;
  * @li @c actors - split by ;
  * @li @c seealso - url where we got the metadata from
  *
  *
 * The @c poster and @c banner won't be downloaded and added if it disabeld in the metadata KConfig.
 * The location of the @c poster or @c banner can be configured as well, either next to the video file or in the data fodler of the library
  */
class NEPOMUKWEBMINER_EXPORT TvShowPipe : public NepomukPipe
{
public:
    /**
     * @brief default constructor
     *
     * @param parent some parent object
     */
    explicit TvShowPipe(QObject *parent = 0);

    /**
     * @brief Starts the pipe importer for the tvshow metadata
     *
     * @param tvshowEntry metadata map
     */
    bool import(const QVariantMap &tvshowEntry);

private:
    /**
      * @brief Helper function to generate the basic episode details
      *
      * @param episodeInfo the episode detals as retrieved from the internet
      * @param season the created season SimpleResource where the episode is added to
      */
    Nepomuk2::NMM::TVShow createEpisode(const QVariantMap &episodeInfo, const Nepomuk2::NMM::TVSeason &season);

    Q_DECLARE_PRIVATE(TvShowPipe)
    TvShowPipePrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // TVSHOWPIPE_H
