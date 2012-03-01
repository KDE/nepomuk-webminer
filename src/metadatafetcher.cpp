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

#include "metadataparameters.h"

#include "fileextractor/popplerextractor.h"
#include "fileextractor/odfextractor.h"
#include "fileextractor/videoextractor.h"

#include "nepomukpipe/publicationpipe.h"
#include "nepomukpipe/moviepipe.h"

#include <PythonQt/PythonQt.h>

#include <KDE/KApplication>
#include <KDE/KStandardDirs>
#include <KDE/KInputDialog>
#include <KDE/KMimeType>
#include <KDE/KDebug>


#include <KDE/Nepomuk/File>
#include <Nepomuk/Vocabulary/NMM>

#include <QtCore/QDir>
#include <QtCore/QTimer>

MetaDataFetcher::MetaDataFetcher(QObject *parent)
    : QObject(parent)
    , m_forceUpdate(false)
    , m_bulkDownload(false)
    , m_maxProgress(0)
    , m_curProgress(0)
{
    PythonQt::init( PythonQt::RedirectStdOut );
    mainContext = PythonQt::self()->getMainModule();
    PythonQt::self()->addObject(mainContext, "cppObj", this);

//    QString pythonFile = QString("/home/joerg/Development/KDE/metadataextractor/python/extractor.py");
//    PythonQt::self()->addSysPath(QString("/home/joerg/Development/KDE/metadataextractor/python/"));
    QString pythonFile = KStandardDirs::locate("data", "metadataextractor/extractor.py");
    PythonQt::self()->addSysPath(pythonFile.section('/', 0, -2));

    mainContext.evalFile(pythonFile);

    kDebug() << "main python file used ::" << pythonFile;

    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(pythonStdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(pythonStdOut(QString)));

    //m_selectedSearchEngine.insert(QLatin1String("publication"), QLatin1String("msa"));
    //m_selectedSearchEngine.insert(QLatin1String("tvshow"), QLatin1String("msa"));
    //m_selectedSearchEngine.insert(QLatin1String("movie"), QLatin1String("imdb"));
}

MetaDataFetcher::~MetaDataFetcher()
{
}

void MetaDataFetcher::setForceUpdate(bool update)
{
    m_forceUpdate = update;
}

void MetaDataFetcher::setBulkDownload(bool bulk)
{
    m_bulkDownload = bulk;
}

void MetaDataFetcher::lookupFiles(const KUrl &fileOrFolder)
{
    emit progressStatus( i18n("Check available files") );
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
                addFilesToList(url);
            }
        }
    }
    else {
        addFilesToList(fileOrFolder.toLocalFile());
    }

    emit progressStatus( i18n("Found:") );

    int pubSize = m_resourcesToLookup.value(QLatin1String("publication")).size();
    if( pubSize != 0)
        emit progressStatus( i18n("%1 files that miss publication data", pubSize) );

    int tvshowSize = m_resourcesToLookup.value(QLatin1String("tvshow")).size();
    if( tvshowSize != 0)
        emit progressStatus( i18n("%1 files that miss tvshow data", tvshowSize) );

    int movieSize = m_resourcesToLookup.value(QLatin1String("movie")).size();
    if( movieSize != 0)
        emit progressStatus( i18n("%1 files that miss movie data", movieSize) );

    if( pubSize == 0 && tvshowSize == 0 && movieSize == 0) {
        emit progressStatus( i18n("No files for meta data fetching found") );
    }
    else {
        // max progress 3 + items
        // as every item needs, search / extract / insert
        m_maxProgress = 3 * (pubSize + tvshowSize + movieSize);
        emit fileFetchingDone();
    }
}

void MetaDataFetcher::lookupResource(const Nepomuk::Resource &resource)
{
    //m_resourcesToLookup << resource;
}

void MetaDataFetcher::lookupResource(const QList<Nepomuk::Resource> &resources)
{
    //m_resourcesToLookup << resources;
}

QStringList MetaDataFetcher::availableResourceTypes()
{
    QStringList availableTypes;
    foreach(const QString &k, m_resourcesToLookup.keys() ) {
        availableTypes << k;
    }

    return availableTypes;
}

QVariantList MetaDataFetcher::availableSearchEngines( const QString &resourceType )
{
    QVariantList list; list << resourceType;
    QVariant engines = mainContext.call("availableSearchEngines", list);

    QVariantList engineList;
    if(engines.isNull()) {
        return engineList;
    }

    engineList = engines.toList();

    if(!engineList.isEmpty()) {
        return engineList;
    }

    engineList.append( engines );

    return engineList;
}

int MetaDataFetcher::resourcesToUpdate( const QString &type ) const
{
    QList<MetaDataParameters *> mdpList = m_resourcesToLookup.value(type);

    return mdpList.size();
}

MetaDataParameters *MetaDataFetcher::getResource( const QString &type, int resource ) const
{
    QList<MetaDataParameters *> mdpList = m_resourcesToLookup.value(type);

    if(resource >= mdpList.size()) {
        return 0;
    }
    else {
        return mdpList.at(resource);
    }
}

void MetaDataFetcher::searchItem(MetaDataParameters *mdp, const QString &engine)
{
    m_currentItemToFetch = mdp;
    m_currentEngineToUse = engine;

    QString title = mdp->searchTitle;

    emit progressStatus( QLatin1String("") );
    emit progressStatus( QLatin1String("#############################################################") );
    emit progressStatus( i18n("# check next file: %1", mdp->resourceUri.fileName()) );

    if(!title.isEmpty()) {
        emit progressStatus( i18n("Lookup title \"%1\"", title) );

        m_altSearchStarted = false;

        QVariantList list;
        list << engine; //search engine identifier
        list << title;
        //list << entryToQuery->metaData.value(QLatin1String("author"));  // author
        //list << entryToQuery->metaData.value(QLatin1String("freetext"));  // freetext

        kDebug() << "start python query with arguments" << list;

        // start the search with the module specified with the moduleId
        mainContext.call("search", list);
        // wait till searchResults() slot is called
    }
    else {
        emit progressStatus( i18n("Can't lookup file, no search parameters defined") );
    }
}

