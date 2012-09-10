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

#include "batchextractor.h"

#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include "nepomukpipe/nepomukpipe.h"
#include "nepomukpipe/moviepipe.h"
#include "nepomukpipe/musicpipe.h"
#include "nepomukpipe/publicationpipe.h"
#include "nepomukpipe/tvshowpipe.h"

#include <KDE/KDebug>

#include <QtCore/QList>
#include <QtCore/QFutureWatcher>
#include <QtCore/QtConcurrentRun>

namespace NepomukMetaDataExtractor {
namespace UI {
    class BatchExtractorPrivate {
    public:
        NepomukMetaDataExtractor::Extractor::ExtractorFactory extractorFactory;
        NepomukMetaDataExtractor::Extractor::WebExtractor *currentExtractor;
        QList<NepomukMetaDataExtractor::UI::BatchExtractor::ExtractionJob> jobList;
        bool extractionInProgress;
    };
}
}

using namespace NepomukMetaDataExtractor::Pipe;
using namespace NepomukMetaDataExtractor::Extractor;
using namespace NepomukMetaDataExtractor::UI;

NepomukMetaDataExtractor::UI::BatchExtractor::BatchExtractor(QObject *parent)
    : QObject(parent)
    , d_ptr( new NepomukMetaDataExtractor::UI::BatchExtractorPrivate )
{
    Q_D( BatchExtractor );

    d->extractionInProgress = false;
    d->currentExtractor = 0;
}

QString NepomukMetaDataExtractor::UI::BatchExtractor::currentExtractionInfo() const
{
    Q_D( const BatchExtractor );

    QString currentJob;

    if( !d->jobList.isEmpty()) {
        currentJob = d->jobList.first().name;
    }

    return currentJob;
}

void NepomukMetaDataExtractor::UI::BatchExtractor::addJob(const QUrl &detailsUrl, const QVariantMap &options, const QString &name, const QString &resourceUri )
{
    Q_D( BatchExtractor );

    kDebug() << "addJob running?" << d->extractionInProgress;

    ExtractionJob newJob;
    newJob.detailsUrl = detailsUrl;
    newJob.options = options;
    newJob.name = name;
    newJob.resourceUri = resourceUri;

    d->jobList.append( newJob );

    emit extractionStarted( d->jobList.size() );

    if( !d->extractionInProgress ) {
        extractNext();
    }
}

void NepomukMetaDataExtractor::UI::BatchExtractor::extractNext()
{
    Q_D( BatchExtractor );

    ExtractionJob nextJob = d->jobList.first();

    if( d->currentExtractor ) {
        disconnect( d->currentExtractor, SIGNAL(error(QString)), this, SLOT(error(QString)) );
        disconnect( d->currentExtractor, SIGNAL(log(QString)), this, SLOT(log(QString)) );
        disconnect( d->currentExtractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(itemResults(QString,QVariantMap)) );
    }

    d->currentExtractor = d->extractorFactory.getExtractor(nextJob.detailsUrl);

    if( d->currentExtractor ) {
        d->extractionInProgress = true;
        connect( d->currentExtractor, SIGNAL(error(QString)), this, SLOT(error(QString)) );
        connect( d->currentExtractor, SIGNAL(log(QString)), this, SLOT(log(QString)) );

        connect( d->currentExtractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(itemResults(QString,QVariantMap)) );

        d->currentExtractor->extractItem( nextJob.detailsUrl, nextJob.options);
    }
    else {
        d->extractionInProgress = false;
        d->jobList.takeFirst();
        kWarning() << "could not get Webextractor for url " << nextJob.detailsUrl;
    }
}

void NepomukMetaDataExtractor::UI::BatchExtractor::log(const QString &msg)
{
    kDebug() << msg;
}

void NepomukMetaDataExtractor::UI::BatchExtractor::error(const QString &msg)
{
    kDebug() << msg;
}

