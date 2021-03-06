/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
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

#include "nepomukwebminer_export.h"

namespace Nepomuk2
{
class Resource;
}

namespace NepomukWebMiner
{
namespace Extractor
{

class MetaDataParameters;
class ResourceExtractorPrivate;

/**
  * @brief The resource extractor is used to retrieve some basic information from files and resources
  *
  * Here we check the file or the Nepomuk resource and extract values from it that can be used for the @c WebExtractor search.
  *
  * Here we rely on the Nepomuk FileIndexer for the basic resource extraction and combine it with additional
  * filename analyzing to get more information if necessary.
  *
  * Each file/resource is represented by its @c MetaDataParameters that is used for the actual search
  *
  * This Extractor keeps a list of all files that must be processed.
  * You can get the list via resourcesList() and pop the next entry from takeNext()
  */
class NEPOMUKWEBMINER_EXPORT ResourceExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief standard constructor
     *
     * @param parent some parten object
     */
    explicit ResourceExtractor(QObject *parent = 0);

    /**
     * @brief Cancel resource/file lookup
     */
    void cancel();

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
     * @brief Sets a hint, that the given file is a movie
     *
     * @param moviemode @arg true resource is a movie
     *                  @arg false resource is either a tvshow or movie (default)
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
      */
    void lookupFiles(const KUrl &fileOrFolder, bool nested = false);

    /**
      * @brief Prepare a nepomuk resource for the lookup.
      *
      * Gets some important search parameters from it
      *
      * @p resource the resource where the data should be added to
      */
    void lookupResource(const Nepomuk2::Resource &resource);

    /**
     * @brief Prepare a list nepomuk resources for the lookup.
     *
     * @param resources list of nepomuk resources where the data should be added to
     */
    void lookupResource(const QList<Nepomuk2::Resource> &resources);

    /**
      * @brief Returns the complete list of resources that are ready for the metadata fetching
      *
      * To get the type of resource check the resourceType parameter
      */
    QList<NepomukWebMiner::Extractor::MetaDataParameters *> resourcesList();

    /**
     * @brief Removes the first element in the lookup list and returns it
     *
     * The caller needs to delete the object if it is not needed anymore
     *
     * @return next file to process
     */
    NepomukWebMiner::Extractor::MetaDataParameters *takeNext();

Q_SIGNALS:
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
      * @brief Called by lookupFiles checks if the metadata matches and extracts some info
      *
      * The results will be added to @c m_resourcesToLookup
      *
      * @p fileUrl file location that should be added
      */
    void addFilesToList(const KUrl &fileUrl);

    /**
     * @brief Checks the mimetype of the file if we have one of the supported files
     *
     * @param fileUrl the url of the file on the disk
     *
     * @return @arg true if the file mimetype is supported
     *         @arg false if the mimetype is not supported
     */
    bool mimeTypeChecker(const KUrl &fileUrl);

    QString selectPlugin(const KUrl fileUrl);

    /**
     * @brief Checks the Ontology Type <i>(nmm::Movie/nbib::Publication etc)</i> and extracts some basic information
     *
     * @param mdp the MetaDataParameters object where the data will added to
     * @param resource the resource that will be checked
     *
     * @return @arg true some data could be extracted
     *         @arg false if no data was found
     */
    bool resourceChecker(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const Nepomuk2::Resource &resource);

    /**
     * @brief Starts the FileNameAnalyzer to extract more information from the filename via regexp
     *
     * @param mdp the MetaDataParameters object where the data will added to
     * @param fileUrl the url of the file on the disk
     */
    void filenameAnalyzer(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl);

    Q_DECLARE_PRIVATE(ResourceExtractor)
    ResourceExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // RESOURCEEXTRACTOR_H
