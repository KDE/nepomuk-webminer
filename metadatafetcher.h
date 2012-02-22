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

#include "publicationentry.h"

class NepomukPipe;

class MetaDataFetcher : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataFetcher(QObject *parent = 0);

    QStringList availableFileTypes();
    QVariantList availableSearchEngines( const QString &resourceType);

    void setUsedEngine(const QString &type, const QString &engine);
    void startFetching(const QString &type);

signals:
    void progressStatus(const QString &status);
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
    PythonQtObjectPtr mainContext;
    QMap<QString, QList<MetaDataParameters> > m_filesToLookup;
    QMap<QString,QString> m_selectedSearchEngine;

    QString m_currentType;

    bool m_altSearchStarted;
    NepomukPipe *m_nepomukPipe;
};

#endif // METADATAFETCHER_H
