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
          */
        class VideoExtractor : public QObject
        {
            Q_OBJECT
        public:
            explicit VideoExtractor(QObject *parent = 0, bool tvshowOnly = false);

            void parseUrl(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl = KUrl());

        private:
            bool parseTvShowFolder(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl = KUrl());
            bool parseTvShowFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName);
            bool parseMovieFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName);

            Q_DECLARE_PRIVATE(VideoExtractor)
            VideoExtractorPrivate *const d_ptr; /**< d-pointer for this class */
        };
    }
}

#endif // VIDEOEXTRACTOR_H
