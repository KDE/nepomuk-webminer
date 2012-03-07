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

#ifndef TVDB_H
#define TVDB_H

#include "webextractor.h"

#include <tvdb/series.h>

namespace Tvdb {
    class Client;
}

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class TvdbExtractor : public WebExtractor
        {
            Q_OBJECT
        public:
            explicit TvdbExtractor(QObject *parent = 0);
            ~TvdbExtractor();

            WebExtractor::Info info();

        public slots:
            void search(const QString &resourceType, const QVariantMap &parameters) ;
            void extractItem(const QUrl &url);

        private Q_SLOTS:
            void slotFinished( const Tvdb::Series& series );
            void slotMultipleResultsFound( const QList<Tvdb::Series>& series );

        private:
            Tvdb::Client* m_client;
            QList<Tvdb::Series> m_seriesCache;
            QVariantMap m_searchParameters;
        };
    }
}

#endif // TVDB_H
