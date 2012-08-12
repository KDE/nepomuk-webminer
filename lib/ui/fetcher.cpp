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

#include "fetcher.h"

#include "metadataparameters.h"
#include "resourceextractor/resourceextractor.h"
#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include "nepomukpipe/nepomukpipe.h"
#include "nepomukpipe/moviepipe.h"
#include "nepomukpipe/publicationpipe.h"
#include "nepomukpipe/tvshowpipe.h"
#include "nepomukpipe/musicpipe.h"

#include <KDE/KDebug>

namespace NepomukMetaDataExtractor {
namespace UI {
    class FetcherPrivate {
    public:
        Extractor::ResourceExtractor *m_re;
        Extractor::ExtractorFactory *m_ef;
    };
}
}

NepomukMetaDataExtractor::UI::Fetcher::Fetcher()
    : d_ptr( new NepomukMetaDataExtractor::UI::FetcherPrivate )

{
    Q_D( Fetcher );
    d->m_re = new Extractor::ResourceExtractor;
    d->m_ef = new Extractor::ExtractorFactory;
}

NepomukMetaDataExtractor::UI::Fetcher::~Fetcher()
{
    Q_D( Fetcher );
    delete d->m_re;
    delete d->m_ef;
}

void NepomukMetaDataExtractor::UI::Fetcher::addFetcherPath( const KUrl &url )
{
    Q_D( Fetcher );
    d->m_re->lookupFiles(url);
}

void NepomukMetaDataExtractor::UI::Fetcher::addFetcherResource( const KUrl &url )
{
    Nepomuk2::Resource r = Nepomuk2::Resource::fromResourceUri( url );

    if(r.exists()) {
        addFetcherResource(r);
    }
}

void NepomukMetaDataExtractor::UI::Fetcher::addFetcherResource(const Nepomuk2::Resource &resource)
{
    Q_D( Fetcher );
    d->m_re->lookupResource(resource);
}

void NepomukMetaDataExtractor::UI::Fetcher::addFetcherResource(const QList<Nepomuk2::Resource> &resources)
{
    Q_D( Fetcher );
    d->m_re->lookupResource(resources);
}

void NepomukMetaDataExtractor::UI::Fetcher::setForceUpdate(bool update)
{
    Q_D( Fetcher );
    d->m_re->setForceUpdate(update);
}

void NepomukMetaDataExtractor::UI::Fetcher::setTvShowMode(bool tvshowMode)
{
    Q_D( Fetcher );
    d->m_re->setTvShowMode( tvshowMode );
}

void NepomukMetaDataExtractor::UI::Fetcher::setTvShowNamesInFolders(bool useFolderNames)
{
    Q_D( Fetcher );
    d->m_re->setTvShowNamesInFolders( useFolderNames );
}

void NepomukMetaDataExtractor::UI::Fetcher::setMovieMode(bool movieMode)
{
    Q_D( Fetcher );
    d->m_re->setMovieMode( movieMode );
}

NepomukMetaDataExtractor::Extractor::ResourceExtractor *NepomukMetaDataExtractor::UI::Fetcher::resourceExtractor()
{
    Q_D( Fetcher );
    return d->m_re;
}

NepomukMetaDataExtractor::Extractor::ExtractorFactory* NepomukMetaDataExtractor::UI::Fetcher::extractorFactory()
{
    Q_D( Fetcher );
    return d->m_ef;
}

void NepomukMetaDataExtractor::UI::Fetcher::addResourceUriToMetaData( NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp )
{
    // For tv shows put the resource uri inti the Episode part of the MetaData
    // this way around it is possible to use the TvShowPipe with more episode/files at once
    //TODO: do not just use first entry, in case we connect more than 1 file here
    if( mdp->resourceType == QLatin1String("tvshow")) {
        QVariantList seasons = mdp->metaData.value(QLatin1String("seasons")).toList();
        if(!seasons.isEmpty()) {
            QVariantMap season = seasons.takeFirst().toMap();
            QVariantList episodes = season.value(QLatin1String("episodes")).toList();

            if(!episodes.isEmpty()) {
                QVariantMap episodesMap = episodes.takeFirst().toMap();
                kDebug() << "add to episode" << episodesMap.value(QLatin1String("title")).toString() << "url" << mdp->resourceUri.url();
                episodesMap.insert(QLatin1String("resourceuri"), mdp->resourceUri.url());

                episodes << episodesMap;
                season.insert( QLatin1String("episodes"), episodes);
                seasons << season;
                mdp->metaData.insert( QLatin1String("seasons"), seasons);
            }
        }
    }
    // music piece / album works the same as tvshows, we add the fileurl to the track not the toplevel album
    //TODO: do not just use first entry, in case we connect more than 1 file here
    else if( mdp->resourceType == QLatin1String("music")) {
        QVariantList trackList = mdp->metaData.value(QLatin1String("tracks")).toList();

        if(!trackList.isEmpty()) {
            QVariantMap trackMap = trackList.takeFirst().toMap();
            kDebug() << "add to track" << trackMap.value(QLatin1String("title")).toString() << "url" << mdp->resourceUri.url();
            trackMap.insert(QLatin1String("resourceuri"), mdp->resourceUri.url());

            trackList << trackMap;
            mdp->metaData.insert( QLatin1String("tracks"), trackList);
        }
    }

    // For all other cases put the resource uri in the top part of the MetaData
    // add this for all cases, helps to find the folder for the poster/cover download easier for music/tvshows)
    mdp->metaData.insert(QLatin1String("resourceuri"), mdp->resourceUri.url());
}

void NepomukMetaDataExtractor::UI::Fetcher::saveMetaData(const NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp) const
{
    QString type = mdp->resourceType;

    Pipe::NepomukPipe *nepomukPipe = 0;

    if(type == QLatin1String("publication")) {
        nepomukPipe = new Pipe::PublicationPipe;
    }
    else if(type == QLatin1String("tvshow")) {
        nepomukPipe = new Pipe::TvShowPipe;
    }
    else if(type == QLatin1String("movie")) {
        nepomukPipe = new Pipe::MoviePipe;
    }
    else if(type == QLatin1String("music")) {
        nepomukPipe = new Pipe::MusicPipe;
    }

    if(nepomukPipe) {
        nepomukPipe->pipeImport( mdp->metaData );
    }
    else {
        kWarning() << "No nepomuk pipe available for the resource type" << type;
    }
}
