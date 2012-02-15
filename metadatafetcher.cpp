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

#include "metadatafetcher.h"

#include "publicationentry.h"
#include "fileextractor/popplerextractor.h"
#include "fileextractor/odfextractor.h"
#include "search/searchmicrosoftacademic.h"

#include <KDE/KApplication>
#include <KDE/KDebug>

#include <QtCore/QDir>
#include <QtCore/QTimer>


MetaDataFetcher::MetaDataFetcher(QObject *parent)
    : QObject(parent)
{
}

void MetaDataFetcher::run()
{
    retrieveMetaDataFromNextFile();
}

void MetaDataFetcher::lookupFiles(const KUrl &fileOrFolder)
{
    QDir dir(fileOrFolder.toLocalFile());

    if(dir.exists()) {
        QFileInfoList list = dir.entryInfoList();
        foreach( const QFileInfo &fileInfo, list) {
            if( fileInfo.fileName() == QString(".") || fileInfo.fileName() == QString("..")) {
                continue;
            }
            KUrl url( fileInfo.absoluteFilePath() );
            if(fileInfo.isDir()) {
                lookupFiles( url );
            }
            else {
                m_filesToLookup << url;
            }
        }
    }
    else {
        m_filesToLookup << fileOrFolder.toLocalFile();
    }
}

void MetaDataFetcher::lookupResource(const Nepomuk::Resource &resource)
{
    m_resourcesToLookup << resource;
}

void MetaDataFetcher::lookupResource(const QList<Nepomuk::Resource> &resources)
{
    m_resourcesToLookup << resources;
}

void MetaDataFetcher::retrieveMetaDataFromNextFile()
{
    if(m_filesToLookup.isEmpty()) {
        emit fetchingDone();
        return;
    }

    KUrl nextFile = m_filesToLookup.takeFirst();

    QFileInfo file(nextFile.toLocalFile());
    QString suffix = file.suffix();

    if(suffix == QLatin1String("pdf")) {
        PopplerExtractor pdfExtractor;
        PublicationEntry *pubEntry = pdfExtractor.parseUrl( nextFile );
        lookupMetaDataOnTheWeb( pubEntry );
    }
    else if(suffix == QLatin1String("odt")) {
        OdfExtractor odfExtractor;
        PublicationEntry *pubEntry = odfExtractor.parseUrl( nextFile );
        lookupMetaDataOnTheWeb( pubEntry );
    }
    else {
        // unknown file suffix
        retrieveMetaDataFromNextFile();
    }
}

void MetaDataFetcher::lookupMetaDataOnTheWeb(PublicationEntry *entryToQuery)
{
    SearchMicrosoftAcademic *sma = new SearchMicrosoftAcademic;
    connect(sma, SIGNAL(searchFinished()), this, SLOT(searchfinished()));

    sma->setPublicationEntry( entryToQuery );
    sma->startSearch();
}
void MetaDataFetcher::searchfinished()
{
    SearchMicrosoftAcademic *sma = qobject_cast<SearchMicrosoftAcademic*>(sender());

    if(!sma) {
        kWarning() << "could not cast search class from sender";
        return;
    }

    PublicationEntry *pe = sma->publicationEntry();

    kDebug() << "###########################################################";
    kDebug() << "# Metadata for file : " << pe->fileUrl;
    kDebug() << "title    :\t" << pe->dataMap.value(QLatin1String("title"));
    kDebug() << "alt-title:\t" << pe->dataMap.value(QLatin1String("altTitle"));;
    kDebug() << "type: \t@" << pe->type;
    kDebug() << "data:";

    QMapIterator<QString, QString> i(pe->dataMap);
    while (i.hasNext()) {
        i.next();
        qDebug() << "\t" << i.key() << ": " << i.value();
    }

    qDebug() << "citations:" << pe->citations.size();

    foreach(PublicationEntry *citation, pe->citations) {
        qDebug() << "\t @" << citation->type;
        QMapIterator<QString, QString> i2(citation->dataMap);
        while (i2.hasNext()) {
            i2.next();
            qDebug() << "\t\t" << i2.key() << ": " << i2.value();
        }
    }

    qDebug() << "###########################################################";


    // some timeout before we start the next search query..
    QTimer::singleShot(6000, this, SLOT(retrieveMetaDataFromNextFile()));
//    retrieveMetaDataFromNextFile();
}
