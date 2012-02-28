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

    void setForceUpdate(bool update);

    QStringList availableFileTypes();
    QVariantList availableSearchEngines( const QString &resourceType);

    void setUsedEngine(const QString &type, const QString &engine);
    void startFetching(const QString &type);

signals:
    void progressStatus(const QString &status);
    void progress(int current, int max);
    void fileFetchingDone();
    void fetchingDone();

public slots:
    void lookupFiles(const KUrl &fileOrFolder);
    void lookupResource(const Nepomuk::Resource &resource);
    void lookupResource(const QList<Nepomuk::Resource> &resources);

    void searchResults(const QVariantList &searchResults);
    // start search again with alttitle
    //@todo select next search engine instead?
    void noSearchResultsFound();
    void itemResult(const QVariantMap &itemResults);

private slots:
    void addFilesToList(const KUrl &fileUrl);
    void lookupNextMetaDataOnTheWeb();
    void pythonStdOut(QString test);

private:
    bool m_forceUpdate;
    PythonQtObjectPtr mainContext;
    QMap<QString, QList<MetaDataParameters> > m_filesToLookup;
    QMap<QString,QString> m_selectedSearchEngine;

    int m_maxProgress;
    int m_curProgress;
    QString m_currentType;

    bool m_altSearchStarted;
    NepomukPipe *m_nepomukPipe;
};

#endif // METADATAFETCHER_H
