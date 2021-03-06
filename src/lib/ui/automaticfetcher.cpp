/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
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

#include <Nepomuk2/StoreResourcesJob>

#include <KDE/KDebug>

namespace NepomukWebMiner
{
namespace UI
{
class AutomaticFetcherPrivate
{
public:
    NepomukWebMiner::Extractor::WebExtractor *webextractor;
    NepomukWebMiner::Extractor::MetaDataParameters *currentItemToupdate;
    QList<KUrl> urlList;
    QList<NepomukWebMiner::Extractor::WebExtractor::Info> pluginqueue;
};
}
}

using namespace NepomukWebMiner;
using namespace Extractor;

NepomukWebMiner::UI::AutomaticFetcher::AutomaticFetcher(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukWebMiner::UI::AutomaticFetcherPrivate)
{
    Q_D(AutomaticFetcher);
    d->webextractor = 0;
    d->currentItemToupdate = 0;
}

NepomukWebMiner::UI::AutomaticFetcher::~AutomaticFetcher()
{
    Q_D(AutomaticFetcher);
    delete d->currentItemToupdate;
}

void NepomukWebMiner::UI::AutomaticFetcher::addFetcherUrl(const KUrl& url)
{
    Q_D(AutomaticFetcher);
    d->urlList.append(url);
}

void NepomukWebMiner::UI::AutomaticFetcher::startFetcher()
{
    kDebug() << "Start fetching meta data for" << resourceExtractor()->resourcesList().size() << "items";

    searchNextItem();
}

void NepomukWebMiner::UI::AutomaticFetcher::startUrlFetcher()
{
    Q_D(AutomaticFetcher);
    if (d->urlList.isEmpty()) {
        kWarning() << "started url fetcher without any urls";
        emit finished();
        return;
    }

    //TODO: implement downloading of more than one url, loop trough all urls in d->urllist

    // create a webextractor
    d->webextractor = extractorFactory()->getExtractor(d->urlList.first());

    if (!d->webextractor) {
        kWarning() << "could not find webextractor plugin for URL" << d->urlList.first();
    } else {
        d->currentItemToupdate = new MetaDataParameters;
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));

        QVariantMap options;
        options.insert(QString("references"), MDESettings::downloadReferences());
        options.insert(QString("banner"), MDESettings::downloadBanner());

        d->webextractor->extractItem(d->urlList.first(), options);
    }
}

