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

#include "webminerindexingqueue.h"

#include "webminerindexingjob.h"
#include "mdesettings.h"

#include <Nepomuk2/ResourceManager>
#include <Soprano/Model>
#include <Soprano/Node>
#include <Soprano/QueryResultIterator>

#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <KDE/KDebug>

#include <QtCore/QTimer>
#include <QtCore/QFileInfo>

WebMinerIndexingQueue::WebMinerIndexingQueue(QObject *parent)
    : IndexingQueue(parent)
{
    m_fileQueue.reserve( 10 );

    QTimer::singleShot( 0, this, SLOT(init()) );
}

void WebMinerIndexingQueue::init()
{
    fillQueue();
    emit startedIndexing();

    callForNextIteration();
}

void WebMinerIndexingQueue::fillQueue()
{
    /* prevent abuse this API */
    if (m_fileQueue.size() > 0) {
        return;
    }

    QStringList mimeSelection;
    if( MDESettings::documentServiceEnabled() ) {
        mimeSelection << QLatin1String("pdf");
    }
    if( MDESettings::musicServiceEnabled() ) {
        mimeSelection << QLatin1String("audio");
    }
    if( MDESettings::videoServiceEnabled() ) {
        mimeSelection << QLatin1String("video");
    }

    if( mimeSelection.isEmpty() ) {
        kDebug() << "no mimetype selected for the background service";
        return;
    }

    QString whiteListRegExp;
    if( MDESettings::useWhiteList() ) {
        whiteListRegExp.append( QLatin1String("Filter (") );

        foreach(const QString &folder, MDESettings::whiteList()) {
            QString regexp = QString( "regex(?url , \"%1\", \"i\") || " ).arg( Soprano::Node::resourceToN3(folder) );
            whiteListRegExp.append( regexp );
        }

        whiteListRegExp.chop(2);
        whiteListRegExp.append( QLatin1String(" ).") );
    }

    QString query = QString("select ?url where { ?r nie:url ?url ; kext:indexingLevel ?l ; nie:mimeType ?mime"
                            " Filter regex(?mime , \"%1\", \"i\") . %2"
                            " FILTER(?l = 2  ). } LIMIT 10")
                            .arg( mimeSelection.join(QLatin1String("|")) )
                            .arg( whiteListRegExp );

    Soprano::Model* model = Nepomuk2::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    while( it.next() ) {
        //only add urls of files that exist
        //as we can't save metadata for localfiles that are on an external drive when the
        //drive is not connected this would fail anyway
        //TODO: external harddrives queue should be solved differently.
        //      otherwise even if we have resources on local drive this queue will never fill
        //      until all resources on the external drive is checked again.
        //      maybe have a list of external drives, and change the SPARQL query so it only
        //      looks for path that are currently available?
        QFileInfo fi( it[0].uri().toLocalFile() );
        if(fi.exists()) {
            m_fileQueue.enqueue( it[0].uri() );
        }
        else {
            kDebug() << fi.absoluteFilePath() << "does not exist";
        }
    }
}

void WebMinerIndexingQueue::fillQueue(const QUrl &selection)
{
    // resume the webminingqueue as we force indexing, no matter what
    resumeNoAction();

    // if the url is a simple fle, just add it to the queue
    // if we have a folder, add all files in it to the queue
    // for eac hsubfodler call fillQueue(selection) recursive
    QDir dir(selection.toLocalFile());
    QUrl fileurl(selection.toLocalFile());

    if (dir.exists()) {
        QFileInfoList list = dir.entryInfoList();
        foreach (const QFileInfo & fileInfo, list) {
            if(isSuspended()) { // if we suspended the service in the meatime, we break here
                break;        // in case we index a full usb drive this will run for a long time otherwise
                              // before this would freally suspend the service
            }
            if (fileInfo.fileName() == QString(".") || fileInfo.fileName() == QString("..")) {
                continue;
            }
            QUrl url(fileInfo.absoluteFilePath());
            if (fileInfo.isDir()) {
                fillQueue(url);
            } else {
                kDebug() << "add file" << url;
                m_fileQueue.enqueue(url);
            }
        }
    }
    else  {
        kDebug() << "add file" << fileurl;
        m_fileQueue.enqueue(fileurl);
    }

    emit startedIndexing();

    callForNextIteration();
}

bool WebMinerIndexingQueue::isEmpty()
{
    return m_fileQueue.isEmpty();
}

void WebMinerIndexingQueue::processNextIteration()
{
    const QUrl fileUrl = m_fileQueue.dequeue();
    process( fileUrl );
}

void WebMinerIndexingQueue::process(const QUrl& url)
{
    m_currentUrl = url;

    KJob* job = new WebMinerIndexingJob( QFileInfo(url.toLocalFile()) );
    job->start();
    emit beginIndexingFile( url );
    connect( job, SIGNAL(finished(KJob*)), this, SLOT(slotFinishedIndexingFile(KJob*)) );
}

void WebMinerIndexingQueue::slotFinishedIndexingFile(KJob* job)
{
    if( job->error() ) {
        kDebug() << job->errorString();
    }

    QUrl url = m_currentUrl;
    m_currentUrl.clear();
    emit endIndexingFile( url );
    if( m_fileQueue.isEmpty() ) {
        fillQueue();
    }
    finishIteration();
}

void WebMinerIndexingQueue::clear()
{
    m_currentUrl.clear();
    m_fileQueue.clear();
}

QUrl WebMinerIndexingQueue::currentUrl()
{
    return m_currentUrl;
}
