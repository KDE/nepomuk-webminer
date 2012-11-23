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

#ifndef AUDIOEXTRACTOR_H
#define AUDIOEXTRACTOR_H

#include <QtCore/QObject>
#include <KDE/KUrl>

#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Extractor
{

class AudioExtractorPrivate;
class MetaDataParameters;

/**
 * @brief Extarctor to analyze audio files
 *
 * This class uses @c TagLib to read the ID3 tags of any music file and retrieve basic information from it for the search.
 *
 * @todo TODO: Add regular expressions to analyze the filename and get information from there if TagLib didn't find anything
 */
class NEPOMUKWEBMINER_EXPORT AudioExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Standard constructor
     *
     * @param parent some parent object
     */
    explicit AudioExtractor(QObject *parent = 0);

    /**
     * @brief starts the parsing of the file
     *
     * @param mdp the MetaDataParameters where all data will be added to
     * @param fileUrl the url of the file
     */
    void parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl);

private:
    /**
     * @brief Use TagLib to find additional meta data
     *
     * @param mdp the resource where the data is added to
     * @return @arg true if something could be found
     *         @arg false otherwise
     */
    bool findByTag(MetaDataParameters *mdp);

    /**
     * @brief Analyze the filename to find artist/title information
     *
     * @param mdp the resource where the data is added to
     * @return @arg true if something could be found
     *         @arg false otherwise
     */
    bool findByFileName(MetaDataParameters *mdp);

    Q_DECLARE_PRIVATE(AudioExtractor)
    AudioExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // AUDIOEXTRACTOR_H
