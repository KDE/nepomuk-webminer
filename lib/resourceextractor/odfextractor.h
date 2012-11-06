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

#ifndef ODFEXTRACTOR_H
#define ODFEXTRACTOR_H

#include <QtCore/QObject>
#include <KDE/KUrl>

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Extractor {

class OdfExtractorPrivate;
class MetaDataParameters;

/**
 * @brief The OdfExtractor parse some additional data from .odt files
 *
 * The following data fields from the embedded xml file will be used:
 * @li @c dc:title for the @c title key
 * @li @c meta:keyword for the @c keywords key
 * @li @c dc:subject for the @c keywords key
 * @li @c meta:page-count
 * @li @c dc:description for the @c note key
 */
class NEPOMUKMETADATAEXTRACTOR_EXPORT OdfExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Standard constructor
     *
     * @param parent some parent object
     */
    explicit OdfExtractor(QObject *parent = 0);

    /**
     * @brief Parse the odf document at the given url
     *
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileUrl the url of the file on the harddrive
     */
    void parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl);

private:
    Q_DECLARE_PRIVATE(OdfExtractor)
    OdfExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // ODFEXTRACTOR_H
