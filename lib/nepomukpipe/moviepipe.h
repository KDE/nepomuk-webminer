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

#ifndef MOVIEPIPE_H
#define MOVIEPIPE_H

#include "nepomukpipe.h"
#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Pipe
{

class MoviePipePrivate;

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
 * The @c poster won't be downloaded and added if it disabled in the metadata KConfig.
 * The location of the poster can be configured as well, either next to the movie file or in the data folder of the library
  */
class NEPOMUKWEBMINER_EXPORT MoviePipe : public NepomukPipe
{

public:
    /**
     * @brief Standard Constructor
     *
     * @param parent parent object
     */
    explicit MoviePipe(QObject *parent = 0);

    bool import(const QVariantMap &movieEntry);

private:
    Q_DECLARE_PRIVATE(MoviePipe)
    MoviePipePrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // MOVIEPIPE_H
