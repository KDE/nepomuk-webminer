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

#ifndef SEARCHMICROSOFTACADEMIC_H
#define SEARCHMICROSOFTACADEMIC_H

#include <QObject>

class PublicationEntry;
class QNetworkAccessManager;
class QNetworkReply;

class SearchMicrosoftAcademic : public QObject
{
    Q_OBJECT
public:
    explicit SearchMicrosoftAcademic(QObject *parent = 0);

    void setPublicationEntry(PublicationEntry *entryToQuery);
    PublicationEntry *publicationEntry();

signals:
    void searchFinished();

public slots:
    void startSearch();

private slots:
    void searchReplyFinished(QNetworkReply *reply);
//    void detailReplyFinished(QNetworkReply *reply);
    void bibtexReplyFinished(QNetworkReply *reply);
    void citationReplyFinished(QNetworkReply *reply);
    void citationBibtexReplyFinished(QNetworkReply *reply);

private:
    PublicationEntry *m_entryToQuery;
    bool m_altTitleSearchStarted;
    int m_maid;
    int m_citations;
    bool bibtexFileReceived;
    QNetworkAccessManager *m_initialQuerymanager;
//    QNetworkAccessManager *m_detailQuerymanager;
    QNetworkAccessManager *m_bibtexQuerymanager;
    QNetworkAccessManager *m_citationQuerymanager;
    QNetworkAccessManager *m_citationBibTeXQuerymanager;
    QNetworkAccessManager *m_nam;
};

#endif // SEARCHMICROSOFTACADEMIC_H
