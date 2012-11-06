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
#include "sro/nfo/image.h"
#include "sro/nfo/webdataobject.h"
#include "sro/nmm/tvseries.h"
#include "sro/nco/personcontact.h"

#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/NAO>
#include <KDE/KLocalizedString>

#include <KDE/KDebug>

using namespace Soprano::Vocabulary;

namespace NepomukMetaDataExtractor {
namespace Pipe {
class TvShowPipePrivate {
    // nothing yet, but might be needed in the future
};
}
}

NepomukMetaDataExtractor::Pipe::TvShowPipe::TvShowPipe(QObject *parent)
    : NepomukPipe(parent)
    , d_ptr( new NepomukMetaDataExtractor::Pipe::TvShowPipePrivate )
{
}

void NepomukMetaDataExtractor::Pipe::TvShowPipe::pipeImport(const QVariantMap &tvshowEntry)
{
    Nepomuk2::SimpleResourceGraph graph;

    // get all the series information
    Nepomuk2::NMM::TVSeries seriesRes;

    QString seriesTitle = tvshowEntry.value(QLatin1String("title")).toString();
    QString seriesOverview = tvshowEntry.value(QLatin1String("overview")).toString();

    if( !seriesTitle.isEmpty()) {
        seriesRes.setTitle( seriesTitle );
    }
    if( !seriesOverview.isEmpty()) {
        seriesRes.addNieDescription( seriesOverview );
    }

    QString seriesImdbId = tvshowEntry.value(QLatin1String("imdbid")).toString();
    if( !seriesImdbId.isEmpty() ) {
        // TODO: this is not nice: the DMS does not allow to simply use an http URL as object
        Nepomuk2::NFO::WebDataObject imdbRes( seriesImdbId );
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

        KUrl resourceFolder( tvshowEntry.value(QLatin1String("resourceuri")).toString() );
        const KUrl localUrl = downloadBanner( showBannerUrl,
                                              QString("%1 (banner)").arg(seriesTitle),
                                              seriesTitle,
                                              resourceFolder.directory()
                                              );

        if(!localUrl.isEmpty()) {
            Nepomuk2::NFO::Image banner(localUrl);
            seriesRes.addDepiction(banner.uri());
            graph << banner;
        }
    }

    QString showPosterUrl = tvshowEntry.value(QLatin1String("poster")).toString();
    if( !showPosterUrl.isEmpty() ) {

        KUrl resourceFolder( tvshowEntry.value(QLatin1String("resourceuri")).toString() );
        const KUrl localUrl = downloadBanner( showPosterUrl,
                                              QString("%1 (poster)").arg(seriesTitle),
                                              seriesTitle,
                                              resourceFolder.directory()
                                              );
        if(!localUrl.isEmpty()) {
            Nepomuk2::NFO::Image banner(localUrl);
            seriesRes.addDepiction(banner.uri());
            graph << banner;
        }
    }

    // now add all the available series
    QVariantList seasonList = tvshowEntry.value(QLatin1String("seasons")).toList();

    foreach( const QVariant &season, seasonList) {
        QVariantMap seasonInfo = season.toMap();

        Nepomuk2::NMM::TVSeason seasonRes;

        QString seasonNumber = seasonInfo.value(QLatin1String("number")).toString();
        // this is just for the internal usage in NepSak
        seasonRes.setPrefLabel(i18n("Season %1",seasonNumber));

        seasonRes.setSeasonNumber( seasonNumber.toInt() );
        seasonRes.setSeasonOf(seriesRes.uri());
        seriesRes.addSeason(seasonRes.uri());

        QString seriesBannerUrl = seasonInfo.value(QLatin1String("banner")).toString();
        if( !seriesBannerUrl.isEmpty() ) {
            KUrl resourceFolder( tvshowEntry.value(QLatin1String("resourceuri")).toString() );
            const KUrl localUrl = downloadBanner( seriesBannerUrl,
                                                  QString("%1 S%2 (banner)").arg(seriesTitle).arg(seasonNumber),
                                                  seriesTitle,
                                                  resourceFolder.directory()
                                                  );
            if(!localUrl.isEmpty()) {
                Nepomuk2::NFO::Image banner(localUrl);
                seasonRes.addDepiction(banner.uri());
                graph << banner;
            }
        }

        QString seriesPosterUrl = seasonInfo.value(QLatin1String("poster")).toString();
        if( !seriesPosterUrl.isEmpty() ) {
            KUrl resourceFolder( tvshowEntry.value(QLatin1String("resourceuri")).toString() );
            const KUrl localUrl = downloadBanner( seriesPosterUrl,
                                                  QString("%1 S%2 (poster)").arg(seriesTitle).arg(seasonNumber),
                                                  seriesTitle,
                                                  resourceFolder.directory()
                                                  );
            if(!localUrl.isEmpty()) {
                Nepomuk2::NFO::Image banner(localUrl);
                seasonRes.addDepiction(banner.uri());
                graph << banner;
            }
        }

        QVariantList episodeList = seasonInfo.value(QLatin1String("episodes")).toList();

        kDebug() << "add" << episodeList.size() << "episodes to the show" << seriesTitle;
        foreach( const QVariant &episode, episodeList) {
            QVariantMap episodeInfo = episode.toMap();

            Nepomuk2::NMM::TVShow episodeRes = createEpisode(episodeInfo, seasonRes );
            if( !genres.isEmpty() ) {
                episodeRes.setGenres( genres );
            }

            QString episodeBannerUrl = episodeInfo.value(QLatin1String("banner")).toString();
            if( !episodeBannerUrl.isEmpty() ) {

                KUrl resourceFolder( tvshowEntry.value(QLatin1String("resourceuri")).toString() );
                const KUrl localUrl = downloadBanner( episodeBannerUrl,
                                                      QString("%1 S%2E%3 (banner)").arg(seriesTitle).arg(seasonNumber).arg(episodeInfo.value(QLatin1String("number")).toString() ),
                                                      seriesTitle,
                                                      resourceFolder.directory()
                                                      );

                if(!localUrl.isEmpty()) {
                    Nepomuk2::NFO::Image banner(localUrl);
                    episodeRes.addDepiction(banner.uri());
                    graph << banner;
                }
            }

            QString episodePosterUrl = episodeInfo.value(QLatin1String("poster")).toString();
            if( !episodePosterUrl.isEmpty() ) {

                KUrl resourceFolder( tvshowEntry.value(QLatin1String("resourceuri")).toString() );
                const KUrl localUrl = downloadBanner( episodePosterUrl,
                                                      QString("%1 S%2E%3 (poster)").arg(seriesTitle).arg(seasonNumber).arg(episodeInfo.value(QLatin1String("number")).toString() ),
                                                      seriesTitle,
                                                      resourceFolder.directory()
                                                      );
                if(!localUrl.isEmpty()) {
                    Nepomuk2::NFO::Image banner(localUrl);
                    episodeRes.addDepiction(banner.uri());
                    graph << banner;
                }
            }

            seasonRes.addSeasonEpisode(episodeRes.uri());
            episodeRes.setIsPartOfSeason(seasonRes.uri());

            // we make the seasons sub-resources of the episodes since they do not make sense without them
            episodeRes.addProperty(NAO::hasSubResource(), seasonRes.uri());

            // Now add all the actors and guest stars
            QList<Nepomuk2::NCO::Contact> actorList = createPersonContacts( episodeInfo.value(QLatin1String("actors")).toString() );
            foreach(const Nepomuk2::NCO::Contact &actor, actorList) {
                graph << actor;
                episodeRes.addActor( actor.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), actor.uri());
            }

            QList<Nepomuk2::NCO::Contact> writerList = createPersonContacts( episodeInfo.value(QLatin1String("writer")).toString() );
            foreach(const Nepomuk2::NCO::Contact &writer, writerList) {
                graph << writer;
                episodeRes.addWriter( writer.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), writer.uri());
            }

            QList<Nepomuk2::NCO::Contact> directorList = createPersonContacts( episodeInfo.value(QLatin1String("director")).toString() );
            foreach(const Nepomuk2::NCO::Contact &director, directorList) {
                graph << director;
                episodeRes.addDirector( director.uri() );
                episodeRes.addProperty(NAO::hasSubResource(), director.uri());
            }

            //Add the url where we fetched the data from as SeeAlso
            QString seeAlso = episodeInfo.value(QLatin1String("seealso")).toString();
            if ( !seeAlso.isEmpty() ) {
                KUrl saUrl = KUrl(seeAlso);
                Nepomuk2::NFO::WebDataObject seeAlsoRes( saUrl );
                episodeRes.addProperty(RDFS::seeAlso(), seeAlsoRes.uri());
                graph << seeAlsoRes;
            }

            seriesRes.addEpisode(episodeRes.uri());
            episodeRes.setSeries(seriesRes.uri());
            graph << episodeRes;

        } // end episodeList

        graph << seasonRes;

    } // end seasonList

    graph << seriesRes;

    Nepomuk2::StoreResourcesJob *srj = Nepomuk2::storeResources(graph, Nepomuk2::IdentifyNew, Nepomuk2::OverwriteProperties,
                                                                QHash<QUrl,QVariant>(),KComponentData(componentName().toLatin1()));
    connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
    srj->exec();
}

