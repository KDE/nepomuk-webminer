/*
 * Copyright 2013 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#ifndef FILENAMEANALYZER_H
#define FILENAMEANALYZER_H

#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <KDE/KUrl>

#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Extractor
{
class MetaDataParameters;
class FilenameAnalyzerPrivate;

// only add new entry at the bottom, or else the KConfig stuff will break
// this relys on the enum <->int conversion
enum MatchTypes {
    MATCH_TITLE = 0,    /**< Title of the Book, music piece, tvshow episode */
    MATCH_YEAR = 1,     /**< Release date */

    MATCH_SHOW = 2,     /**< Name of the tv show */
    MATCH_SEASON = 3,   /**< TvShow season number */
    MATCH_EPISODE = 4,  /**< TvShow episode number */

    MATCH_PERSON = 5,   /**< name of the music artist, book author */
    MATCH_ALBUM = 6,    /**< name of the music album */
    MATCH_TRACK = 7     /**< number of the track on the album */
};

struct RegExpData {
    QRegExp regExp;
    QString example;
    QList<MatchTypes> matchList;
    bool useFolder;
};

class NEPOMUKWEBMINER_EXPORT FilenameAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit FilenameAnalyzer(QObject *parent = 0);

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
     * @brief Analyze the filename and writes additional search parameter into the object
     *
     * This is done by first strip all string the user supplied to be removed.
     * Cleanup the string (replace ". _" by whitespaces
     * Call the regexp to find the right parts of the string and save the found parts
     *
     * @param mdp the file/resource object where the data should be saved in
     * @param fileUrl the url of the file to be checked
     */
    void analyze(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl);

    /**
     * @brief Helper function to parse the KConfig entry for the regexp line
     *
     * Each regexp line is split by #|# and each entry is split by #,# as |;, could be part of the regexp this was chosen
     *
     * @param config the regeyp line that need to be parsed
     */
    static QList<NepomukWebMiner::Extractor::RegExpData> parseRegExpConfig(const QString &config);
    static QString writeRegExpConfig(const QList<NepomukWebMiner::Extractor::RegExpData> &data);

public Q_SLOTS:
    /**
     * @brief Reloads all user regexp from the "nepomuk-webminerrc" config file
     */
    void reloadRegExp();

private:
    /**
     * @brief Removes some parts of the filename based on what the user specified in KConfig
     *
     * @param fileName the filename that will be altered
     */
    void stripFileName(QString &fileName);

    /**
     * @brief Replaces "." and "_" by " "
     *
     * @param fileName the filename that will be altered
     */
    void cleanFileName(QString &fileName);

    /**
     * @brief Checks the filename against all supplied RegExpData and calls saveResult to save the data
     *
     * @param regExpList the list of RegExpData that should be used
     * @param mdp the MetaDataParameters that will be altered
     * @param filename the cleaned filename that should checked
     * @param fileUrl the cleaned filename + path that should be used
     *
     * @return @arg @c true if the one of the regexp matched
     *         @ar g@c false if no regexp matched
     */
    bool analyzeFileName(QList<Extractor::RegExpData> regExpList, NepomukWebMiner::Extractor::MetaDataParameters *mdp, const QString &filename, const QString &fileUrl);

    /**
     * @brief Saves the parsed data
     * @param mdp the MetaDataParameters that will be altered
     * @param text the found text
     * @param type the interpretation of what this text represents
     */
    void saveResult(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const QString &text, Extractor::MatchTypes type);

    Q_DECLARE_PRIVATE(FilenameAnalyzer)
    FilenameAnalyzerPrivate *const d_ptr; /**< d-pointer for this class */
};

}
}
#endif // FILENAMEANALYZER_H
