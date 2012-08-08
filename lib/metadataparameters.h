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

#ifndef METADATAPARAMETERS_H
#define METADATAPARAMETERS_H

#include <KDE/KUrl>

#include <QtCore/QString>
#include <QtCore/QVariantMap>

namespace NepomukMetaDataExtractor {
    namespace Extractor {
       /**
         * @brief This struct holds all information about a file/reosurce we want to search additional meta data for
         *
         * Beside the url of the file location additional search parameters (from @c ResourceExtractor) necessary for the future websearch (@c WebExtractor) are saved in here.
         * The results of this websearch (all item details) are saved as @p metaData @c QVariantMap and precessed by the @c NepomukPipe
         */
        struct MetaDataParameters {
            //NOTE: replace single QString parameters with a search QMap i ncase we need to add many other parameters in the future?
            QString searchTitle;          /**< title of the entry to search for */
            QString searchAltTitle;       /**< alternative title of the entry to search for in case the first one does not return anything */
            QString searchShowTitle;
            QString searchEpisode;
            QString searchSeason;
            QString searchYearMin;
            QString searchYearMax;
            QString searchAuthor;
            QString searchJournal;

            QString resourceType;         /**< used to select the right set of python modules that allow retrieving data for this resoruce */
            KUrl resourceUri;             /**< the nepomuk or file url where the retrieved data will be added to */
            QVariantMap metaData;         /**< here all fetched data is pushed into @todo define proper api how the data should be saved in her (key=value list) */
            bool metaDataSaved;
        };
    }
}

#endif // METADATAPARAMETERS_H
