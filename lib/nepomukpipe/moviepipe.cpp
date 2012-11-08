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

#include <Nepomuk2/SimpleResource>
#include <Nepomuk2/SimpleResourceGraph>
#include <Nepomuk2/DataManagement>
#include <Nepomuk2/StoreResourcesJob>

#include "ontology/nmm/movie.h"
#include "ontology/nco/personcontact.h"
#include "ontology/nfo/image.h"
#include "ontology/nfo/webdataobject.h"
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/RDFS>

#include <KDE/KJob>

#include <KDE/KDebug>

using namespace Soprano::Vocabulary;

namespace NepomukMetaDataExtractor
{
namespace Pipe
{
class MoviePipePrivate
{
    // nothing yet, but might be needed in the future
};
}
}


NepomukMetaDataExtractor::Pipe::MoviePipe::MoviePipe(QObject *parent)
    : NepomukPipe(parent)
    , d_ptr(new NepomukMetaDataExtractor::Pipe::MoviePipePrivate)
{
}

void NepomukMetaDataExtractor::Pipe::MoviePipe::pipeImport(const QVariantMap &movieEntry)
{
    //TODO: do not use local file url here, this will double type the resource for now this is the best way to deal with this
    QString resourceUri = movieEntry.value(QLatin1String("resourceuri"), QString()).toString();
    KUrl existingUri;
    existingUri.setEncodedUrl(resourceUri.toLatin1());

    // first remove the old metadata
    //BUG: Removing lots of person subresources cause virtuoso to go crazy. Takes ~10Min to get all the data removed properly
    KJob *job = Nepomuk2::removeDataByApplication(QList<QUrl>() << existingUri, Nepomuk2::NoRemovalFlags, KComponentData(componentName().toLatin1()));
    if (!job->exec()) {
        kWarning() << job->errorString();
    } else {
        kDebug() << "Successfully removed old metadata from " << existingUri;
    }

    // now create the graph and fill it with all the new metadata
    Nepomuk2::SimpleResourceGraph graph;

    Nepomuk2::NMM::Movie movieResource(existingUri);

    QString title = movieEntry.value(QLatin1String("title")).toString();
    movieResource.setTitle(title);

    kDebug() << "Import movie from url" << resourceUri << "with name " << title;

    QString plot = movieEntry.value(QLatin1String("plot")).toString();
    if (!plot.isEmpty()) {
        movieResource.setSynopsis(plot);
    }

    QString genreList = movieEntry.value(QLatin1String("genre")).toString();
    QStringList genres = genreList.split(';');
    QStringList genres2;

    foreach (const QString & genre, genres) {
        QString gen = genre.trimmed();
        if (!gen.isEmpty())
            genres2 << gen;
    }
    if (!genres2.isEmpty()) {
        movieResource.setGenres(genres2);
    }

    QString moviePosterUrl = movieEntry.value(QLatin1String("poster")).toString();
    if (!moviePosterUrl.isEmpty()) {
        KUrl resourceFolder(existingUri);
        const KUrl localUrl = downloadBanner(moviePosterUrl,
                                             QString("%1-poster").arg(title),
                                             title,
                                             resourceFolder.directory());
        if (!localUrl.isEmpty()) {
            Nepomuk2::NFO::Image banner(localUrl);
            movieResource.addDepiction(banner.uri());
            graph << banner;
        }
    }

    QDateTime releaseDate = createDateTime(movieEntry.value(QLatin1String("year")).toString());
    if (releaseDate.isValid())
        movieResource.setReleaseDate(releaseDate);

    QList<Nepomuk2::NCO::Contact> directorList = createPersonContacts(movieEntry.value(QLatin1String("director")).toString());
    foreach (const Nepomuk2::NCO::Contact & director, directorList) {
        graph << director;
        movieResource.addDirector(director.uri());
        movieResource.addProperty(NAO::hasSubResource(), director.uri());
    }

    QList<Nepomuk2::NCO::Contact> writerList = createPersonContacts(movieEntry.value(QLatin1String("writer")).toString());
    foreach (const Nepomuk2::NCO::Contact & writer, writerList) {
        graph << writer;
        movieResource.addWriter(writer.uri());
        movieResource.addProperty(NAO::hasSubResource(), writer.uri());
    }

    QList<Nepomuk2::NCO::Contact> actorList = createPersonContacts(movieEntry.value(QLatin1String("cast")).toString());
    foreach (const Nepomuk2::NCO::Contact & actor, actorList) {
        graph << actor;
        movieResource.addActor(actor.uri());
        movieResource.addProperty(NAO::hasSubResource(), actor.uri());
    }

    //Add the url where we fetched the data from as SeeAlso
    QString seeAlso = movieEntry.value(QLatin1String("seealso")).toString();
    if (!seeAlso.isEmpty()) {
        QUrl saUrl = QUrl(seeAlso);
        Nepomuk2::NFO::WebDataObject seeAlsoRes(saUrl);
        movieResource.addProperty(RDFS::seeAlso(), seeAlsoRes.uri());
        graph << seeAlsoRes;
    }

    graph << movieResource;

    Nepomuk2::StoreResourcesJob *srj = Nepomuk2::storeResources(graph, Nepomuk2::IdentifyNew, Nepomuk2::OverwriteProperties,
                                       QHash<QUrl, QVariant>(), KComponentData(componentName().toLatin1()));
    connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
    srj->exec();
}