static void concurrentPipe(const QString &resourceType, const QVariantMap &entry)
{
    // find the right nepomuk pipe and throw it in
    NepomukPipe *nepomukPipe = 0;

    if(resourceType == QLatin1String("publication")) {
        nepomukPipe = new PublicationPipe;
    }
    else if(resourceType == QLatin1String("tvshow")) {
        nepomukPipe = new TvShowPipe;
    }
    else if(resourceType == QLatin1String("movie")) {
        nepomukPipe = new MoviePipe;
    }
    else if(resourceType == QLatin1String("music")) {
        nepomukPipe = new MusicPipe;
    }

    if(nepomukPipe) {
        nepomukPipe->pipeImport( entry );
    }
    else {
        kWarning() << "No nepomuk pipe available for the resource type" << resourceType;
    }

    delete nepomukPipe;
}

void NepomukMetaDataExtractor::UI::BatchExtractor::itemResults(const QString &resourceType, const QVariantMap &entry)
{
    Q_D( BatchExtractor );

    ExtractionJob nextJob = d->jobList.first();

    QVariantMap metaData = entry;
    if( !nextJob.resourceUri.isEmpty() ) {
        addResourceUriToMetaData(resourceType, nextJob.resourceUri, metaData);
    }

    QFuture<void> future = QtConcurrent::run(concurrentPipe, resourceType, metaData);
    QFutureWatcher<void> *futureWatcher = new QFutureWatcher<void>();

    connect( futureWatcher, SIGNAL(finished()), this, SLOT(pipeFinished()) );

    futureWatcher->setFuture(future);
}

void NepomukMetaDataExtractor::UI::BatchExtractor::addResourceUriToMetaData( const QString &resourceType, const QString &uri, QVariantMap &metadata )
{
    // For tv shows put the resource uri into the Episode part of the MetaData
    // this way around it is possible to use the TvShowPipe with more episode/files at once
    if( resourceType == QLatin1String("tvshow")) {
        QVariantList seasons = metadata.value(QLatin1String("seasons")).toList();
        if(!seasons.isEmpty()) {
            QVariantMap season = seasons.takeFirst().toMap();
            QVariantList episodes = season.value(QLatin1String("episodes")).toList();

            if(!episodes.isEmpty()) {
                QVariantMap episodesMap = episodes.takeFirst().toMap();
                kDebug() << "add to episode" << episodesMap.value(QLatin1String("title")).toString() << "url" << uri;
                episodesMap.insert(QLatin1String("resourceuri"), uri);

                episodes << episodesMap;
                season.insert( QLatin1String("episodes"), episodes);
                seasons << season;
                metadata.insert( QLatin1String("seasons"), seasons);
            }
        }
    }
    // music piece / album works the same as tvshows, we add the fileurl to the track not the toplevel album
    else if( resourceType == QLatin1String("music")) {
        QVariantList trackList = metadata.value(QLatin1String("tracks")).toList();

        if(!trackList.isEmpty()) {
            QVariantMap trackMap = trackList.takeFirst().toMap();
            kDebug() << "add to track" << trackMap.value(QLatin1String("title")).toString() << "url" << uri;
            trackMap.insert(QLatin1String("resourceuri"), uri);

            trackList << trackMap;
            metadata.insert( QLatin1String("tracks"), trackList);
        }
    }

    // For all other cases put the resource uri in the top part of the MetaData
    // add this for all cases, helps to find the folder for the poster/cover download easier for music/tvshows)
    metadata.insert(QLatin1String("resourceuri"), uri);
}

void NepomukMetaDataExtractor::UI::BatchExtractor::pipeFinished()
{
    sender()->deleteLater(); // delete the created future watcher again

    Q_D( BatchExtractor );

    d->jobList.takeFirst();

    // parallel fetch from the web and pipe the last to nepomuk
    if( !d->jobList.isEmpty() ) {
        extractNext();
    }
    else {
        d->extractionInProgress = false;

        emit extractionFinished();
    }
}

