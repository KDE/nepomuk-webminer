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

#include "../metadataparameters.h"

#include "dms-copy/simpleresource.h"
#include "dms-copy/simpleresourcegraph.h"
#include "dms-copy/datamanagement.h"
#include "dms-copy/storeresourcesjob.h"
#include <KDE/KJob>

#include "sro/nmm/tvshow.h"

TvShowPipe::TvShowPipe(QObject *parent)
: NepomukPipe(parent)
{
}

TvShowPipe::~TvShowPipe()
{

}

void TvShowPipe::pipeImport(MetaDataParameters* tvshowEntry)
{
    Nepomuk::SimpleResourceGraph graph;

    Nepomuk::NMM::TVShow episodeRes(tvshowEntry->resourceUri);

//    QString title = movieEntry->metaData.value(QLatin1String("title")).toString();
//    episodeRes.setEpisodeNumber(episode);

//    QString title = movieEntry->metaData.value(QLatin1String("title")).toString();
//    episodeRes.setSeason(season);

//    QString title = movieEntry->metaData.value(QLatin1String("title")).toString();
//    episodeRes.setTitle(series[season][episode].name());

//    if(!series[season][episode].overview().isEmpty()) {
//            episodeRes.setSynopsis(series[season][episode].overview());
//        }
//        if(series[season][episode].firstAired().isValid()) {
//            episodeRes.setReleaseDate(QDateTime(series[season][episode].firstAired(), QTime(), Qt::UTC));
//        }

//        episodeRes.setGenres(series.genres());


        graph << episodeRes;

        Nepomuk::storeResources(graph, Nepomuk::IdentifyNew, Nepomuk::OverwriteProperties);
}
