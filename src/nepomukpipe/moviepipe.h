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

#include "sro/nco/personcontact.h"

class MoviePipe : public NepomukPipe
{
public:
    MoviePipe(QObject *parent = 0);
    virtual ~MoviePipe();

    /**
      * Does the piping action
      */
    void pipeImport(MetaDataParameters* movieEntry);

private:
    Nepomuk::NCO::PersonContact createPerson(const QString &fullName);
};

#endif // MOVIEPIPE_H