void MetaDataFetcher::searchResults(const QVariantList &searchResults)
{
//    QVariantList searchResultList = m_searchResults.value(m_currentItemToFetch);

//    searchResultList.append(searchResults);
    m_searchResults.insert(m_currentItemToFetch, searchResults);

    emit progressStatus(i18n("Several results found. Please select the one you want to use"));
    emit selectSearchEntry(m_currentItemToFetch, searchResults);
}

void MetaDataFetcher::noSearchResultsFound()
{
    // ok nothing found with normal title check alttitle
    MetaDataParameters *mdp = m_currentItemToFetch;
    QString altTitle = mdp->searchAltTitle;

    if(!m_altSearchStarted && !altTitle.isEmpty()) {

        emit progressStatus( i18n("No search result found, try with alternative title \"%1\"", altTitle) );

        QVariantList list;
        list << m_currentEngineToUse;
        list << altTitle;
        //list << entryToQuery->metaData.value(QLatin1String("author"));  // author
        //list << entryToQuery->metaData.value(QLatin1String("freetext"));  // freetext

        // start the search with the module specified with the moduleId
        mainContext.call("search", list);
        // wait till searchResults() slot is called
    }
    else {
        emit progressStatus( i18n("No search result found") );

        QVariantList empty;
        emit selectSearchEntry(mdp, empty);
    }
}

void MetaDataFetcher::fetchItem(MetaDataParameters *mdp, const KUrl &fetchUrl)
{
    m_currentItemToFetch = mdp;

    // extract item info, module is selected based on url regexp
    // this is because the browser plugins reuse this part and figure the module out anyway
    // also we could fetch item urls that lead to a different side, than the
    // page we used to search these entries

    emit progressStatus( i18n("Start fetching the item, this may take a while") );

    QVariantList list; list << fetchUrl.url(); //selectedEntry.value("url");
    mainContext.call("extract", list);
}

void MetaDataFetcher::itemResult(const QVariantMap &itemResults)
{
    // now we have the fetched meta data as nice QVariantMap call the pipeImporter
    // these know how the VariantMap should be handled and create the
    // right SimpleResource, SimpleResourceGraph information from it

    // combine the downloaded meta data with the one extracted from the file
    //    m_metaDataParameters.metaData.unite(itemResults); // creates multimap .. not good
    QMapIterator<QString, QVariant> i(itemResults);
    while (i.hasNext()) {
        i.next();
        m_currentItemToFetch->metaData.insert(i.key(), i.value());
    }

    emit fetchedItemDetails( m_currentItemToFetch, m_currentItemToFetch->metaData );
}

void MetaDataFetcher::saveItemMetaData( MetaDataParameters *mdp )
{
    QString type = mdp->resourceType;

    NepomukPipe *nepomukPipe = 0;
    if(type == QLatin1String("publication")) {
        nepomukPipe = new PublicationPipe;
    }

    else if(type == QLatin1String("tvshow")) {

        nepomukPipe = 0;
        //m_nepomukPipe = new PubEntryToNepomukPipe;
    }

    else if(type == QLatin1String("movie")) {
        nepomukPipe = new MoviePipe;
    }

    if(nepomukPipe) {
        nepomukPipe->pipeImport( mdp );

        mdp->metaDataSaved = true;
    }
    else {
        emit progressStatus(i18n("No nepomuk pipe available for the resoure type %1", type));
    }
}

void MetaDataFetcher::pythonStdOut(QString test)
{
    if(!test.trimmed().isEmpty()) {
        QString pythonOutput = QString("\t %1").arg(test);
        emit progressStatus(pythonOutput);
        kDebug() << test;
    }
}

void MetaDataFetcher::addFilesToList(const KUrl &fileUrl)
{
    Nepomuk::File fileResource(fileUrl);

    if( !m_forceUpdate && (
        fileResource.hasProperty(Nepomuk::Vocabulary::NBIB::publishedAs()) ||
        fileResource.hasType(Nepomuk::Vocabulary::NMM::TVShow()) ||
        fileResource.hasType(Nepomuk::Vocabulary::NMM::Movie()))) {
        kDebug() << "skip file " << fileUrl;
        return;
    }

    KSharedPtr<KMimeType> kmt = KMimeType::findByUrl( fileUrl );

    MetaDataParameters *metaDataParameters = new MetaDataParameters;
    metaDataParameters->metaDataSaved = false;

    if(kmt.data()->name().contains(QLatin1String("application/vnd.oasis.opendocument.text"))) {
        OdfExtractor odfExtractor;
        odfExtractor.parseUrl( metaDataParameters, fileUrl );
    }
    else if(kmt.data()->name().contains(QLatin1String("application/pdf"))) {
        PopplerExtractor pdfExtractor;
        pdfExtractor.parseUrl( metaDataParameters, fileUrl );
    }

    else if(kmt.data()->name().contains(QLatin1String("video/"))) {
        VideoExtractor videoExtractor;
        videoExtractor.parseUrl( metaDataParameters, fileUrl );
    }
    else {
        kDebug() << "unsupportet mimetype" << kmt.data()->name();
    }

    QList<MetaDataParameters*> currentFileList = m_resourcesToLookup.value( metaDataParameters->resourceType );
    currentFileList.append( metaDataParameters );
    m_resourcesToLookup.insert( metaDataParameters->resourceType, currentFileList);
}
