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

/**
  * @brief Handles the import of tvshows
  *
  * @todo support bulk import of a complete series with all its episodes
  */
class TvShowPipe : public NepomukPipe
{
public:
    TvShowPipe(QObject *parent = 0);
    virtual ~TvShowPipe();

    /**
      * Imports one episode and its sereis details
      */
    void pipeImport(MetaDataParameters* tvshowEntry);
};

#endif // TVSHOWPIPE_H
