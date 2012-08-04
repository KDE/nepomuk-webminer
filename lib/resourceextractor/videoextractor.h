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

#ifndef VIDEOEXTRACTOR_H
#define VIDEOEXTRACTOR_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QRegExp>

#include <KDE/KUrl>

namespace NepomukMetaDataExtractor {
namespace Extractor {

    class VideoExtractorPrivate;
    class MetaDataParameters;

/**
  * @brief Extracts some useful search parameters from the video filename or nepomuk resource
  *
  * partly copied from Sebastian Trüg's tvshowfilenameanalyzer
  *
  * The file/resource will be analized by its filename or if available some data from the nepomuk resource.
  * Based on this data (title, series, episode numer, season, year etc) a webbased search can be used to find the
  * exact item details.
  *
  * This class will determine if the given resource is a tvshow or a movie based on its regexp detection.
  * To help the class with this process some additional hints can be given
  * @see setTvShowMode
  * @see setTvShowNamesInFolders
  * @see setMovieMode
  *
  * @todo TODO: parse also meta data not only the filename
  * @todo TODO: the regexp mechanism is likely to fail, needs some better checks in here
  */
class VideoExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor that creates all necessary regular expressions
     *
     * @param parent some parrent
     */
    explicit VideoExtractor(QObject *parent = 0);

    /**
     * @brief Sets a hint, that the given file is a tvshow
     *
     * @param tvshowmode @arg true resource is a tvshow
     *                   @arg false resource is either a tvshow or movie (default)
     */
    void setTvShowMode(bool tvshowmode);

    /**
     * @brief sets a hint for tvshow resources to check also folder names for title/season/episode
     * @param useFolderNames @arg true check fodler names
     *                       @arg false do not check fodler names (default)
     */
    void setTvShowNamesInFolders(bool useFolderNames);

    /**
     * @brief Sets a hint, that the given file is a movie
     *
     * @param tvshowmode @arg true resource is a movie
     *                   @arg false resource is either a tvshow or movie (default)
     */
    void setMovieMode(bool moviemode);

    /**
     * @brief Parse the video filename at the given url
     *
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileUrl the url of the file on the harddrive
     */
    void parseUrl(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl = KUrl());

private:
    /**
     * @brief Parse all files in a given folder
     *
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileUrl filename that will be parsed
     * @param baseUrl the base folder url
     * @return @arg true if regexp matched some case
     *         @arg false if nothing matched
     */
    bool parseTvShowFolder(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl = KUrl());

    /**
     * @brief Parse a TvShow at a given fileurl
     *
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileName filename that will be parsed
     * @param baseUrl the base folder url
     * @return @arg true if regexp matched some case
     *         @arg false if nothing matched
     */
    bool parseTvShowFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName);

    /**
     * @brief Parse a Movie at a given fileurl
     *
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileName filename that will be parsed
     * @param baseUrl the base folder url
     * @return @arg true if regexp matched some case
     *         @arg false if nothing matched
     */
    bool parseMovieFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName);

    Q_DECLARE_PRIVATE(VideoExtractor)
    VideoExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // VIDEOEXTRACTOR_H
