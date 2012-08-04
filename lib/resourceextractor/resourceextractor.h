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

#include <QtCore/QObject>
#include <KDE/KUrl>

namespace Nepomuk2 {
    class Resource;
}

namespace NepomukMetaDataExtractor {
namespace Extractor {

class MetaDataParameters;
class ResourceExtractorPrivate;

/**
  * @brief The resource extarctor is used to retrieve some basic information from files and resources
  *
  * Here we check the file or the Nepomuk resource and extract values from it that can be used for the @c WebExtractor search.
  * Usually it would be enough to check only Nepomuk for this info and when some files are given as input the right nepomuk
  * resource can be looked up and used for the retrival. As the @c libstreamanalyzer might fail on several pdf files this class
  * uses its own pdf parsing to get the right details.
  *
  * Each file/resource is represented by its @c MetaDataParameters that is used for the actual search
  */
class ResourceExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief standard constructor
     * @param parent some parten object
     */
    explicit ResourceExtractor(QObject *parent = 0);

    /**
      * @brief If force update is @c true metadata is also fetched for files that already have data attached to them.
      *
      * Otherwise those files will be skipped
      *
      * @p update @arg true overwrite current meta data
      *           @arg false skip file (default)
      */
    void setForceUpdate(bool update);

    /**
     * @brief Sets a hint, that the given file is a tvshow
     *
     * @param tvshowmode @arg true resource is a tvshow
     *                   @arg false resource is either a tvshow or movie (default)
     */
    void setTvShowMode(bool tvshowmode);

    /**
     * @brief sets a hint for tvshow resources to check also folder names for title/season/episode
     *
     * @param useFolderNames @arg true check fodler names
     *                       @arg false do not check fodler names (default)
     */
    void setTvShowNamesInFolders(bool useFolderNames);

    /**
     * @brief Sets a hint, that the given file is a movie
     *
     * @param moviemode @arg true resource is a movie
     *                   @arg false resource is either a tvshow or movie (default)
     */
    void setMovieMode(bool moviemode);

    /**
      * @brief Prepare a file or folder for the lookup
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
      * @brief Prepare a nepomuk resource for the lookup.
      *
      * Gets some important search parameters from it
      *
      * @todo TODO: implement data extracting from existing resources
      */
    void lookupResource(const Nepomuk2::Resource &resource);
    void lookupResource(const QList<Nepomuk2::Resource> &resources);

    /**
      * @brief Returns the complete list of resources that are ready for the metadata fetching
      *
      * To get the type of resource check the resourceType parameter
      */
    QList<NepomukMetaDataExtractor::Extractor::MetaDataParameters *> resourcesList();

    /**
     * @brief Removes the first element in the lookup list and returns it
     *
     * The caller needs to delete the object if it is not needed anymore
     * @return next file to process
     */
    NepomukMetaDataExtractor::Extractor::MetaDataParameters *takeNext();

signals:
    /**
     * @brief Emits some information whats going on internally
     *
     * @param status current status message
     */
    void progressStatus(const QString &status);

    /**
     * @brief emits if all files in a folder or all resources in a list finished the preprocessing
     */
    void resourceExtarctionDone();

private:
    /**
      * @brief Called by lookupFiles checks if the metadata matches and extarcts some info
      *
      * the results will be added to @c m_resourcesToLookup
      *
      * @p fileUrl file location that should be added
      */
    void addFilesToList(const KUrl &fileUrl);

    Q_DECLARE_PRIVATE(ResourceExtractor)
    ResourceExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // RESOURCEEXTRACTOR_H
