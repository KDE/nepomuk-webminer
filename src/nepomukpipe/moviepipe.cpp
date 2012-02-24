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

#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/Vocabulary/NMM>

#include <QDebug>

using namespace Nepomuk::Vocabulary;

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
    Nepomuk::NMM::Movie movieResource(movieEntry->resourceUri);

    QString title = movieEntry->metaData.value(QLatin1String("title")).toString();
    movieResource.setProperty(NIE::title(), title);

    QString plot = movieEntry->metaData.value(QLatin1String("plot")).toString();
    movieResource.setProperty(NIE::description(), plot);

    QString genreList = movieEntry->metaData.value(QLatin1String("genres")).toString();
    QStringList genres = genreList.split(';');

    foreach(const QString &genre, genres) {
        movieResource.addProperty(NMM::genre(), genre);
    }

    QString releaseDate = movieEntry->metaData.value(QLatin1String("year")).toString() + QLatin1String("");
    movieResource.setProperty(NMM::releaseDate(), releaseDate);

    QString directorList = movieEntry->metaData.value(QLatin1String("director")).toString();
    QStringList directors = directorList.split(';');

    foreach(const QString &director, directors) {
        QStringList directorParts = director.split(' ');

        Nepomuk::NCO::PersonContact directorResource;
        directorResource.setFullname( director );
        directorResource.setNameFamily( directorParts.last() );
        directorResource.setNameGiven( directorParts.first() );
//        directorParts.takeFirst();
//        directorParts.takeLast();

//        if(!directorParts.isEmpty()) {
//            directorResource.setNameAdditionals( directorParts );
//        }

        graph << directorResource;
        movieResource.addProperty(NMM::director(), directorResource);
    }

    QString writerList = movieEntry->metaData.value(QLatin1String("writer")).toString();
    QStringList writers = writerList.split(';');

    foreach(const QString &writer, writers) {
        QStringList writerParts = writer.split(' ');

        Nepomuk::NCO::PersonContact writerResource;
        writerResource.setFullname( writer );
        writerResource.setNameFamily( writerParts.last() );
        writerResource.setNameGiven( writerParts.first() );
//        writerParts.takeFirst();
//        writerParts.takeLast();

//        if(!writerParts.isEmpty()) {
//            writerResource.setNameAdditionals( writerParts );
//        }

        graph << writerResource;
        movieResource.addProperty(NMM::writer(), writerResource);
    }

    QString actorList = movieEntry->metaData.value(QLatin1String("cast")).toString();
    QStringList actors = actorList.split(';');

    foreach(const QString &actor, actors) {
        QStringList actorParts = actor.split(' ');

        Nepomuk::NCO::PersonContact actorResource;
        actorResource.setFullname( actor );
        actorResource.setNameFamily( actorParts.last() );
        actorResource.setNameGiven( actorParts.first() );
//        actorParts.takeFirst();
//        actorParts.takeLast();

//        if(!actorParts.isEmpty()) {
//            actorResource.setNameAdditionals( actorParts );
//        }

        graph << actorResource;
        movieResource.addProperty(NMM::actor(), actorResource);
    }

    graph << movieResource;

    Nepomuk::StoreResourcesJob *srj = Nepomuk::storeResources(graph, Nepomuk::IdentifyNew, Nepomuk::OverwriteProperties);
    //connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
    srj->exec();

    qDebug() << srj->errorText();

    // get the nepomu kuri of the newly created resource
    //QUrl movieUri( srj->mappings().value( movieResource.uri() ));

    // add crossref with
}

Nepomuk::NCO::PersonContact createPerson(const QString &fullName)
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
