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

#include <Nepomuk2/SimpleResource>
#include <Nepomuk2/SimpleResourceGraph>
#include <Nepomuk2/DataManagement>
#include <Nepomuk2/StoreResourcesJob>
#include <KDE/KJob>

#include <KDE/KUrl>
#include "nfo/image.h"
#include "nfo/webdataobject.h"
#include "nmm/tvseries.h"

#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/NAO>

#include <KDE/KDebug>

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
    Nepomuk2::SimpleResourceGraph graph;

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

    QString genreList = tvshowEntry.value(QLatin1String("genre")).toString();
    QStringList genresSplitted = genreList.split(';');
    QStringList genres;

    foreach(const QString &genre, genresSplitted) {
        QString gen = genre.trimmed();
        if(!gen.isEmpty())
            genres << gen;
    }

    QString showBannerUrl = tvshowEntry.value(QLatin1String("banner")).toString();
    if( !showBannerUrl.isEmpty() ) {
        const KUrl localUrl = downloadBanner( showBannerUrl, seriesTitle );
        if(!localUrl.isEmpty()) {
            Nepomuk::NFO::Image banner(localUrl);
            seriesRes.addDepiction(banner.uri());
            graph << banner;
        }
    }

    QString showPosterUrl = tvshowEntry.value(QLatin1String("poster")).toString();
    if( !showPosterUrl.isEmpty() ) {
        const KUrl localUrl = downloadBanner( showPosterUrl, seriesTitle );
        if(!localUrl.isEmpty()) {
            Nepomuk::NFO::Image banner(localUrl);
            seriesRes.addDepiction(banner.uri());
            graph << banner;
        }
    }

    // now add all the available series
    QVariantList seasonList = tvshowEntry.value(QLatin1String("seasons")).toList();

    foreach( const QVariant &season, seasonList) {
        QVariantMap seasonInfo = season.toMap();

        //TODO: add title/name to it like Season 1
        Nepomuk::NMM::TVSeason seasonRes;

        QString seasonNumber = seasonInfo.value(QLatin1String("number")).toString();

        seasonRes.setSeasonNumber( seasonNumber.toInt() );
        seasonRes.setSeasonOf(seriesRes.uri());
        seriesRes.addSeason(seasonRes.uri());

        QString seriesBannerUrl = seasonInfo.value(QLatin1String("banner")).toString();
        if( !seriesBannerUrl.isEmpty() ) {
            const KUrl localUrl = downloadBanner( seriesBannerUrl, seriesTitle );
            if(!localUrl.isEmpty()) {
                Nepomuk::NFO::Image banner(localUrl);
                seasonRes.addDepiction(banner.uri());
                graph << banner;
            }
        }

        QString seriesPosterUrl = seasonInfo.value(QLatin1String("poster")).toString();
        if( !seriesPosterUrl.isEmpty() ) {
            const KUrl localUrl = downloadBanner( seriesPosterUrl, seriesTitle );
            if(!localUrl.isEmpty()) {
                Nepomuk::NFO::Image banner(localUrl);
                seasonRes.addDepiction(banner.uri());
                graph << banner;
            }
        }

        QVariantList episodeList = seasonInfo.value(QLatin1String("episodes")).toList();

        kDebug() << "add" << episodeList.size() << "episodes";
        foreach( const QVariant &episode, episodeList) {
            QVariantMap episodeInfo = episode.toMap();

            Nepomuk::NMM::TVShow episodeRes = createEpisode(episodeInfo, seasonRes );
            episodeRes.setGenres( genres );

            QString episodeBannerUrl = episodeInfo.value(QLatin1String("banner")).toString();
            if( !episodeBannerUrl.isEmpty() ) {
                const KUrl localUrl = downloadBanner( episodeBannerUrl, seriesTitle );
                if(!localUrl.isEmpty()) {
                    Nepomuk::NFO::Image banner(localUrl);
                    episodeRes.addDepiction(banner.uri());
                    graph << banner;
                }
            }

            QString episodePosterUrl = episodeInfo.value(QLatin1String("poster")).toString();
            if( !episodePosterUrl.isEmpty() ) {
                const KUrl localUrl = downloadBanner( episodePosterUrl, seriesTitle );
                if(!localUrl.isEmpty()) {
                    Nepomuk::NFO::Image banner(localUrl);
                    episodeRes.addDepiction(banner.uri());
                    graph << banner;
                }
            }

            seasonRes.addSeasonEpisode(episodeRes.uri());
            episodeRes.setIsPartOfSeason(seasonRes.uri());

            // we make the seasons sub-resources of the episodes since they do not make sense without them
            episodeRes.addProperty(NAO::hasSubResource(), seasonRes.uri());

            // Now add all the actors and guest stars

            QList<Nepomuk::NCO::PersonContact> actorList = createPersonContacts( episodeInfo.value(QLatin1String("actors")).toString() );
            foreach(const Nepomuk::NCO::PersonContact &actor, actorList) {
                graph << actor;
                episodeRes.addActor( actor.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), actor.uri());
            }

            QList<Nepomuk::NCO::PersonContact> writerList = createPersonContacts( episodeInfo.value(QLatin1String("writer")).toString() );
            foreach(const Nepomuk::NCO::PersonContact &writer, writerList) {
                graph << writer;
                episodeRes.addWriter( writer.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), writer.uri());
            }

            QList<Nepomuk::NCO::PersonContact> directorList = createPersonContacts( episodeInfo.value(QLatin1String("director")).toString() );
            foreach(const Nepomuk::NCO::PersonContact &director, directorList) {
                graph << director;
                episodeRes.addDirector( director.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), director.uri());
            }

            seriesRes.addEpisode(episodeRes.uri());
            episodeRes.setSeries(seriesRes.uri());
            graph << episodeRes;

        } // end episodeList

        graph << seasonRes;

    } // end seasonList

    graph << seriesRes;

    Nepomuk2::StoreResourcesJob *srj = Nepomuk2::storeResources(graph, Nepomuk2::IdentifyNew, Nepomuk2::OverwriteProperties);
    connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
    srj->exec();
}

Nepomuk::NMM::TVShow NepomukMetaDataExtractor::Pipe::TvShowPipe::createEpisode(QVariantMap episodeInfo, Nepomuk::NMM::TVSeason season )
{
    kDebug() << "create new episode with url" << episodeInfo.value(QLatin1String("resourceuri")).toString();
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

    QDateTime firstAired = createDateTime( episodeInfo.value(QLatin1String("firstaired")).toString() );
    if(firstAired.isValid())
        episodeRes.setReleaseDate( firstAired );

    QString genres = episodeInfo.value(QLatin1String("genres")).toString();
    QStringList genreList = genres.split(';');
    QStringList genreList2;
    foreach(const QString & genre, genreList) {
        genreList2 << genre.trimmed();
    }
    episodeRes.setGenres( genreList2 );

    return episodeRes;
}
