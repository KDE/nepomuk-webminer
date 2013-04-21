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

#include <KDE/KJob>
#include <QtCore/QScopedPointer>
#include <Nepomuk2/ResourceManager>
#include <Nepomuk2/StoreResourcesJob>
#include "kext.h"

#include <KDE/KDebug>

#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Vocabulary/RDF>

#include <Nepomuk2/File>
#include <Nepomuk2/Variant>

#include <Solid/Networking>

namespace NepomukWebMiner
{
namespace UI
{
class FetcherPrivate
{
public:
    Extractor::ResourceExtractor *m_re;
    Extractor::ExtractorFactory *m_ef;
    bool m_cancel;
};
}
}

NepomukWebMiner::UI::Fetcher::Fetcher()
    : d_ptr(new NepomukWebMiner::UI::FetcherPrivate)

{
    Q_D(Fetcher);
    d->m_re = new Extractor::ResourceExtractor;
    d->m_ef = new Extractor::ExtractorFactory;
    d->m_cancel = false;
}

NepomukWebMiner::UI::Fetcher::~Fetcher()
{
    Q_D(Fetcher);
    delete d->m_re;
    delete d->m_ef;
}

void NepomukWebMiner::UI::Fetcher::addFetcherPath(const KUrl &url)
{
    Q_D(Fetcher);
    d->m_re->lookupFiles(url);
}

void NepomukWebMiner::UI::Fetcher::addFetcherResource(const KUrl &url)
{
    Nepomuk2::Resource r = Nepomuk2::Resource::fromResourceUri(url);

    if (r.exists()) {
        addFetcherResource(r);
    }
}

void NepomukWebMiner::UI::Fetcher::addFetcherResource(const Nepomuk2::Resource &resource)
{
    Q_D(Fetcher);
    d->m_re->lookupResource(resource);
}

void NepomukWebMiner::UI::Fetcher::addFetcherResource(const QList<Nepomuk2::Resource> &resources)
{
    Q_D(Fetcher);
    d->m_re->lookupResource(resources);
}

void NepomukWebMiner::UI::Fetcher::setForceUpdate(bool update)
{
    Q_D(Fetcher);
    d->m_re->setForceUpdate(update);
}

void NepomukWebMiner::UI::Fetcher::setTvShowMode(bool tvshowMode)
{
    Q_D(Fetcher);
    d->m_re->setTvShowMode(tvshowMode);
}

void NepomukWebMiner::UI::Fetcher::setMovieMode(bool movieMode)
{
    Q_D(Fetcher);
    d->m_re->setMovieMode(movieMode);
}

NepomukWebMiner::Extractor::ResourceExtractor *NepomukWebMiner::UI::Fetcher::resourceExtractor()
{
    Q_D(Fetcher);
    return d->m_re;
}

NepomukWebMiner::Extractor::ExtractorFactory* NepomukWebMiner::UI::Fetcher::extractorFactory()
{
    Q_D(Fetcher);
    return d->m_ef;
}

void NepomukWebMiner::UI::Fetcher::addResourceUriToMetaData(NepomukWebMiner::Extractor::MetaDataParameters *mdp)
{
    // For tv shows put the resource uri into the Episode part of the MetaData
    // this way around it is possible to use the TvShowPipe with more episodes/files at once
    if (mdp->resourceType() == QLatin1String("tvshow")) {
        QVariantList seasons = mdp->metaData().value(QLatin1String("seasons")).toList();
        if (!seasons.isEmpty()) {
            QVariantMap season = seasons.takeFirst().toMap();
            QVariantList episodes = season.value(QLatin1String("episodes")).toList();

            if (!episodes.isEmpty()) {
                QVariantMap episodesMap = episodes.takeFirst().toMap();
                kDebug() << "add to episode" << episodesMap.value(QLatin1String("title")).toString() << "url" << mdp->resourceUri().url();
                episodesMap.insert(QLatin1String("resourceuri"), mdp->resourceUri().url());

                episodes << episodesMap;
                season.insert(QLatin1String("episodes"), episodes);
                seasons << season;
                QVariantMap savedMetaData = mdp->metaData();
                savedMetaData.insert(QLatin1String("seasons"), seasons);
                mdp->setMetaData(savedMetaData);
            }
        }
    }
    // music piece / album works the same as tvshows, we add the fileurl to the track not the toplevel album
    else if (mdp->resourceType() == QLatin1String("music")) {
        QVariantList trackList = mdp->metaData().value(QLatin1String("tracks")).toList();

        if (!trackList.isEmpty()) {
            QVariantMap trackMap = trackList.takeFirst().toMap();
            kDebug() << "add to track" << trackMap.value(QLatin1String("title")).toString() << "url" << mdp->resourceUri().url();
            trackMap.insert(QLatin1String("resourceuri"), mdp->resourceUri().url());

            trackList << trackMap;
            QVariantMap savedMetaData = mdp->metaData();
            savedMetaData.insert(QLatin1String("tracks"), trackList);
            mdp->setMetaData(savedMetaData);
        }
    }

    // For all other cases put the resource uri in the top part of the MetaData
    // add this for all cases, helps to find the folder for the poster/cover download easier for music/tvshows)
    QVariantMap savedMetaData = mdp->metaData();
    savedMetaData.insert(QLatin1String("resourceuri"), mdp->resourceUri().url());
    mdp->setMetaData(savedMetaData);
}

