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

#include "tvdbextractor.h"

#include <tvdb/client.h>
#include <tvdb/series.h>
#include <tvdb/season.h>
#include <tvdb/episode.h>
#include <tvdb/banner.h>

NepomukMetaDataExtractor::Extractor::TvdbExtractor::TvdbExtractor(QObject *parent)
    : WebExtractor(parent)
{
    m_client = new Tvdb::Client( this );
    m_client->setFlags(Tvdb::Client::FetchFullDetails);
    connect( m_client, SIGNAL( finished( Tvdb::Series ) ), SLOT( slotFinished( Tvdb::Series ) ) );
    connect( m_client, SIGNAL( multipleResultsFound( QList<Tvdb::Series> ) ), SLOT( slotMultipleResultsFound( QList<Tvdb::Series> ) ) );
}

NepomukMetaDataExtractor::Extractor::TvdbExtractor::~TvdbExtractor()
{
    delete m_client;
}

NepomukMetaDataExtractor::Extractor::WebExtractor::Info NepomukMetaDataExtractor::Extractor::TvdbExtractor::info()
{
    WebExtractor::Info scriptInfo;
    scriptInfo.name = QLatin1String("TVDb");
    scriptInfo.identifier = QLatin1String("tvdbcpp");
    scriptInfo.description = QLatin1String("An open database for television fans");
    scriptInfo.urlregex << QLatin1String("http://thetvdb.com/?tab=episode&seriesid=(\\d+)&seasonid=(\\d+)&id=(\\d+).*");
    scriptInfo.urlregex << QLatin1String("http://thetvdb.com/?tab=season&seriesid=(\\d+)&seasonid=(\\d+).*");
    scriptInfo.urlregex << QLatin1String("http://thetvdb.com/?tab=series&id=(\\d+).*");

    scriptInfo.author = QLatin1String("");
    scriptInfo.email = QLatin1String("");
    scriptInfo.resource << QLatin1String("tvshow");

    return scriptInfo;
}

void NepomukMetaDataExtractor::Extractor::TvdbExtractor::search(const QString &resourceType, const QVariantMap &parameters)
{
    m_searchParameters = parameters;
    m_seriesCache.clear();
    m_client->getSeriesByName( parameters.value(QLatin1String("title")).toString() );
}

void NepomukMetaDataExtractor::Extractor::TvdbExtractor::extractItem(const QUrl &url)
{

}

void NepomukMetaDataExtractor::Extractor::TvdbExtractor::slotFinished( const Tvdb::Series& series )
{
    m_seriesCache.append(series);
}

void NepomukMetaDataExtractor::Extractor::TvdbExtractor::slotMultipleResultsFound( const QList<Tvdb::Series>& series )
{
    m_seriesCache.append(series);
}
