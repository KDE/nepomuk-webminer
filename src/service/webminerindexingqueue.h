/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Jörg Ehrichs <joerg.ehrichs@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef WEBMINERINDEXINGQUEUE_H
#define WEBMINERINDEXINGQUEUE_H

#include "indexingqueue.h"

#include <KJob>
#include <Soprano/QueryResultIterator>

class WebMinerIndexingQueue : public Nepomuk2::IndexingQueue
{
    Q_OBJECT
public:
    explicit WebMinerIndexingQueue(QObject *parent = 0);
    virtual bool isEmpty();
    virtual void fillQueue();
    virtual void fillQueue(const QUrl &selection);

    void clear();
    QUrl currentUrl();

signals:
    void beginIndexingFile(const QUrl& url);
    void endIndexingFile(const QUrl& url);

protected:
    virtual void processNextIteration();

private slots:
    void init();
    void slotFinishedIndexingFile(KJob* job);

private:
    void process(const QUrl& url);

    QQueue<QUrl> m_fileQueue;
    QUrl m_currentUrl;

};

#endif // WEBMINERINDEXINGQUEUE_H
