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

#ifndef NEPOMUKPIPE_H
#define NEPOMUKPIPE_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QUrl>

class KJob;

namespace NepomukMetaDataExtractor {
    namespace Pipe {

        /**
          * @brief This part is used to import a plain QVariantMap into Nepomuk
          *
          * The data received from the internet is nothing else than a list of key=value pairs.
          * To save one the trouble of writing the Nepomuk parts that does the semantic transition from such an key=value
          * list to the right ontology parts, this pipe appraoch exist.
          *
          * It is not meant to cover all of Nepomuks ontoloy but rather a small defined subset of items that will be fetched
          * from the internet.
          */
        class NepomukPipe : public QObject
        {
            Q_OBJECT
        public:
            explicit NepomukPipe(QObject *parent = 0);
            virtual ~NepomukPipe();

            virtual void pipeImport(const QVariantMap &entry) = 0;

        private slots:
            void slotSaveToNepomukDone(KJob *job);
        };
    }
}

#endif // NEPOMUKPIPE_H