void NepomukWebMiner::UI::AutomaticFetcher::searchNextItem()
{
    Q_D(AutomaticFetcher);
    if (resourceExtractor()->resourcesList().isEmpty()) {
        kDebug() << "Finished fetching all items";
        emit finished();
        return;
    }

    // don't takenext so we can try another plugin on this resource next
    d->currentItemToupdate = resourceExtractor()->resourcesList().first();

    // update kext:indexingLevel to 3
    // this ensures we are not going to check this file again in the
    // background servcie no matter if we found something or not
    updateIndexingLevel(d->currentItemToupdate->resourceUri(), 3);

    if (d->pluginqueue.isEmpty()) {
        d->pluginqueue.append(extractorFactory()->listAvailablePlugins(d->currentItemToupdate->resourceType()));
    }
    if (d->pluginqueue.isEmpty()) {
        kWarning() << "Could not get any plugins for the resourcetype :: " << d->currentItemToupdate->resourceType();
        return;
    }

    //TODO: maybe enum for resourcetype should be used...
    QString favPlugin;
    if (d->currentItemToupdate->resourceType() == QString("movie")) {
        favPlugin = MDESettings::favoriteMoviePlugin();
    }
    else if (d->currentItemToupdate->resourceType() == QString("tvshow")) {
        favPlugin = MDESettings::favoriteTvShowPlugin();
    }
    else if (d->currentItemToupdate->resourceType() == QString("publication")) {
        favPlugin = MDESettings::favoritePublicationPlugin();
    }
    else if (d->currentItemToupdate->resourceType() == QString("music")) {
        favPlugin = MDESettings::favoriteMusicPlugin();
    }

    Extractor::WebExtractor::Info selectedEngine;
    // if a fave is set we put it in the front of the queue, if it exists!
    if (!favPlugin.isEmpty()) {
        Extractor::WebExtractor::Info faveinfo;
        if (d->webextractor && d->webextractor->info().identifier == favPlugin) {
            faveinfo = d->webextractor->info();
        } else {
            faveinfo.identifier = favPlugin;
        }
        if (d->pluginqueue.contains(faveinfo)) {
            // if we do not want to check more than the fav plugin
            // clear list again and set only the fav plugin
            if (!MDESettings::checkNextPlugin()) {
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

    if (!d->webextractor || d->webextractor->info().identifier != selectedEngine.identifier) {

        if (d->webextractor) {
            disconnect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
            disconnect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
            disconnect(d->webextractor, SIGNAL(log(QString)), this, SLOT(log(QString)));
            disconnect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
        }

        d->webextractor = extractorFactory()->getExtractor(selectedEngine.identifier);

        connect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(log(QString)), this, SLOT(log(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert(QLatin1String("title"), d->currentItemToupdate->searchTitle());
    searchParameters.insert(QLatin1String("alttitle"), d->currentItemToupdate->searchAltTitle());
    searchParameters.insert(QLatin1String("author"), d->currentItemToupdate->searchPerson());
    searchParameters.insert(QLatin1String("season"), d->currentItemToupdate->searchSeason());
    searchParameters.insert(QLatin1String("episode"), d->currentItemToupdate->searchEpisode());
    searchParameters.insert(QLatin1String("yearMin"), d->currentItemToupdate->searchYearMin());
    searchParameters.insert(QLatin1String("yearMax"), d->currentItemToupdate->searchYearMax());
    searchParameters.insert(QLatin1String("journal"), d->currentItemToupdate->searchJournal());
    searchParameters.insert(QLatin1String("showtitle"), d->currentItemToupdate->searchShowTitle());
    searchParameters.insert(QLatin1String("album"), d->currentItemToupdate->searchAlbum());
    searchParameters.insert(QLatin1String("track"), d->currentItemToupdate->searchTrack());

    kDebug() << "Start searching with " << d->webextractor->info().name;
    //kDebug() << searchParameters;

    d->webextractor->search(d->currentItemToupdate->resourceType(), searchParameters);
}

void NepomukWebMiner::UI::AutomaticFetcher::selectSearchEntry(QVariantList searchResults)
{
    Q_D(AutomaticFetcher);

    if (searchResults.isEmpty()) {
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
    } else {

        // every result with a distance bigger than 10 will be removed from the list
        // this ensures the returned item are at least similar to the search result.
        // also this sorts the list and takes the best fit
        QVariantList sortedList = setLevenshteinDistance(searchResults, d->currentItemToupdate,10);
        if(sortedList.isEmpty()) {
            kDebug() << "Levenshtein distance to big for all" << searchResults.size() << "results";
            selectSearchEntry(QVariantList());
            return;
        }

        kDebug() << "Found " << sortedList.size() << "search results, taking the first entry";
        kDebug() << sortedList.first();

        QVariantMap selectedSearchResult = sortedList.first().toMap();

        KUrl fetchUrl(selectedSearchResult.value(QLatin1String("url")).toString());

        QVariantMap options;
        options.insert(QLatin1String("references"), MDESettings::downloadReferences());
        options.insert(QLatin1String("banner"), MDESettings::downloadBanner());

        d->webextractor->extractItem(fetchUrl, options);
    }
}

void NepomukWebMiner::UI::AutomaticFetcher::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    Q_D(AutomaticFetcher);

    if (!d->currentItemToupdate) {
        kWarning() << "tried to add fetched item data to non existing item";
        return;
    }

    // copy the fetched meta data into the current item details
    d->currentItemToupdate->setMetaData(itemDetails);
    d->currentItemToupdate->setResourceType(resourceType);

    addResourceUriToMetaData(d->currentItemToupdate);

    saveMetaData(d->currentItemToupdate);

    // clear the pluginqueue so it will be initialized for the next item
    d->pluginqueue.clear();


    // delete the current item, we do not need it anymore
    if (!resourceExtractor()->resourcesList().isEmpty()) {
        resourceExtractor()->takeNext(); // pop it off the list
        delete d->currentItemToupdate; // and delete it
        d->currentItemToupdate = 0;
    }

    // and start the next round
    searchNextItem();
}

void NepomukWebMiner::UI::AutomaticFetcher::errorInScriptExecution(const QString &error)
{
    kDebug() << error;

    // no need to call searchNextItem
    // next item will be used as even in error case the plugins return serachresults(just empty ones)
}

void NepomukWebMiner::UI::AutomaticFetcher::log(const QString &msg)
{
    kDebug() << msg;
}
