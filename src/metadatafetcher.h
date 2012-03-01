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

#ifndef METADATAFETCHER_H
#define METADATAFETCHER_H

#include <KDE/KUrl>
#include <Nepomuk/Resource>

#include <QtCore/QObject>
#include <QtCore/QList>

#include <PythonQt/PythonQt.h>

#include "metadataparameters.h"

#include "metadataextractor_export.h"

class NepomukPipe;

/**
  * @brief Main worker class to fetch all the data
  *
  * When @c lookupFiles() is called all files will be scanned and its local data retrieved.
  * Depending on the @c mimetype the right @c fileextractor is called to gather the necessary bits.
  *
  * For pdf/odt this means reading the RDF output, for video files the information is retrieved via filename extraction.
  *
  * Once this data is collected and the files are sorted by the resourcetype we need to fetch (nbib:Publication, nmm:TvShow and such),
  * the @c fileFetchingDone() signal is thrown. The user should now select the search engine hew wants to use and
  * afterwards start the extraction via @c startFetching()
  *
  * One file after another is now checked and the right python plugin called to:
  * @li search for items that might fit
  * @li extract the information from the right item
  *
  * If the search returned only 1 item where the @c title is exactly as the title we searched for, this entry is automatically
  * selected. Otherwise the user can select the right entry via the @c SelectSearchResultDialog.
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT MetaDataFetcher : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataFetcher(QObject *parent = 0);
    ~MetaDataFetcher();

    /**
      * If force update is @c true metadata is also fetched for files that already have data attached to them.
      *
      * Otherwise those files will be skipped
      */
    void setForceUpdate(bool update);

    /**
      * if @p bulk is true thesearch results for each item to lookup is cached
      * and the user can select the right entry once all items are queried.
      *
      * This reduce the need to interact with the fetcher after every item
      *
      * Default is false
      */
    void setBulkDownload(bool bulk);

    /**
      * Prepare a file or folder for the lookup
      *
      * Check each file mimetype if it is supported and parse some additional info from the file
      *
      * @p fileOrFolder the local url of the file or folder
      *
      * @see addFilesToList()
      * @see OdfExtractor
      * @see PopplerExtractor
      * @see VideoExtractor
      */
    void lookupFiles(const KUrl &fileOrFolder);

    /**
      * Prepare a resource for the lookup.
      *
      * Gets some important search parameters from it
      */
    void lookupResource(const Nepomuk::Resource &resource);
    void lookupResource(const QList<Nepomuk::Resource> &resources);

    /**
      * If a folder with different filetypes is scanned this returnes the list of types
      *
      * the types are "publication", "tvshow", "movie"
      */
    QStringList availableResourceTypes();

    /**
      * Returns the list of available search engines for a specific resource type.
      *
      * For example the resourceType "movie" returns the imdb and other engines that can fetch
      * movie data
      */
    QVariantList availableSearchEngines( const QString &resourceType );

    int resourcesToUpdate( const QString &type ) const;
    MetaDataParameters *getResource( const QString &type, int resource ) const;


    void searchItem( MetaDataParameters *mdp, const QString &engine );
    void saveItemMetaData( MetaDataParameters *mdp );

signals:
    void progress(int current, int max);
    void progressStatus(const QString &status);

    void fileFetchingDone();
    void fetchingDone();

    void searchlookUpFinished();
    void itemExtractionFinished();

    void selectSearchEntry( MetaDataParameters *mdp, QVariantList searchResults);
    void fetchedItemDetails(MetaDataParameters *mdp, QVariantMap itemDetails);

public slots:
    /**
      * This slot is called from the python plugin when the serach results are ready
      *
      * If only 1 search result is found or only 1 result that has an exact math on the title take this one
      * and call @c fetchItem()
      *
      * Otherwise @c selectSearchEntry() is emitted where the user needs to do his selection
      * After the selectio nis done @c fetchItem() mus tbe called to proceed
      *
      * In the case @c batchDownload is enabled the search rsults will be saved and the next item is used for the lookup
      * after all items are lookedUp @c searchlookUpFinished is emitted and the user can select the entries he want in one go
      */
    void searchResults(const QVariantList &searchResults);
    void noSearchResultsFound();

    void fetchItem(MetaDataParameters *mdp, const KUrl &fetchUrl);
    void itemResult(const QVariantMap &itemResults);

private slots:
    /**
      * Outputs the python "print xxx" to the commandline and emit them via progressStatus()
      */
    void pythonStdOut(QString test);

private:
    /**
      * called by lookupFiles checks if the metadata matches and extarcts some info
      *
      * the results will be added to @c m_resourcesToLookup
      */
    void addFilesToList(const KUrl &fileUrl);

private:
    bool m_forceUpdate;
    bool m_bulkDownload;

    PythonQtObjectPtr mainContext;
    QMap<QString, QList<MetaDataParameters *> > m_resourcesToLookup;
    QMap<MetaDataParameters *, QVariantList > m_searchResults;
    MetaDataParameters *m_currentItemToFetch;
    QString m_currentEngineToUse;

    int m_maxProgress;
    int m_curProgress;
    QString m_currentType;

    bool m_altSearchStarted;
};

#endif // METADATAFETCHER_H
