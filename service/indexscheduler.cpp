/* This file is part of the KDE Project
   Copyright (c) 2008-2010 Sebastian Trueg <trueg@kde.org>
   Copyright (c) 2010-12 Vishesh Handa <handa.vish@gmail.com>
   Copyright (c) 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>

   Parts of this file are based on code from Strigi
   Copyright (C) 2006-2007 Jos van den Oever <jos@vandenoever.info>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "indexscheduler.h"
#include "webminerindexingqueue.h"
#include "eventmonitor.h"

#include <KDebug>
#include <KDE/KUrl>
#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <KDE/KStandardDirs>
#include <KDE/KLocale>

class IndexSchedulerPrivate
{
public:
    bool indexing;

    // Queues
    WebMinerIndexingQueue *webIQ;

    Nepomuk2::EventMonitor* eventMonitor;

    enum State {
        State_Normal,
        State_OnBattery,
        State_UserIdle,
        State_LowDiskSpace,
        State_NetworkDisabled,
        State_Suspended
    };

    State state;

    bool shouldSuspendFileIQOnNormal;
};

IndexScheduler::IndexScheduler( QObject* parent )
    : QObject( parent )
    , d_ptr(new IndexSchedulerPrivate)
{
    Q_D(IndexScheduler);

    d->indexing = false;

    // remove old indexing error log
    KConfig config( "nepomukstrigirc" );
    KConfigGroup cfg = config.group( "General" );
    if(cfg.group( "General" ).readEntry( "debug mode", false )) {
        QFile::remove(KStandardDirs::locateLocal("data", QLatin1String("nepomuk/webminer-indexer-error-log")));
    }

    d->webIQ = new WebMinerIndexingQueue( this );

    connect( d->webIQ, SIGNAL(beginIndexingFile(QUrl)), this, SLOT(slotBeginIndexingFile(QUrl)) );
    connect( d->webIQ, SIGNAL(endIndexingFile(QUrl)), this, SLOT(slotEndIndexingFile(QUrl)) );

    connect( d->webIQ, SIGNAL(startedIndexing()), this, SLOT(slotStartedIndexing()) );
    connect( d->webIQ, SIGNAL(finishedIndexing()), this, SLOT(slotFinishedIndexing()) );

    connect( d->webIQ, SIGNAL(startedIndexing()), this, SIGNAL(statusStringChanged()) );
    connect( d->webIQ, SIGNAL(beginIndexingFile(QUrl)), this, SIGNAL(statusStringChanged()) );
    connect( d->webIQ, SIGNAL(finishedIndexing()), this, SIGNAL(statusStringChanged()) );

    d->eventMonitor = new Nepomuk2::EventMonitor( this );
    connect( d->eventMonitor, SIGNAL(diskSpaceStatusChanged(bool)), this, SLOT(slotScheduleIndexing()) );
    connect( d->eventMonitor, SIGNAL(idleStatusChanged(bool)), this, SLOT(slotScheduleIndexing()) );
    connect( d->eventMonitor, SIGNAL(powerManagementStatusChanged(bool)), this, SLOT(slotScheduleIndexing()) );

    // Special settings for the queues
    // reuse settings for the fileindexer here, no need to specify this twice

    cfg = config.group( "Indexing" );

    int fileIQDelay = cfg.readEntry<int>( "FileIQDelay", 0 );
    d->webIQ->setDelay( fileIQDelay );

    QString value = cfg.readEntry<QString>( "NormalMode_FileIndexing", "suspend" );
    if( value == "suspend" ) {
        d->shouldSuspendFileIQOnNormal = true;
    }
    else if( value == "resume" ) {
        d->shouldSuspendFileIQOnNormal = false;
    }

    slotScheduleIndexing();
}

IndexScheduler::~IndexScheduler()
{
    Q_D(IndexScheduler);

    delete d->webIQ;
    delete d->eventMonitor;
}

void IndexScheduler::suspend()
{
    Q_D(IndexScheduler);

    if ( d->state != IndexSchedulerPrivate::State_Suspended ) {
        d->state = IndexSchedulerPrivate::State_Suspended;
        slotScheduleIndexing();

        d->eventMonitor->disable();

        emit indexingSuspended( true );
    }
}

void IndexScheduler::resume()
{
    Q_D(IndexScheduler);

    if( d->state == IndexSchedulerPrivate::State_Suspended ) {
        d->state = IndexSchedulerPrivate::State_Normal;
        slotScheduleIndexing();

        d->eventMonitor->enable();

        emit indexingSuspended( false );
    }
}

void IndexScheduler::setSuspended( bool suspended )
{
    if ( suspended )
        suspend();
    else
        resume();
}

bool IndexScheduler::isSuspended() const
{
    Q_D(const IndexScheduler);

    return d->state == IndexSchedulerPrivate::State_Suspended;
}

bool IndexScheduler::isIndexing() const
{
    Q_D(const IndexScheduler);

    return d->indexing;
}

QUrl IndexScheduler::currentUrl() const
{
    Q_D(const IndexScheduler);

    return d->webIQ->currentUrl();
}

void IndexScheduler::setIndexingStarted( bool started )
{
    Q_D(IndexScheduler);

    if ( started != d->indexing ) {
        d->indexing = started;
        emit indexingStateChanged( d->indexing );
        if ( d->indexing ) {
            emit indexingStarted();
        }
        else {
            emit indexingStopped();
        }
    }
}

void IndexScheduler::slotStartedIndexing()
{
    Q_D(IndexScheduler);

    d->eventMonitor->enable();
}

void IndexScheduler::slotFinishedIndexing()
{
    Q_D(IndexScheduler);

    d->eventMonitor->suspendDiskSpaceMonitor();
}

void IndexScheduler::updateAll( bool forceUpdate )
{
    if( forceUpdate ) {
        //TODO: remove kext:indexingLevel=3 and set them to 2 again
        kDebug() << "replace kext:indexingLevel 3 with 2";

    }

    // fill the webminer queue
    slotConfigChanged();
}

void IndexScheduler::slotConfigChanged()
{
    Q_D(IndexScheduler);

    d->webIQ->clear();

    slotScheduleIndexing();
}

void IndexScheduler::slotBeginIndexingFile(const QUrl& url)
{
    setIndexingStarted( true );

    QString path = url.toLocalFile();
    emit indexingFile( path );
}

void IndexScheduler::slotEndIndexingFile(const QUrl&)
{
    Q_D(IndexScheduler);

    const QUrl webUrl = d->webIQ->currentUrl();

    if( webUrl.isEmpty() ) {
        setIndexingStarted( false );
    }
}

void IndexScheduler::slotScheduleIndexing()
{
    Q_D(IndexScheduler);

    if( d->state == IndexSchedulerPrivate::State_Suspended ) {
        emit statusStringChanged();
        return;
    }

    if( d->eventMonitor->isDiskSpaceLow() ) {
        kDebug() << "Disk Space Low";
        d->state = IndexSchedulerPrivate::State_LowDiskSpace;

        d->webIQ->suspend();
    }

    else if( d->eventMonitor->isOnBattery() ) {
        kDebug() << "Battery mode";
        d->state = IndexSchedulerPrivate::State_OnBattery;

        d->webIQ->suspend();
    }

    else if( d->eventMonitor->isNetworkDisabled() ) {
        kDebug() << "Network disabled";
        d->state = IndexSchedulerPrivate::State_NetworkDisabled;

        d->webIQ->suspend();
    }

    else if( d->eventMonitor->isIdle() ) {
        kDebug() << "Idle";
        d->state = IndexSchedulerPrivate::State_UserIdle;

        d->webIQ->resume();
    }

    else {
        kDebug() << "Normal indexing mode";
        d->state = IndexSchedulerPrivate::State_Normal;

        if( d->shouldSuspendFileIQOnNormal ) {
            kDebug() << "suspend on normal";
            d->webIQ->suspend();
        }
        else {
            kDebug() << "index on normal";
            d->webIQ->resume();
        }
    }

    emit statusStringChanged();
}

QString IndexScheduler::userStatusString() const
{
    bool indexing = isIndexing();
    bool suspended = isSuspended();

    if ( suspended ) {
        return i18nc( "@info:status", "WebMiner service is suspended." );
    }
    else if ( indexing ) {
        QUrl url = currentUrl();

        if( url.isEmpty() ) {
            return i18nc( "@info:status", "Fetching metadata from the web." );
        }
        else {
            return i18nc( "@info:status", "Indexing %1", url.toLocalFile() );
        }
    }
    else {
        return i18nc( "@info:status", "WebMiner service is idle." );
    }
}

