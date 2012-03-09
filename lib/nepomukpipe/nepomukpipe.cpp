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

#include "nepomukpipe.h"

#include "dms-copy/storeresourcesjob.h"
#include <KDE/KJob>

#include <KDE/KDebug>

NepomukMetaDataExtractor::Pipe::NepomukPipe::NepomukPipe(QObject *parent)
    : QObject(parent)
{
	
}

NepomukMetaDataExtractor::Pipe::NepomukPipe::~NepomukPipe()
{

}

void NepomukMetaDataExtractor::Pipe::NepomukPipe::slotSaveToNepomukDone(KJob *job)
{
    if(job->error()) {
        kDebug() << "Failed to store information in Nepomuk. " << job->errorString();
    }
}
