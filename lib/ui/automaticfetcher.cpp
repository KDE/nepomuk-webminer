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

#include <mdesettings.h>
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
        QList<NepomukMetaDataExtractor::Extractor::WebExtractor::Info> pluginqueue;
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
        emit finished();
        return;
    }

    //TODO: implement downloading of more than one url, loop trough all urls in d->urllist

    // create a webextractor
    d->webextractor = extractorFactory()->getExtractor( d->urlList.first() );

    if(!d->webextractor) {
        kWarning() << "could not find webextractor plugin for URL" << d->urlList.first();
    }
    else {
        d->currentItemToupdate = new MetaDataParameters;
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));

        QVariantMap options;
        options.insert(QString("references"), MDESettings::downloadReferences());
        options.insert(QString("banner"), MDESettings::downloadBanner());

        d->webextractor->extractItem( d->urlList.first(), options );
    }
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::searchNextItem()
{
    Q_D( AutomaticFetcher );
    if( resourceExtractor()->resourcesList().isEmpty() ) {
        kDebug() << "Finished fetching all items";
        emit finished();
        return;
    }

    // don't takenext so we can try another plugin on this resource next
    d->currentItemToupdate = resourceExtractor()->resourcesList().first();

    if (d->pluginqueue.isEmpty()) {
        d->pluginqueue.append(extractorFactory()->listAvailablePlugins(d->currentItemToupdate->resourceType()));
    }
    if( d->pluginqueue.isEmpty()) {
        kWarning() << "Could not get any plugins for the resourcetype :: " << d->currentItemToupdate->resourceType();
        return;
    }

    QString favPlugin;
    if( d->currentItemToupdate->resourceType() == QString("movie")) {
        favPlugin = MDESettings::favoriteMoviePlugin();
    }
    if( d->currentItemToupdate->resourceType() == QString("tvshow")) {
        favPlugin = MDESettings::favoriteTvShowPlugin();
    }
    if( d->currentItemToupdate->resourceType() == QString("publication")) {
        favPlugin = MDESettings::favoritePublicationPlugin();
    }
    if( d->currentItemToupdate->resourceType() == QString("music")) {
        favPlugin = MDESettings::favoriteMusicPlugin();
    }

    Extractor::WebExtractor::Info selectedEngine;
    // if a fave is set we put it in the front of the queue, if it exists!
    if(!favPlugin.isEmpty()) {
        Extractor::WebExtractor::Info faveinfo;
        if (d->webextractor && d->webextractor->info().identifier == favPlugin) {
            faveinfo = d->webextractor->info();
        } else {
            faveinfo.identifier = favPlugin;
        }
        if (d->pluginqueue.contains(faveinfo)) {
            // if we do not want to check more than the fav plugin
            // clear list again and set only the fav plugin
            if( !MDESettings::checkNextPlugin()) {
                d->pluginqueue.clear();
                d->pluginqueue << faveinfo;
            }
            // otherwise just put it in the front and start with it
            else {
                d->pluginqueue.move(d->pluginqueue.indexOf(faveinfo), 0);
            }
        }
    }
    selectedEngine = d->pluginqueue.takeFirst();

    if(!d->webextractor || d->webextractor->info().identifier != selectedEngine.identifier) {

        if(d->webextractor) {
            disconnect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
            disconnect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
            disconnect(d->webextractor, SIGNAL(log(QString)), this, SLOT(log(QString)));
            disconnect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
        }

        d->webextractor = extractorFactory()->getExtractor( selectedEngine.identifier );

        connect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(log(QString)), this, SLOT(log(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert("title", d->currentItemToupdate->searchTitle());
    searchParameters.insert("alttitle", d->currentItemToupdate->searchAltTitle());
    searchParameters.insert("author", d->currentItemToupdate->searchPerson());
    searchParameters.insert("season", d->currentItemToupdate->searchSeason());
    searchParameters.insert("episode", d->currentItemToupdate->searchEpisode());
    searchParameters.insert("yearMin", d->currentItemToupdate->searchYearMin());
    searchParameters.insert("yearMax", d->currentItemToupdate->searchYearMax());
    searchParameters.insert("journal", d->currentItemToupdate->searchJournal());
    searchParameters.insert("showtitle", d->currentItemToupdate->searchShowTitle());
    searchParameters.insert("album", d->currentItemToupdate->searchAlbum());
    searchParameters.insert("track", d->currentItemToupdate->searchTrack());

    kDebug() << "Start searching with " << d->webextractor->info().name;
    //kDebug() << searchParameters;

    d->webextractor->search( d->currentItemToupdate->resourceType(), searchParameters );
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::selectSearchEntry( QVariantList searchResults)
{
    Q_D( AutomaticFetcher );

    if( searchResults.isEmpty() ) {
        kDebug() << "Could not find any search results for the item" << d->currentItemToupdate->resourceUri();

        if (d->pluginqueue.isEmpty()) {
            // we're out of plugins
            // delete the current item, we do not need it anymore
            resourceExtractor()->takeNext(); // pop it off the list
            delete d->currentItemToupdate; // and delete it
            d->currentItemToupdate = 0;
            // the pluginqueue will be refilled in searchNextItem since it's now empty
        }

        // and start the next round
        searchNextItem();
    }
    else {
        //FIXME: Find a good way to determine which search result is the best one to take. Or if none fit skip this to avoid false results
        kDebug() << "Found " << searchResults.size() << "search results, taking the first entry";
        kDebug() << searchResults.first();

        QVariantMap selectedSearchResult = searchResults.first().toMap();

        KUrl fetchUrl( selectedSearchResult.value(QLatin1String("url")).toString() );

        QVariantMap options;
        options.insert(QString("references"), MDESettings::downloadReferences());
        options.insert(QString("banner"), MDESettings::downloadBanner());

        d->webextractor->extractItem( fetchUrl, options );
    }
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    Q_D( AutomaticFetcher );

    if(!d->currentItemToupdate) {
        kWarning() << "tried to add fetched item data to non existing item";
        return;
    }

    // copy the fetched meta data into the current item details
    d->currentItemToupdate->setMetaData(itemDetails);
    d->currentItemToupdate->setResourceType(resourceType);

    addResourceUriToMetaData( d->currentItemToupdate );

    saveMetaData( d->currentItemToupdate );

    // clear the pluginqueue so it will be initialized for the next item
    d->pluginqueue.clear();

    // delete the current item, we do not need it anymore
    if( !resourceExtractor()->resourcesList().isEmpty() ) {
        resourceExtractor()->takeNext(); // pop it off the list
        delete d->currentItemToupdate; // and delete it
        d->currentItemToupdate = 0;
    }

    // and start the next round
    searchNextItem();
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::errorInScriptExecution(const QString &error)
{
    kDebug() << error;

    // no need to call searchNextItem
    // next item will be used as even in error case the plugins return serachresults(just empty ones)
}

void NepomukMetaDataExtractor::UI::AutomaticFetcher::log(const QString &msg)
{
    kDebug() << msg;
}
