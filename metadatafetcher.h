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

signals:
    void fetchingDone();

public slots:
    void run();
    void lookupFiles(const KUrl &fileOrFolder);
    void lookupResource(const Nepomuk::Resource &resource);
    void lookupResource(const QList<Nepomuk::Resource> &resources);

    void searchResults(const QVariantList &searchResults);
    void itemResult(const QVariantMap &itemResults);

private slots:
    void lookupMetaDataOnTheWeb();
    void retrieveMetaDataFromNextFile();
    void pythonStdOut(QString test);

private:
    QList<KUrl> m_filesToLookup;
    QList<Nepomuk::Resource> m_resourcesToLookup;
    PythonQtObjectPtr mainContext;

    MetaDataParameters m_metaDataParameters;
    NepomukPipe *m_nepomukPipe;
};

#endif // METADATAFETCHER_H