Nepomuk2::NMM::TVShow NepomukMetaDataExtractor::Pipe::TvShowPipe::createEpisode(const QVariantMap &episodeInfo, const Nepomuk2::NMM::TVSeason &season )
{
    QString resourceUri = episodeInfo.value(QLatin1String("resourceuri")).toString();
    QUrl existingUri;
    existingUri.setEncodedUrl(resourceUri.toLatin1());

    // first remove the old metadata from the file before we create a new episode later on
    KJob *job = Nepomuk2::removeDataByApplication(QList<QUrl>() << existingUri, Nepomuk2::NoRemovalFlags, KComponentData(componentName().toLatin1()) );
    if (!job->exec() ) {
        kWarning() << job->errorString();
    }
    else {
        kDebug() << "Successfully removed old metadata from " << existingUri;
    }

    // now create the graph and fill it with all the new metadata
    kDebug() << "create new episode with url" << episodeInfo.value(QLatin1String("resourceuri")).toString();

    Nepomuk2::NMM::TVShow episodeRes(existingUri);

    QString episodeNumber = episodeInfo.value(QLatin1String("number")).toString();
    if( !episodeNumber.isEmpty()) {
        episodeRes.setEpisodeNumber(episodeNumber.toInt());
    }

    // Deprecated but keep for backwards compatibility
    episodeRes.setSeason( season.seasonNumber() );

    QString episodeTitle = episodeInfo.value(QLatin1String("title")).toString();
    if( !episodeTitle.isEmpty()) {
        episodeRes.setTitle( episodeTitle );
    }

    QString episodeOverview = episodeInfo.value(QLatin1String("overview")).toString();
    if(!episodeOverview.isEmpty()) {
        episodeRes.setSynopsis( episodeOverview );
    }

    QDateTime firstAired = createDateTime( episodeInfo.value(QLatin1String("firstaired")).toString() );
    if(firstAired.isValid()) {
        episodeRes.setReleaseDate( firstAired );
    }

    QString genres = episodeInfo.value(QLatin1String("genres")).toString();
    QStringList genreList = genres.split(';');
    QStringList genreList2;
    foreach(const QString & genre, genreList) {
        genreList2 << genre.trimmed();
    }

    if( !genreList2.isEmpty()) {
        episodeRes.setGenres( genreList2 );
    }

    return episodeRes;
}