void NepomukWebMiner::UI::Fetcher::saveMetaData(NepomukWebMiner::Extractor::MetaDataParameters *mdp) const
{
    QString type = mdp->resourceType();

    Pipe::NepomukPipe *nepomukPipe = 0;

    if (type == QLatin1String("publication")) {
        nepomukPipe = new Pipe::PublicationPipe;
    } else if (type == QLatin1String("tvshow")) {
        nepomukPipe = new Pipe::TvShowPipe;
    } else if (type == QLatin1String("movie")) {
        nepomukPipe = new Pipe::MoviePipe;
    } else if (type == QLatin1String("music")) {
        nepomukPipe = new Pipe::MusicPipe;
    }

    if (nepomukPipe) {
        nepomukPipe->import(mdp->metaData());
    } else {
        kWarning() << "No nepomuk pipe available for the resource type" << type;
    }

    delete nepomukPipe;
}

bool distanceLessThan(const QVariant &l1, const QVariant &l2)
{
    QVariantMap lm1 = l1.toMap();
    QVariantMap lm2 = l2.toMap();
    uint d1 = lm1.value(QLatin1String("stringdistance"),500).toUInt();
    uint d2 = lm2.value(QLatin1String("stringdistance"),500).toUInt();

    return d1 < d2;
}

QVariantList NepomukWebMiner::UI::Fetcher::setLevenshteinDistance(const QVariantList &searchResults, NepomukWebMiner::Extractor::MetaDataParameters *currentItem, uint skipValue)
{
    QString type = currentItem->resourceType();
    QString checkString;
    if (type == QLatin1String("publication")) {
        // check only the title
        checkString = currentItem->searchTitle();

    } else if (type == QLatin1String("tvshow")) {
        // check tvshow + Season Number + Episode Number
        checkString = currentItem->searchShowTitle();
        checkString.append(currentItem->searchSeason());
        checkString.append(currentItem->searchEpisode());

    } else if (type == QLatin1String("movie")) {
        // check movie name only
        checkString = currentItem->searchTitle();

    } else if (type == QLatin1String("music")) {
        // check artist + track name
        checkString = currentItem->searchPerson();
        checkString.append(currentItem->searchTitle());
    }

    checkString.remove(QChar(' '));

    QVariantList sortedList;

    foreach(const QVariant &v, searchResults) {

        QVariantMap vMap = v.toMap();
        QString resultString = vMap.value(QLatin1String("distanceString")).toString();

        resultString.remove(QChar(' '));

        uint distance = levenshteinDistance(checkString, resultString);

        if(distance <= skipValue) {
            vMap.insert(QLatin1String("stringdistance"),distance);
            sortedList.append(vMap);
        }
    }

     qSort(sortedList.begin(), sortedList.end(), distanceLessThan);

     return sortedList;
}

uint NepomukWebMiner::UI::Fetcher::levenshteinDistance(const QString &s1, const QString & s2)
{
    const uint len1 = s1.size();
    const uint len2 = s2.size();
    QVector<uint> col(len2+1);
    QVector<uint> prevCol(len2+1);

    for (uint i = 0; i < (uint)prevCol.size(); i++)
        prevCol[i] = i;
    for (uint i = 0; i < len1; i++) {
        col[0] = i+1;
        for (uint j = 0; j < len2; j++)
            col[j+1] = qMin( qMin( 1 + col[j], 1 + prevCol[1 + j]),
                            prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
        col.swap(prevCol);
    }
    return prevCol[len2];
}

//
// We don't really care if the indexing level is in the incorrect graph
//
void NepomukWebMiner::UI::Fetcher::updateIndexingLevel(const QUrl& url, int level)
{
    // do not set kext:indexingLevel == 3 if no internet connection was available
    // otherwise no matter if something was found or not, set the level to 3
    if( Solid::Networking::status() != Solid::Networking::Connected) {
        kDebug() << "ignore updateIndexingLevel(3) due to missing network connection";
        return;
    }

    //first get the nepomuk uri from the file url

    QString query = QString::fromLatin1("select ?r where { ?r nie:url %1 }").arg( Soprano::Node::resourceToN3( url ) );
    Soprano::Model* model = Nepomuk2::ResourceManager::instance()->mainModel();

    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparqlNoInference );

    QUrl uri;
    if( it.next() ) {
        uri = it[0].uri();
    }
    else {
        kError() << "could not update indexing level for the file" << url << " via: " << query;
        return;
    }

    // now update the indexing level
    QString uriN3 = Soprano::Node::resourceToN3( uri );

    query = QString::fromLatin1("select ?g ?l where { graph ?g { %1 kext:indexingLevel ?l . } }")
                    .arg ( uriN3 );
    it = model->executeQuery( query, Soprano::Query::QueryLanguageSparqlNoInference );

    QUrl graph;
    Soprano::Node prevLevel;
    if( it.next() ) {
        graph = it[0].uri();
        prevLevel = it[1];
        it.close();
    }

    if( !graph.isEmpty() ) {
        QString graphN3 = Soprano::Node::resourceToN3( graph );
        QString removeCommand = QString::fromLatin1("sparql delete { graph %1 { %2 kext:indexingLevel %3 . } }")
                                .arg( graphN3, uriN3, prevLevel.toN3() );
        model->executeQuery( removeCommand, Soprano::Query::QueryLanguageUser, QLatin1String("sql") );

        QString insertCommand = QString::fromLatin1("sparql insert { graph %1 { %2 kext:indexingLevel %3 . } }")
                                .arg( graphN3, uriN3, Soprano::Node::literalToN3(level) );
        model->executeQuery( insertCommand, Soprano::Query::QueryLanguageUser, QLatin1String("sql") );
    }
    // Practically, this should never happen, but still
    else {
        QScopedPointer<KJob> job( Nepomuk2::setProperty( QList<QUrl>() << uri, Nepomuk2::Vocabulary::KExt::indexingLevel(),
                                                                QVariantList() << QVariant(level) ) );
        job->setAutoDelete(false);
        job->exec();
    }
}
