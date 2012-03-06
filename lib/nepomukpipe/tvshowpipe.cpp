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

#include "tvshowpipe.h"

#include "dms-copy/simpleresource.h"
#include "dms-copy/datamanagement.h"
#include "dms-copy/storeresourcesjob.h"
#include <KDE/KJob>

#include <KDE/KUrl>
#include "nfo/image.h"
#include "nfo/webdataobject.h"

#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/NAO>

using namespace Soprano::Vocabulary;

NepomukMetaDataExtractor::Pipe::TvShowPipe::TvShowPipe(QObject *parent)
: NepomukPipe(parent)
{
}

NepomukMetaDataExtractor::Pipe::TvShowPipe::~TvShowPipe()
{

}

void NepomukMetaDataExtractor::Pipe::TvShowPipe::pipeImport(const QVariantMap &tvshowEntry)
{
    Nepomuk::SimpleResourceGraph graph;

    // get all the series information
    Nepomuk::NMM::TVSeries seriesRes;

    QString seriesTitle = tvshowEntry.value(QLatin1String("title")).toString();
    QString seriesOverview = tvshowEntry.value(QLatin1String("overview")).toString();

    seriesRes.setTitle( seriesTitle );
    seriesRes.addDescription( seriesOverview );

    QString seriesImdbId = tvshowEntry.value(QLatin1String("imdbid")).toString();
    if( !seriesImdbId.isEmpty() ) {
        // TODO: this is not nice: the DMS does not allow to simply use an http URL as object
        Nepomuk::NFO::WebDataObject imdbRes( seriesImdbId );
        seriesRes.addProperty(RDFS::seeAlso(), imdbRes.uri());
        graph << imdbRes;
    }

    QString seriesBanner = tvshowEntry.value(QLatin1String("Banner")).toString();
    //const KUrl localUrl = downloadBanner(series.name(), seriesBanner;
//    if(!localUrl.isEmpty()) {
//        Nepomuk::NFO::Image banner(localUrl);
//        seriesRes.addDepiction(banner.uri());
//        graph << banner;
//    }

    // now add all the available series
    QVariantList seasonList = tvshowEntry.value(QLatin1String("seasons")).toList();

    foreach( const QVariant &season, seasonList) {
        QVariantMap seasonInfo = season.toMap();

        Nepomuk::NMM::TVSeason seasonRes;

        QString seasonNumber = seasonInfo.value(QLatin1String("number")).toString();

        seasonRes.setSeasonNumber( seasonNumber.toInt() );
        seasonRes.setSeasonOf(seriesRes.uri());
        seriesRes.addSeason(seasonRes.uri());

        QString seasonBanner = seasonInfo.value(QLatin1String("banner")).toString();
//        const KUrl localUrl = downloadBanner(series.name(), seriesBanner;
//        if(!localUrl.isEmpty()) {
//            Nepomuk::NFO::Image banner(localUrl);
//            seasonRes.addDepiction(banner.uri());
//            graph << banner;
//        }

        QVariantMap episodeList = seasonInfo.value(QLatin1String("episodes")).toMap();

        foreach( const QVariant &episode, episodeList) {
            QVariantMap episodeInfo = episode.toMap();

            Nepomuk::NMM::TVShow episodeRes = createEpisode(episodeInfo, seasonRes );

            seasonRes.addSeasonEpisode(episodeRes.uri());
            episodeRes.setIsPartOfSeason(seasonRes.uri());

            // we make the seasons sub-resources of the episodes since they do not make sense without them
            episodeRes.addProperty(NAO::hasSubResource(), seasonRes.uri());

            // Now add all the actors and guest stars
            QString actorList = episodeInfo.value(QLatin1String("actors")).toString();
            QStringList actors = actorList.split(';');

            foreach(const QString &actor, actors) {
                Nepomuk::NCO::PersonContact actorResource = createPerson(actor);

                graph << actorResource;
                episodeRes.addActor( actorResource.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), actorResource.uri());
            }

            // the script writers
            QString writerList = episodeInfo.value(QLatin1String("writer")).toString();
            QStringList writers = writerList.split(';');

            foreach(const QString &writer, writers) {
                Nepomuk::NCO::PersonContact writerResource = createPerson(writer);

                graph << writerResource;
                episodeRes.addWriter( writerResource.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), writerResource.uri());
            }

            // the directos responsible for it
            QString directorList = episodeInfo.value(QLatin1String("director")).toString();
            QStringList directors = directorList.split(';');

            foreach(const QString &director, directors) {
                Nepomuk::NCO::PersonContact directorResource = createPerson(director);

                graph << directorResource;
                episodeRes.addWriter( directorResource.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), directorResource.uri());
            }

            seriesRes.addEpisode(episodeRes.uri());
            episodeRes.setSeries(seriesRes.uri());
            graph << episodeRes;

        } // end episodeList

        graph << seriesRes;

    } // end seasonList

    graph << seriesRes;

    Nepomuk::storeResources(graph, Nepomuk::IdentifyNew, Nepomuk::OverwriteProperties);
}

Nepomuk::NMM::TVShow NepomukMetaDataExtractor::Pipe::TvShowPipe::createEpisode(QVariantMap episodeInfo, Nepomuk::NMM::TVSeason season )
{
    KUrl localFile( episodeInfo.value(QLatin1String("resourceuri")).toString() );
    Nepomuk::NMM::TVShow episodeRes(localFile);

    QString episodeNumber = episodeInfo.value(QLatin1String("number")).toString();
    episodeRes.setEpisodeNumber(episodeNumber.toInt());

    episodeRes.setSeason( season.seasonNumber() );

    QString episodeTitle = episodeInfo.value(QLatin1String("title")).toString();
    episodeRes.setTitle( episodeTitle );

    QString episodeOverview = episodeInfo.value(QLatin1String("overview")).toString();
    if(!episodeOverview.isEmpty()) {
        episodeRes.setSynopsis( episodeOverview );
    }

    QString episodeFirstAired = episodeInfo.value(QLatin1String("firstaired")).toString();
    QDateTime firstAired = QDateTime::fromString(episodeFirstAired, Qt::ISODate);
    if(firstAired.isValid()) {
        episodeRes.setReleaseDate( firstAired );
    }

    QString genres = episodeInfo.value(QLatin1String("genres")).toString();
    QStringList genreList = genres.split(';');
    QStringList genreList2;
    foreach(const QString & genre, genreList) {
        genreList2 << genre.trimmed();
    }
    episodeRes.setGenres( genreList2 );

    return episodeRes;
}

Nepomuk::NCO::PersonContact NepomukMetaDataExtractor::Pipe::TvShowPipe::createPerson(const QString &fullName)
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
