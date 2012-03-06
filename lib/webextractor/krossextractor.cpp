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

#include "krossextractor.h"

#include <KDE/Kross/Manager>

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class KrossExtractorPrivate {
        public:
            Kross::Action *scriptFile;
            WebExtractor::Info scriptInfo;
        };
    }
}

NepomukMetaDataExtractor::Extractor::KrossExtractor::KrossExtractor(const QString &scriptFile, QObject *parent)
    : WebExtractor(parent)
    , d_ptr( new NepomukMetaDataExtractor::Extractor::KrossExtractorPrivate )
{
    Q_D( KrossExtractor );

    d->scriptFile = new Kross::Action(this, "WebExtractor");

    d->scriptFile->addObject(this, "WebExtractor", Kross::ChildrenInterface::AutoConnectSignals);

    d->scriptFile->setFile( scriptFile );
    d->scriptFile->trigger();

    // load script info
    QVariantMap result = d->scriptFile->callFunction("info").toMap();
    d->scriptInfo.name = result.value("name").toString();
    d->scriptInfo.identifier = result.value("identifier").toString();
    d->scriptInfo.description = result.value("desscription").toString();
    d->scriptInfo.author = result.value("author").toString();
    d->scriptInfo.email = result.value("email").toString();
    d->scriptInfo.resource = result.value("resource").toString();
    d->scriptInfo.icon = result.value("icon").toString();
    d->scriptInfo.file = d->scriptFile->file();
}

NepomukMetaDataExtractor::Extractor::KrossExtractor::~KrossExtractor()
{
    Q_D( KrossExtractor );
    delete d->scriptFile;
}

NepomukMetaDataExtractor::Extractor::WebExtractor::Info NepomukMetaDataExtractor::Extractor::KrossExtractor::info()
{
    Q_D( KrossExtractor );
    return d->scriptInfo;
}

void NepomukMetaDataExtractor::Extractor::KrossExtractor::search(const QVariantMap &parameters)
{
    emit searchItems(parameters);
}

void NepomukMetaDataExtractor::Extractor::KrossExtractor::extractItem(const QUrl &url)
{
    emit extractItemFromUri(url);
}
