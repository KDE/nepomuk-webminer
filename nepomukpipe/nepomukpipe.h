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

#ifndef NEPOMUKPIPE_H
#define NEPOMUKPIPE_H

#include <QObject>

//class MetaDataParameters;
#include "../publicationentry.h"

class NepomukPipe : public QObject
{
    Q_OBJECT
public:
    explicit NepomukPipe(QObject *parent = 0);
    virtual ~NepomukPipe();

    /**
      * Does the piping action
      */
    virtual void pipeImport(QList<MetaDataParameters*> & bibEntries) = 0;
    virtual void pipeImport(MetaDataParameters* bibEntries) = 0;

signals:
    void progress(int progress);
    void finished();

};

#endif // NEPOMUKPIPE_H
