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

#include "searchmicrosoftacademic.h"


#include "../publicationentry.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <KDE/KDebug>

SearchMicrosoftAcademic::SearchMicrosoftAcademic(QObject *parent) :
    QObject(parent)
{
    m_maid = 0;
    m_citations = -1;
    bibtexFileReceived = false;

    m_initialQuerymanager = new QNetworkAccessManager(this);
//    m_detailQuerymanager = new QNetworkAccessManager(this);
    m_bibtexQuerymanager = new QNetworkAccessManager(this);
    m_citationQuerymanager = new QNetworkAccessManager(this);
    m_citationBibTeXQuerymanager = new QNetworkAccessManager(this);

    connect(m_initialQuerymanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(searchReplyFinished(QNetworkReply*)));
//    connect(m_detailQuerymanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(detailReplyFinished(QNetworkReply*)));
    connect(m_bibtexQuerymanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(bibtexReplyFinished(QNetworkReply*)));
    connect(m_citationQuerymanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(citationReplyFinished(QNetworkReply*)));
    connect(m_citationBibTeXQuerymanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(citationBibtexReplyFinished(QNetworkReply*)));
}

void SearchMicrosoftAcademic::setPublicationEntry(PublicationEntry *entryToQuery)
{
    m_entryToQuery = entryToQuery;
}

PublicationEntry *SearchMicrosoftAcademic::publicationEntry()
{
    return m_entryToQuery;
}

void SearchMicrosoftAcademic::startSearch()
{
    QString query = QString("http://academic.research.microsoft.com/Search?query=%1").arg("\"" + m_entryToQuery->dataMap.value(QLatin1String("title")) + "\"");
    m_initialQuerymanager->get(QNetworkRequest( QUrl(query) ));
}

void SearchMicrosoftAcademic::searchReplyFinished(QNetworkReply *reply)
{
    QString result(reply->readAll());

    // first check for all the publication detail links
    QRegExp rx("Publication/(\\d+)/");
    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(result, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    kDebug() << "found " << list.size() << "results for the entry" << m_entryToQuery->dataMap.value(QLatin1String("title"));

    if( list.size() == 0) {
        if( !m_altTitleSearchStarted) {
            m_altTitleSearchStarted = true;
            QString query = QString("http://academic.research.microsoft.com/Search?query=%1").arg("\"" + m_entryToQuery->dataMap.value(QLatin1String("alttitle")) + "\"");
            m_initialQuerymanager->get(QNetworkRequest( QUrl(query) ));
        }
        else {
            emit searchFinished();
        }
    }

    // based on the parsed id, we download the bibtexFile and the citations
    foreach(const QString &id, list) {
        m_maid = id.toInt();
        // gets the abstarct from the details
//        QString queryDetails = QString("http://academic.research.microsoft.com/Publication/%1").arg(id);
//        m_detailQuerymanager->get(QNetworkRequest( QUrl(queryDetails) ));
        // returns the bibtexFile
        QString queryBibTeX = QString("http://academic.research.microsoft.com/%1.bib?type=2&format=0&download=1").arg(id);
        m_bibtexQuerymanager->get(QNetworkRequest( QUrl(queryBibTeX) ));

        // gets a list of all citations
        QString queryCitation = QString("http://academic.research.microsoft.com/Detail?entitytype=1&searchtype=2&id=%1&start0&end=100").arg(id);
        m_citationQuerymanager->get(QNetworkRequest( QUrl(queryCitation) ));

        break; // stick to first result for now
    }
}

//void SearchMicrosoftAcademic::detailReplyFinished(QNetworkReply *reply)
//{
//    qDebug() << "detailReplyFinished";

//    QString result(reply->readAll());
//}

void SearchMicrosoftAcademic::bibtexReplyFinished(QNetworkReply *reply)
{
    qDebug() << "main bibtexReplyFinished";

    QTextStream in(reply->readAll());
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        QRegExp rx("@(\\w+)\\{");
        int pos = 0;

        while ((pos = rx.indexIn(line, pos)) != -1) {
            m_entryToQuery->type = rx.cap(1);
            pos += rx.matchedLength();
        }

        QRegExp rx2("^(\\w+)\\s=\\s\\{(.*)\\},?$");
        int pos2 = 0;

        while ((pos = rx2.indexIn(line, pos2)) != -1) {
            m_entryToQuery->dataMap.insert(rx2.cap(1), rx2.cap(2));
            pos2 += rx2.matchedLength();
        }
    }

    bibtexFileReceived = true;

    if(m_citations != -1) {
        emit searchFinished();
    }
}

void SearchMicrosoftAcademic::citationReplyFinished(QNetworkReply *reply)
{
    qDebug() << "citationReplyFinished";

    QString result(reply->readAll());

    // first check for all the publication detail links
    QRegExp rx("Publication/(\\d+)/");
    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(result, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    m_citations = list.size();
    if( list.isEmpty() && bibtexFileReceived) {
        qDebug() << "no citations found";
        emit searchFinished();
        return;
    }

    qDebug() << "found " << m_citations << "citations";

    // based on the parsed if,
    foreach(const QString &id, list) {
        m_maid = id.toInt();
        // gets the abstarct from the details
//        QString queryDetails = QString("http://academic.research.microsoft.com/Publication/%1").arg(id);
//        m_detailQuerymanager->get(QNetworkRequest( QUrl(queryDetails) ));
        // returns the bibtexFile
        QString queryBibTeX = QString("http://academic.research.microsoft.com/%1.bib?type=2&format=0&download=1").arg(id);
        m_citationBibTeXQuerymanager->get(QNetworkRequest( QUrl(queryBibTeX) ));

        // gets a list of all citations .... don't get citations of citations
//        QString queryCitation = QString("http://academic.research.microsoft.com/Detail?entitytype=1&searchtype=2&id=%1").arg(id);
//        m_citationQuerymanager->get(QNetworkRequest( QUrl(queryCitation) ));
    }
}

void SearchMicrosoftAcademic::citationBibtexReplyFinished(QNetworkReply *reply)
{
    qDebug() << "sub citationBibtexReplyFinished";

    PublicationEntry *newCitation = new PublicationEntry;
    m_entryToQuery->citations.append(newCitation);

    QTextStream in(reply->readAll());
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        QRegExp rx("@(\\w+)\\{");
        int pos = 0;

        while ((pos = rx.indexIn(line, pos)) != -1) {
            newCitation->type = rx.cap(1);
            pos += rx.matchedLength();
        }

        QRegExp rx2("^(\\w+)\\s=\\s\\{(.*)\\},?$");
        int pos2 = 0;

        while ((pos = rx2.indexIn(line, pos2)) != -1) {
            newCitation->dataMap.insert(rx2.cap(1), rx2.cap(2));
            pos2 += rx2.matchedLength();
        }
    }

    m_citations--;

    if(m_citations == 0 && bibtexFileReceived) {
        emit searchFinished();
    }
}
