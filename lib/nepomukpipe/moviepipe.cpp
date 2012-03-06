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

#include "moviepipe.h"

#include "../metadataparameters.h"

#include "dms-copy/simpleresource.h"
#include "dms-copy/simpleresourcegraph.h"
#include "dms-copy/datamanagement.h"
#include "dms-copy/storeresourcesjob.h"
#include <KDE/KJob>

#include "sro/nmm/movie.h"

#include <Soprano/Vocabulary/NAO>

using namespace Soprano::Vocabulary;

MoviePipe::MoviePipe(QObject *parent)
    : NepomukPipe(parent)
{
}

MoviePipe::~MoviePipe()
{

}

void MoviePipe::pipeImport(MetaDataParameters* movieEntry)
{
    Nepomuk::SimpleResourceGraph graph;

    // do not use local file url here, this will double type the resource
    // for now this is the best way to deal with this
    Nepomuk::NMM::Movie movieResource(movieEntry->resourceUri);

    QString title = movieEntry->metaData.value(QLatin1String("title")).toString();
    movieResource.setTitle( title );

    QString plot = movieEntry->metaData.value(QLatin1String("plot")).toString();
    QStringList plotList;
    plotList << plot;
    movieResource.setDescriptions( plotList );

    QString genreList = movieEntry->metaData.value(QLatin1String("genres")).toString();
    QStringList genres = genreList.split(';');

    foreach(const QString &genre, genres) {
        movieResource.addGenre( genre );
    }

    QString releaseDate = movieEntry->metaData.value(QLatin1String("year")).toString() + QLatin1String("");
    QDateTime dateTime = QDateTime::fromString(releaseDate, "yyyy");
    movieResource.setReleaseDate( dateTime );

    QString directorList = movieEntry->metaData.value(QLatin1String("director")).toString();
    QStringList directors = directorList.split(';');

    foreach(const QString &director, directors) {
        Nepomuk::NCO::PersonContact directorResource = createPerson(director);

        graph << directorResource;
        movieResource.addDirector( directorResource.uri() );
        movieResource.addProperty(NAO::hasSubResource(), directorResource.uri());
    }

    QString writerList = movieEntry->metaData.value(QLatin1String("writer")).toString();
    QStringList writers = writerList.split(';');

    foreach(const QString &writer, writers) {
        Nepomuk::NCO::PersonContact writerResource = createPerson(writer);

        graph << writerResource;
        movieResource.addWriter( writerResource.uri() );
        movieResource.addProperty(NAO::hasSubResource(), writerResource.uri());
    }

    QString actorList = movieEntry->metaData.value(QLatin1String("cast")).toString();
    QStringList actors = actorList.split(';');

    foreach(const QString &actor, actors) {
        Nepomuk::NCO::PersonContact actorResource = createPerson(actor);

        graph << actorResource;
        movieResource.addActor( actorResource.uri() );
        movieResource.addProperty(NAO::hasSubResource(), actorResource.uri());
    }

    graph << movieResource;

    Nepomuk::storeResources(graph, Nepomuk::IdentifyNew, Nepomuk::OverwriteProperties);
}

Nepomuk::NCO::PersonContact MoviePipe::createPerson(const QString &fullName)
{
    QStringList nameParts = fullName.split(' ');

    Nepomuk::NCO::PersonContact personResource;
    personResource.setFullname( fullName );
    personResource.setNameFamily( nameParts.last() );
    personResource.setNameGiven( nameParts.first() );
    /*
    nameParts.takeFirst();
    nameParts.takeLast();

    if(!nameParts.isEmpty()) {
        personResource.setNameAdditionals( nameParts );
    }
    */

    return personResource;
}
