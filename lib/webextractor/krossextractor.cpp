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
#include <KDE/KDebug>

#include <qjson/parser.h>

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

    connect(this, SIGNAL(itemResultsJSON(QString,QString)), this, SLOT(transformJSONResult(QString,QString)) );

    // each cross action has its own unique identifier
    d->scriptFile = new Kross::Action(this, QString("WebExtractor-%1").arg(scriptFile));

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

    QFileInfo fileInfo(scriptFile);
    QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + result.value("icon").toString();
    d->scriptInfo.icon = iconPath;

    d->scriptInfo.file = d->scriptFile->file();

    QVariantList resList = result.value("resource").toList();
    foreach(const QVariant &res, resList) {
        d->scriptInfo.resource << res.toString();
    }

    resList = result.value("urlregex").toList();
    foreach(const QVariant &res, resList) {
        d->scriptInfo.urlregex << res.toString();
    }
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

void NepomukMetaDataExtractor::Extractor::KrossExtractor::search(const QString &resourceType, const QVariantMap &parameters)
{
    emit searchItems(resourceType, parameters);
}

void NepomukMetaDataExtractor::Extractor::KrossExtractor::extractItem(const QUrl &url, const QVariantMap &options)
{
    emit extractItemFromUri(url, options);
}

void NepomukMetaDataExtractor::Extractor::KrossExtractor::transformJSONResult(const QString &resourceType, const QString &jsonMap)
{
    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse (jsonMap.toLatin1(), &ok).toMap();
    if (!ok) {
        qFatal("KrossExtractor::transformJSONResult :: An error occurred during json parsing");
        return;
    }

    emit itemResults(resourceType, result);
}
