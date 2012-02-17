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
#include "nepomukpipe/pubentrytonepomukpipe.h"

#include <PythonQt/PythonQt.h>

#include <KDE/KApplication>
#include <KDE/KStandardDirs>
#include <KDE/KDebug>

#include <QtCore/QDir>
#include <QtCore/QTimer>

MetaDataFetcher::MetaDataFetcher(QObject *parent)
    : QObject(parent)
{
    m_metaDataParameters = 0;
    PythonQt::init( PythonQt::RedirectStdOut );
    mainContext = PythonQt::self()->getMainModule();
    PythonQt::self()->addObject(mainContext, "cppObj", this);

    QString pythonFile = KStandardDirs::locate("data", "metadataextractor/extractor.py");
    PythonQt::self()->addSysPath(pythonFile.section('/', 0, -2));
    mainContext.evalFile(pythonFile);

    qDebug() << "main python file used ::" << pythonFile;

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
    qDebug() << "files to check" << m_filesToLookup.size();

    if(m_filesToLookup.isEmpty()) {
        emit fetchingDone();
        return;
    }

    KUrl nextFile = m_filesToLookup.takeFirst();

    QFileInfo file(nextFile.toLocalFile());
    QString suffix = file.suffix();

    delete m_metaDataParameters;
    m_metaDataParameters = new MetaDataParameters;

    if(suffix == QLatin1String("pdf")) {
        PopplerExtractor pdfExtractor;

        pdfExtractor.parseUrl( m_metaDataParameters, nextFile );
//        searchEntry->resourceType = NBIB::Publication();
        m_nepomukPipe = new PubEntryToNepomukPipe;
    }
    else if(suffix == QLatin1String("odt")) {
        OdfExtractor odfExtractor;

        odfExtractor.parseUrl( m_metaDataParameters, nextFile );
//        searchEntry->resourceType = NBIB::Publication();
        m_nepomukPipe = new PubEntryToNepomukPipe;
    }
    else {
        // unknown file suffix
        retrieveMetaDataFromNextFile();
    }

    lookupMetaDataOnTheWeb( );
}

void MetaDataFetcher::lookupMetaDataOnTheWeb()
{
    QString title = m_metaDataParameters->metaData.value(QLatin1String("title")).toString();

    if(!title.isEmpty()) {
        m_altSearchStarted = false;
        // retrieve list of available python plugins that support this resource type

        //for all modules if module.resourceType == entryToQuery->resourceType

        // now tell the user what choices he has, so he can select one engine
        // debug, use always microsoft academics for now
        QVariantList list;
        list << "msa";//     moduleId
        list << m_metaDataParameters->metaData.value(QLatin1String("title"));  // title
        //list << entryToQuery->metaData.value(QLatin1String("author"));  // author
        //list << entryToQuery->metaData.value(QLatin1String("freetext"));  // freetext

        qDebug() << "star t python query with arguments" << list;
        // start the search with the module specified with the moduleId
        mainContext.call("search", list);
        // wait till searchResults() slot is called
    }
    else {
        qDebug() << "no title to search for available, check next file";
        retrieveMetaDataFromNextFile();
    }
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

void MetaDataFetcher::noSearchResultsFound()
{
    QString altTitle = m_metaDataParameters->metaData.value(QLatin1String("alttitle")).toString();

    if(!m_altSearchStarted && !altTitle.isEmpty()) {
        QVariantList list;
        list << "msa";//     moduleId
        list << altTitle;  // title
        //list << entryToQuery->metaData.value(QLatin1String("author"));  // author
        //list << entryToQuery->metaData.value(QLatin1String("freetext"));  // freetext

        qDebug() << "list" << list;
        // start the search with the module specified with the moduleId
        mainContext.call("search", list);
        // wait till searchResults() slot is called

    }
    else {
        qDebug() << "no alt title availble check next file";
        // wait a while and start next search
        // we wait, so we don't hammer to much on the website api
        QTimer::singleShot(6000, this, SLOT(retrieveMetaDataFromNextFile()));
    }
}

void MetaDataFetcher::itemResult(const QVariantMap &itemResults)
{
    //qDebug() << "item result" << itemResults;

    // now we have the fetched meta data as nice QVariantmap call the pipeImporter
    // these knwo how the VariantMap should be handled and create the
    // right SimpleResource, SimpleResourceGraph information from it

    //    m_metaDataParameters.metaData.unite(itemResults); // creates multimap .. not good
    QMapIterator<QString, QVariant> i(itemResults);
    while (i.hasNext()) {
        i.next();
        m_metaDataParameters->metaData.insert(i.key(), i.value());
    }

    m_nepomukPipe->pipeImport( m_metaDataParameters );


    // wait a while and start next search
    // we wait, so we don't hammer to much on the website api
    QTimer::singleShot(6000, this, SLOT(retrieveMetaDataFromNextFile()));
}

void MetaDataFetcher::pythonStdOut(QString test)
{
    if(!test.trimmed().isEmpty())
        qDebug() << "python debug: " << test;
}
