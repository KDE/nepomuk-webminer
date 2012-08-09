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

#ifndef AUDIOEXTRACTOR_H
#define AUDIOEXTRACTOR_H

#include <QtCore/QObject>
#include <KDE/KUrl>

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Extractor {

class MetaDataParameters;

class NEPOMUKMETADATAEXTRACTOR_EXPORT AudioExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Standard constructor
     *
     * @param parent some parent object
     */
    explicit AudioExtractor(QObject *parent = 0);

    void parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl);
    
private:
    /**
     * @brief Use TagLib to find additional meta data
     *
     * @param mdp the resource where the data is added to
     * @return @arg true if something could be found
     *         @arg false otherwise
     */
    bool findByTag( MetaDataParameters *mdp );

    /**
     * @brief Analyze the filename to find artist/title information
     * @param mdp the resource where the data is added to
     * @return @arg true if something could be found
     *         @arg false otherwise
     */
    bool findByFileName( MetaDataParameters *mdp );
};
}
}

#endif // AUDIOEXTRACTOR_H
