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

#ifndef RESOURCEEXTRACTOR_H
#define RESOURCEEXTRACTOR_H

#include <QObject>
#include <KDE/KUrl>
#include <Nepomuk/Resource>

class MetaDataParameters;

/**
  * @brief The resource extarctor is used to retrieve some basic information from files and resources
  *
  * Here we check the file or the Nepomuk resource and extract values from it that can be used for the @c WebExtractor search.
  * Usually it would be enough to check only Nepomuk for this info and when some files are given as input the right nepomuk
  * resource can be looked up and used for the retrival. As the libstreamanalyzer might fail on several pdf files this class
  * uses its own pdf parsing to get the right details.
  *
  * The out put of this class is a ordered Hash where all files are sorted by category:
  * @li publication
  * @li tvshow
  * @li movie
  * Where each file/resource is represented by its MetaDataParameters that is used for the actaul search
  */
class ResourceExtractor : public QObject
{
    Q_OBJECT
public:
    explicit ResourceExtractor(QObject *parent = 0);

    /**
      * If force update is @c true metadata is also fetched for files that already have data attached to them.
      *
      * Otherwise those files will be skipped
      */
    void setForceUpdate(bool update);

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

    QList<MetaDataParameters *> resourcesToFetch(const QString &type);

signals:
    void progressStatus(const QString &status);

    void resourceExtarctionDone();

private:
    /**
      * called by lookupFiles checks if the metadata matches and extarcts some info
      *
      * the results will be added to @c m_resourcesToLookup
      */
    void addFilesToList(const KUrl &fileUrl);

    bool m_forceUpdate;
    QMap<QString, QList<MetaDataParameters *> > m_resourcesToLookup;
};

#endif // RESOURCEEXTRACTOR_H
