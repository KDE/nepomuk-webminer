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

#ifndef MUSICPIPE_H
#define MUSICPIPE_H

#include "nepomukpipe.h"
#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Pipe {

/**
 * @brief Pipes a @c QVariantMap with <i>Music</i> information about a @c album and its @c tracks into nepomuk
 *
 * The QVariantMap should have this keys:
 *
 * @li @c title - Album title
 * @li @c musicbrainz - MusicBrainz id
 * @li @c trackcount - number of tracks on the album
 * @li @c cover - url for the cover image
 * @li @c genre - list of genres for the album split by ;
 * @li @c tracks - QVariantList containing more QVariantMaps with the track infos
 *
 * The QVariantMap containing the track information looks like this:
 *
 * @li @c title - name of the track (music piece)
 * @li @c tracknumber - number of the track in the album
 * @li @c performer - performer of the track
 * @li @c genre - list of genres for the track split by ;
 * @li @c musicbrainz - MusicBrainz id
 *
 * @todo TODO: incomplete, needs lyrics and other stuff
 */
class NEPOMUKMETADATAEXTRACTOR_EXPORT MusicPipe : public NepomukPipe
{
public:
    /**
     * @brief Standard Constructor
     * @param parent parent object
     */
    explicit MusicPipe(QObject *parent = 0);

    void pipeImport(const QVariantMap &movieEntry);
};
}
}

#endif // MUSICPIPE_H
