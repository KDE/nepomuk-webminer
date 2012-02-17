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

#include <PythonQt/PythonQt.h>

#include <KDE/KApplication>
#include <KDE/KDebug>

#include <QtCore/QDir>
#include <QtCore/QTimer>


MetaDataFetcher::MetaDataFetcher(QObject *parent)
    : QObject(parent)
{
    PythonQt::init( PythonQt::RedirectStdOut );
    mainContext = PythonQt::self()->getMainModule();
    PythonQt::self()->addObject(mainContext, "cppObj", this);

    QString pythonFile = QString("/home/joerg/Development/Python/webextractor/extractor.py");
    PythonQt::self()->addSysPath(QString("/home/joerg/Development/Python/webextractor"));//pythonFile.section('/', 0, -2));
    mainContext.evalFile(pythonFile);

    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(pythonStdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(pythonStdOut(QString)));

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

        MetaDataParameters *searchEntry = pdfExtractor.parseUrl( nextFile );
//        searchEntry->resourceType = NBIB::Publication();

        lookupMetaDataOnTheWeb( searchEntry );
    }
    else if(suffix == QLatin1String("odt")) {
        OdfExtractor odfExtractor;

        MetaDataParameters *searchEntry = odfExtractor.parseUrl( nextFile );
//        searchEntry->resourceType = NBIB::Publication();

        lookupMetaDataOnTheWeb( searchEntry );
    }
    else {
        // unknown file suffix
        retrieveMetaDataFromNextFile();
    }
}

void MetaDataFetcher::lookupMetaDataOnTheWeb(MetaDataParameters *entryToQuery)
{
    // retrieve list of available python plugins that support this resource type

    //for all modules if module.resourceType == entryToQuery->resourceType

    // now tell the user what choices he has, so he can select one engine
    // debug, use always microsoft academics for now
    QVariantList list;
    list << "msa";//     moduleId
    list << entryToQuery->metaData.value(QLatin1String("title"));  // title
    //list << entryToQuery->metaData.value(QLatin1String("author"));  // author
    //list << entryToQuery->metaData.value(QLatin1String("freetext"));  // freetext

    // start the search with the module specified with the moduleId
    mainContext.call("search", list);
    // wait till searchResults() slot is called
}

void MetaDataFetcher::searchResults(const QVariantList &searchResults)
{
    qDebug() << "found search " << searchResults.size() << "results";

    foreach(const QVariant &entry, searchResults) {
        QVariantMap map = entry.toMap();

        qDebug() << "Title: " << map.value("title").toString();
        qDebug() << "Url: " << map.value("url").toString();
    }

    // let the user decide which entry he might want to use (default to first one for now)

    QVariantMap map = searchResults.first().toMap();

    // extract item info, module is selected based on url regexp
    // this is because brower plugins reuse this part and figure the module out anyway
    // also we could fetch item urls tha tlead to a different side, than the
    // page we used to search for these entries
    QVariantList list; list << map.value("url");
    mainContext.call("extract", list);
}

void MetaDataFetcher::itemResult(const QVariantMap &itemResults)
{
    qDebug() << "item result" << itemResults;

    // now we have the fetched meta data as nice QVariantmap call the pipeImporter
    // these knwo how the VariantMap should be handled and create the
    // right SimpleResource, SimpleResourceGraph information from it

    // wait a while and start next search
    // we wait, so we don#t hammer to much on the website api
    QTimer::singleShot(6000, this, SLOT(retrieveMetaDataFromNextFile()));
}

void MetaDataFetcher::pythonStdOut(QString test)
{
    qDebug() << test;
}
