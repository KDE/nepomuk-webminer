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
  * @brief copy of Sebastian Trüg's tvshowfilenameanalyzer
  *
  * Defines the file as TvShow if it has S01E0E or simmilar in its name
  * Otherwise it is a movie
  *
  * @todo TODO: parse also meta data not only the filename
  */
class VideoExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor that creates all necessary regular expressions
     * @param parent some parrent
     * @param tvshowOnly if @arg true will match only tv shows
     *                   if @arg false will match also movie titles
     */
    explicit VideoExtractor(QObject *parent = 0, bool tvshowOnly = false);

    /**
     * @brief Parse the video filename at the given url
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileUrl the url of the file on the harddrive
     */
    void parseUrl(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl = KUrl());

private:
    /**
     * @brief Parse all files in a given folder
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileUrl filename that will be parsed
     * @param baseUrl the base folder url
     * @return @arg true if regexp matched some case
     *         @arg false if nothing matched
     */
    bool parseTvShowFolder(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl = KUrl());

    /**
     * @brief Parse a TvShow at a given fileurl
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileName filename that will be parsed
     * @param baseUrl the base folder url
     * @return @arg true if regexp matched some case
     *         @arg false if nothing matched
     */
    bool parseTvShowFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName);

    /**
     * @brief Parse a Movie at a given fileurl
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
