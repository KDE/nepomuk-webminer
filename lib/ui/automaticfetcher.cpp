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

#include <KDE/KDebug>

namespace NepomukMetaDataExtractor {
namespace UI {
    class AutomaticFetcherPrivate {
    public:
        NepomukMetaDataExtractor::Extractor::WebExtractor *webextractor;
        NepomukMetaDataExtractor::Extractor::MetaDataParameters *currentItemToupdate;
        QList<KUrl> urlList;
    };
}
}

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

NepomukMetaDataExtractor::UI::AutomaticFetcher::AutomaticFetcher(QObject *parent)
    : QObject(parent)
    , d_ptr( new NepomukMetaDataExtractor::UI::AutomaticFetcherPrivate )
{
    Q_D( AutomaticFetcher );
    d->webextractor = 0;
    d->currentItemToupdate = 0;
}

NepomukMetaDataExtractor::UI::AutomaticFetcher::~AutomaticFetcher()
{
    Q_D( AutomaticFetcher );
    delete d->webextractor;
    delete d->currentItemToupdate;
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::addFetcherUrl(const KUrl& url)
{
    Q_D( AutomaticFetcher );
    d->urlList.append( url );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::startFetcher()
{
    kDebug() << "Start fetching meta data for" << resourceExtractor()->resourcesList().size() << "items";

    searchNextItem();
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::startUrlFetcher()
{
    Q_D( AutomaticFetcher );
    if(d->urlList.isEmpty()) {
        kWarning() << "started url fetcher without any urls";
        return;
    }

    //TODO: implement downloading of more than one url, loop trough all urls in d->urllist

    // create a webextractor
    d->webextractor = extractorFactory()->createExtractor( d->urlList.first() );

    if(!d->webextractor) {
        kWarning() << "could not find webextractor plugin for URL" << d->urlList.first();
    }
    else {
        d->currentItemToupdate = new MetaDataParameters;
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));

        // TODO: specify some search options, read them form a config file (like no reference or no banner download)
        d->webextractor->extractItem( d->urlList.first(), QVariantMap() );
    }
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::searchNextItem()
{
    Q_D( AutomaticFetcher );
    if( resourceExtractor()->resourcesList().isEmpty() ) {
        kDebug() << "Finished fetching all items";
        return;
    }

    d->currentItemToupdate = resourceExtractor()->takeNext();

    //TODO: select correct search engine select one from a KConfig where the user specifies his favorite
    //TODO: if the search returns no results, check another search engine and try again

    if( extractorFactory()->listAvailablePlugins(d->currentItemToupdate->resourceType).isEmpty()) {
        kWarning() << "Could not get any plugins for the resourcetype :: " << d->currentItemToupdate->resourceType;
        return;
    }

    Extractor::WebExtractor::Info selectedEngine = extractorFactory()->listAvailablePlugins(d->currentItemToupdate->resourceType).last();

    if(!d->webextractor || d->webextractor->info().identifier != selectedEngine.identifier) {
        delete d->webextractor;
        d->webextractor = extractorFactory()->createExtractor( selectedEngine.identifier );

        connect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        //connect(d->webextractor, SIGNAL(log(QString)), this, SLOT(addToProgressLog(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert("title", d->currentItemToupdate->searchTitle);
    searchParameters.insert("alttitle", d->currentItemToupdate->searchAltTitle);
    searchParameters.insert("author", d->currentItemToupdate->searchPerson);
    searchParameters.insert("season", d->currentItemToupdate->searchSeason);
    searchParameters.insert("episode", d->currentItemToupdate->searchEpisode);
    searchParameters.insert("yearMin", d->currentItemToupdate->searchYearMin);
    searchParameters.insert("yearMax", d->currentItemToupdate->searchYearMax);
    searchParameters.insert("journal", d->currentItemToupdate->searchJournal);
    searchParameters.insert("showtitle", d->currentItemToupdate->searchShowTitle);
    searchParameters.insert("album", d->currentItemToupdate->searchAlbum);
    searchParameters.insert("track", d->currentItemToupdate->searchTrack);

    kDebug() << "Start searching " << d->webextractor->info().name << "for items with these parameters ::";
    kDebug() << searchParameters;

    d->webextractor->search( d->currentItemToupdate->resourceType, searchParameters );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::selectSearchEntry( QVariantList searchResults)
{
    Q_D( AutomaticFetcher );

    if( searchResults.isEmpty() ) {
        kDebug() << "Could not find any search results for the item" << d->currentItemToupdate->resourceUri;

        // delete the current item, we do not need it anymore
        delete d->currentItemToupdate;
        d->currentItemToupdate = 0;

        // and start the next round
        searchNextItem();
    }
    else {
        //FIXME: Find a good way to determine which search result is the best one to take. Or if none fit skip this to avoid false results
        kDebug() << "Found " << searchResults.size() << "search results, taking the first entry";
        kDebug() << searchResults.first();

        QVariantMap selectedSearchResult = searchResults.first().toMap();

        KUrl fetchUrl( selectedSearchResult.value(QLatin1String("url")).toString() );
        // TODO: specify some search options, read them form a config file (like no reference or no banner download)
        d->webextractor->extractItem( fetchUrl, QVariantMap() );
    }
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    Q_D( AutomaticFetcher );

    // copy the fetched meta data into the current item details
    d->currentItemToupdate->metaData = itemDetails;
    d->currentItemToupdate->resourceType = resourceType;

    addResourceUriToMetaData( d->currentItemToupdate );

    saveMetaData( d->currentItemToupdate );

    // delete the current item, we do not need it anymore
    delete d->currentItemToupdate;
    d->currentItemToupdate = 0;

    // and start the next round
    searchNextItem();
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::errorInScriptExecution(const QString &error)
{
    kDebug() << error;
}
