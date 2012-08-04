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

#include "automaticfetcher.h"

#include "metadataparameters.h"
#include "resourceextractor/resourceextractor.h"
#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include "nepomukpipe/nepomukpipe.h"
#include "nepomukpipe/moviepipe.h"
#include "nepomukpipe/publicationpipe.h"
#include "nepomukpipe/tvshowpipe.h"

#include <KDE/KApplication>
#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

NepomukMetaDataExtractor::UI::AutomaticFetcher::AutomaticFetcher(QObject *parent)
    : QObject(parent),
      m_webextractor(0),
      m_currentItemToupdate(0)
{
    m_re = new ResourceExtractor;
    m_ef = new ExtractorFactory;
}

NepomukMetaDataExtractor::UI::AutomaticFetcher::~AutomaticFetcher()
{
    delete m_re;
    delete m_ef;
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::setInitialPathOrFile( const KUrl &url )
{
    m_re->lookupFiles(url);
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::setForceUpdate(bool update)
{
    m_re->setForceUpdate(update);
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::setTvShowMode(bool tvshowMode)
{
    m_re->setTvShowMode( tvshowMode );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::setTvShowNamesInFolders(bool useFolderNames)
{
    m_re->setTvShowNamesInFolders( useFolderNames );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::setMovieMode(bool movieMode)
{
    m_re->setMovieMode( movieMode );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::startFetcher()
{
    kDebug() << "Start fetching meta data for" << m_re->resourcesList().size() << "items";

    searchNextItem();
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::searchNextItem()
{
    if( m_re->resourcesList().isEmpty() ) {
        kDebug() << "Finished fetching all items";
        kapp->quit();
        return;
    }

    m_currentItemToupdate = m_re->takeNext();

    //TODO: select correct search engine select one from a KConfig where the user specifies his favorite
    //TODO: if the search returns no results, check anothe rsearch engine and try again

    if( m_ef->listAvailablePlugins(m_currentItemToupdate->resourceType).isEmpty()) {
        kWarning() << "Could not get any plugins for the resourcetype :: " << m_currentItemToupdate->resourceType;
        return;
    }

    Extractor::WebExtractor::Info selectedEngine = m_ef->listAvailablePlugins(m_currentItemToupdate->resourceType).last();

    if(!m_webextractor || m_webextractor->info().identifier != selectedEngine.identifier) {
        delete m_webextractor;
        m_webextractor = m_ef->createExtractor( selectedEngine.identifier );

        connect(m_webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(m_webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        //connect(m_webextractor, SIGNAL(log(QString)), this, SLOT(addToProgressLog(QString)));
        connect(m_webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert("title", m_currentItemToupdate->searchTitle);
    searchParameters.insert("alttitle", m_currentItemToupdate->searchAltTitle);
    searchParameters.insert("author", m_currentItemToupdate->searchAuthor);
    searchParameters.insert("season", m_currentItemToupdate->searchSeason);
    searchParameters.insert("episode", m_currentItemToupdate->searchEpisode);
    searchParameters.insert("yearMin", m_currentItemToupdate->searchYearMin);
    searchParameters.insert("yearMax", m_currentItemToupdate->searchYearMax);
    searchParameters.insert("journal", m_currentItemToupdate->searchJournal);
    searchParameters.insert("showtitle", m_currentItemToupdate->searchShowTitle);

    kDebug() << "Start searching " << m_webextractor->info().name << "for items with these parameters ::";
    kDebug() << searchParameters;

    m_webextractor->search( m_currentItemToupdate->resourceType, searchParameters );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::selectSearchEntry( QVariantList searchResults)
{
    if( searchResults.isEmpty() ) {
        kDebug() << "Could not find any search results for the item" << m_currentItemToupdate->resourceUri;

        // delete the current item, we do not need it anymore
        delete m_currentItemToupdate;
        m_currentItemToupdate = 0;

        // and start the next round
        searchNextItem();
    }
    else {
        //FIXME: Find a good way to determine which search result is the best one to take. Or if none fit skip this to avoid false results
        kDebug() << "Found " << searchResults.size() << "search results, taking the first entry";
        kDebug() << searchResults.first();

        QVariantMap selectedSearchResult = searchResults.first().toMap();

        KUrl fetchUrl( selectedSearchResult.value(QLatin1String("url")).toString() );
        m_webextractor->extractItem( fetchUrl, selectedSearchResult );// FIXME: do not put searchresults here but an option QVariantMap
    }
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    // copy the fetched meta data into the current item details
    m_currentItemToupdate->metaData = itemDetails;

    // TODO: support batch download of many episodes at once
    if( resourceType == QLatin1String("tvshow")) {
        QVariantList seasons = m_currentItemToupdate->metaData.value(QLatin1String("seasons")).toList();
        if(!seasons.isEmpty()) {
            QVariantMap season = seasons.takeFirst().toMap();
            QVariantList episodes = season.value(QLatin1String("episodes")).toList();

            if(!episodes.isEmpty()) {
                QVariantMap episodesMap = episodes.takeFirst().toMap();
                kDebug() << "add to episode" << episodesMap.value(QLatin1String("title")).toString() << "url" << m_currentItemToupdate->resourceUri.url();
                episodesMap.insert(QLatin1String("resourceuri"), m_currentItemToupdate->resourceUri.url());

                episodes << episodesMap;
                season.insert( QLatin1String("episodes"), episodes);
                seasons << season;
                m_currentItemToupdate->metaData.insert( QLatin1String("seasons"), seasons);
            }
        }
    }
    else {
        m_currentItemToupdate->metaData.insert(QLatin1String("resourceuri"), m_currentItemToupdate->resourceUri.url());
    }

    saveMetaData();
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::saveMetaData()
{
    QString type = m_currentItemToupdate->resourceType;

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

    if(nepomukPipe) {
        nepomukPipe->pipeImport( m_currentItemToupdate->metaData );
    }
    else {
        kDebug() << "No nepomuk pipe available for the resoure type" << type;
    }

    // delete the current item, we do not need it anymore
    delete m_currentItemToupdate;
    m_currentItemToupdate = 0;

    // and start the next round
    searchNextItem();
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::errorInScriptExecution(const QString &error)
{
    kDebug() << error;
}
