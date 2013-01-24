/* This file is part of the KDE Project
   Copyright (c) 2008-2010 Sebastian Trueg <trueg@kde.org>
   Copyright (c) 2012 Vishesh Handa <me@vhanda.in>
   Copyright (c) 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>

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

#ifndef _NEPOMUK_FILEINDEXER_INDEX_SCHEDULER_H_
#define _NEPOMUK_FILEINDEXER_INDEX_SCHEDULER_H_

#include <QtCore/QObject>

#include <QtCore/QString>
#include <QtCore/QUrl>

class IndexSchedulerPrivate;
class WebMinerIndexingQueue;
class EventMonitor;

/**
  * @brief The IndexScheduler is responsible for controlling the indexing queue and reacting to events.
  *
  * It contains an EventMonitor and listens for events such as power management, battery,
  * disk space and network availability
  */
class IndexScheduler : public QObject
{
    Q_OBJECT

public:
    IndexScheduler( QObject* parent=0 );
    ~IndexScheduler();

    bool isSuspended() const;
    bool isIndexing() const;

    /**
      * @brief A user readable description of the scheduler's status
      */
    QString userStatusString() const;

    /**
      * @brief The current uri being indexed. It is empty if no file is being indexed.
      *
      * The url being empty does not indicate that the indexer isn't running,
      * just that it hasn't found a file to index.
      *
      * @sa indexingStarted
      * @sa indexingStopped
      */
    QUrl currentUrl() const;

public Q_SLOTS:
    void suspend();
    void resume();

    void setSuspended( bool );

    /**
      * @brief Updates all configured folders.
      */
    void updateAll( bool forceUpdate = false );

    // Event Monitor integration
    void slotScheduleIndexing();

Q_SIGNALS:
    // Indexing State
    void indexingStarted();
    void indexingStopped();
    void indexingStateChanged( bool indexing );

    // Finer Index state
    void indexingFile( const QString & );

    // Emitted on calling suspend/resume
    void indexingSuspended( bool suspended );

    void statusStringChanged();

private Q_SLOTS:
    void slotConfigChanged();

    void slotBeginIndexingFile(const QUrl& url);
    void slotEndIndexingFile(const QUrl& url);

    void slotStartedIndexing();
    void slotFinishedIndexing();

private:
    // emits indexingStarted or indexingStopped based on parameter. Makes sure
    // no signal is emitted twice
    void setIndexingStarted( bool started );

    Q_DECLARE_PRIVATE(IndexScheduler)
    IndexSchedulerPrivate *const d_ptr; /**< d-pointer for this class */
};


#endif

