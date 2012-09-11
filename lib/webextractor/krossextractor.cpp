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

#include <KDE/KConfig>
#include <KDE/KConfigGroup>

#include <QtCore/QFutureWatcher>
#include <QtCore/QtConcurrentRun>

#include <qjson/parser.h>

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class KrossExtractorPrivate {
        public:
            Kross::Action *scriptFile;
            WebExtractor::Info scriptInfo;
            QFutureWatcher<QVariant> *futureWatcher;
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

    KConfig config("nepomukmetadataextractorrc");
    if( config.hasGroup( scriptFile ) ) {
        KConfigGroup pluginGroup( &config, scriptFile );

        d->scriptInfo.name = pluginGroup.readEntry("name", QString());
        d->scriptInfo.homepage = pluginGroup.readEntry("homepage", QString());
        d->scriptInfo.identifier = pluginGroup.readEntry("identifier", QString());
        d->scriptInfo.icon = pluginGroup.readEntry("icon", QString());
        d->scriptInfo.description = pluginGroup.readEntry("description", QString());
        d->scriptInfo.author = pluginGroup.readEntry("author", QString());
        d->scriptInfo.email = pluginGroup.readEntry("email", QString());
        d->scriptInfo.file = pluginGroup.readEntry("file", QString());
        d->scriptInfo.resource = pluginGroup.readEntry("resource", QStringList());
        d->scriptInfo.urlregex = pluginGroup.readEntry("urlregex", QStringList());
    }
    else {
        // load script info
        QVariantMap result = d->scriptFile->callFunction("info").toMap();
        d->scriptInfo.name = result.value("name").toString();
        d->scriptInfo.homepage = result.value("homepage").toString();
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
}

NepomukMetaDataExtractor::Extractor::KrossExtractor::~KrossExtractor()
{
    Q_D( KrossExtractor );
    delete d->scriptFile;
    delete d->futureWatcher;
}

NepomukMetaDataExtractor::Extractor::WebExtractor::Info NepomukMetaDataExtractor::Extractor::KrossExtractor::info()
{
    Q_D( KrossExtractor );
    return d->scriptInfo;
}

static QVariant concurrentSearch(Kross::Action *script, const QString &resourceType, const QVariantMap &parameters)
{
    return script->callFunction(QString("searchItems"), QVariantList() << resourceType << parameters);
}

void NepomukMetaDataExtractor::Extractor::KrossExtractor::search(const QString &resourceType, const QVariantMap &parameters)
{
    Q_D( KrossExtractor );

    if( d->scriptInfo.identifier == "imdbmovies") {
        kDebug() << "use workaround for imdb. This once crashes if executed in its own thread";
        emit searchItems( resourceType, parameters );
    }
    else {
        QFuture<QVariant> future = QtConcurrent::run(concurrentSearch, d->scriptFile,resourceType, parameters);
        d->futureWatcher = new QFutureWatcher<QVariant>();

        d->futureWatcher->setFuture(future);
    }
}

static QVariant concurrentExtraction(Kross::Action *script, const QUrl &url, const QVariantMap &options)
{
    return script->callFunction(QString("extractItemFromUri"), QVariantList() << url << options);
}

void NepomukMetaDataExtractor::Extractor::KrossExtractor::extractItem(const QUrl &url, const QVariantMap &options)
{
    Q_D( KrossExtractor );
    if( d->scriptInfo.identifier == "imdbmovies") {
        kDebug() << "use workaround for imdb. This once crashes if executed in its own thread";
        emit extractItemFromUri( url, options );
    }
    else {
        QFuture<QVariant> future = QtConcurrent::run(concurrentExtraction, d->scriptFile, url, options);
        d->futureWatcher = new QFutureWatcher<QVariant>();

        d->futureWatcher->setFuture(future);
    }
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
