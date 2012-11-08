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

#ifndef METADATAPARAMETERS_H
#define METADATAPARAMETERS_H

#include <KDE/KUrl>

#include <QtCore/QString>
#include <QtCore/QVariantMap>

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor
{
namespace Extractor
{

class MetaDataParametersPrivate;

/**
  * @brief This class holds all information about a file/resource we want to search additional meta data for
  *
  * Beside the url of the file location additional search parameters (from @c ResourceExtractor) necessary for the future websearch (@c WebExtractor) are saved in here.
  * The results of this websearch (all item details) are saved as @p metaData @c QVariantMap and precessed by the @c NepomukPipe
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT MetaDataParameters
{
public:
    /**
     * @brief Standard Constructor
     *
     * @param parent parent object
     */
    MetaDataParameters();

    /**
     * @brief title of the entry to search for
     *
     * @param title the title
     */
    void setSearchTitle(const QString &title);
    QString searchTitle() const;

    /**
     * @brief alternative title of the entry to search for in case the first one does not return anything
     *
     * @param alttitle the alt title
     */
    void setSearchAltTitle(const QString &alttitle);
    QString searchAltTitle() const;

    /**
     * @brief Minimum year for the search
     *
     * @param minyear minimum year in YYYY format
     */
    void setSearchYearMin(const QString &minyear);
    QString searchYearMin() const;

    /**
     * @brief Maximum year for the search
     *
     * @param maxyear maximum year in YYYY format
     */
    void setSearchYearMax(const QString &maxyear);
    QString searchYearMax() const;

    /**
     * @brief Publication author, movie director, album preformer
     *
     * @param name person name example: "John Doe", "Doe, J."
     */
    void setSearchPerson(const QString &name);
    QString searchPerson() const;

    /**
     * @brief Name of the tvshow
     *
     * @param showtitle name of the show
     */
    void setSearchShowTitle(const QString &showtitle);
    QString searchShowTitle() const;

    /**
     * @brief Number of the tvshow episode
     *
     * @param episode the episode number
     */
    void setSearchEpisode(const QString &episode);
    QString searchEpisode() const;

    /**
     * @brief Number of the tvshow season
     *
     * @param season the season number
     */
    void setSearchSeason(const QString &season);
    QString searchSeason() const;

    /**
     * @brief Name of the music album
     *
     * @param album the music album name
     */
    void setSearchAlbum(const QString &album);
    QString searchAlbum() const;

    /**
     * @brief Number of the music track
     *
     * @param track the track number
     */
    void setSearchTrack(const QString &track);
    QString searchTrack() const;

    /**
     * @brief Name of the journal where the article appeard in
     *
     * @param journal the journal name
     */
    void setSearchJournal(const QString &journal);
    QString searchJournal() const;

    /**
     * @brief used to select the right set of python modules that allow retrieving data for this resoruce
     *
     * @param resourceType the used resource type of the data . movie, tvshow, document, music
     */
    void setResourceType(const QString &resourceType);
    QString resourceType() const;

    /**
     * @brief the nepomuk or file url where the retrieved data will be added to
     *
     * @param resourceUri the resource uri of the data.
     *        file:// for files on the local harddrive or nepomuk:// for pure virtual data
     */
    void setResourceUri(const KUrl &resourceUri);
    KUrl resourceUri() const;

    /**
     * @brief here all fetched data is pushed into
     *
     * @param metaData the map with all metadata that will be saved in nepomuk
     *
     * @todo define proper api how the data should be saved in her (key=value list)
     */
    void setMetaData(const QVariantMap &metaData);
    QVariantMap metaData() const;

    /**
     * @brief helper function to figure out if the current metadata was already saved to nepomuk or not
     *
     * @param metaDataSaved @arg @c true if data was saved
     *                      @arg @c false if data was not saved yet
     */
    void setMetaDataSaved(bool metaDataSaved);
    bool metaDataSaved() const;

private:
    Q_DECLARE_PRIVATE(MetaDataParameters)
    MetaDataParametersPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // METADATAPARAMETERS_H
