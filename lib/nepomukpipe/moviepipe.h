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

#ifndef MOVIEPIPE_H
#define MOVIEPIPE_H

#include "nepomukpipe.h"
#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Pipe {

/**
  * @brief Pipes a @c QVariantMap with movie information into @c Nempomuk
  *
  * Creates the correct Movie structure, splits director/writer/cast names correctly and adds them as subresource to the movie
  *
  * The following keys will be used in this transition:
  * @li @c resourceuri - the file url or nepomuk uri of the resource this data should be added to
  * @li @c title - name of the movie
  * @li @c plot - some longer description
  * @li @c year - release year
  * @li @c genre - split by ;
  * @li @c director - split by ;
  * @li @c writer - split by ;
  * @li @c cast - split by ;
  * @li @c poster - url where the poster can be found
  * @li @c seealso - url where we got the metadata from
  *
 * The @c poster won't be downloaded and added if it disabeld in the metadata KConfig.
 * The location of the poster can be configured as well, either next to the movie file or in the data fodler of the library
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT MoviePipe : public NepomukPipe
{

public:
    /**
     * @brief Standard Constructor
     *
     * @param parent parent object
     */
    MoviePipe(QObject *parent = 0);

    void pipeImport(const QVariantMap &movieEntry);
};
}
}

#endif // MOVIEPIPE_H
