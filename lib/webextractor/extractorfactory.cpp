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

#include "extractorfactory.h"

#include "krossextractor.h"
#include "tvdbextractor.h"

#include <KDE/Kross/Action>
#include <KDE/Kross/Manager>
#include <KDE/Kross/Interpreter>

#include <KDE/KStandardDirs>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>

#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor::Extractor;

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class ExtractorFactoryPrivate {
        public:
            QList<NepomukMetaDataExtractor::Extractor::WebExtractor::Info> availableScripts;
            TvdbExtractor *tvdbExtractor;
        };
    }
}

NepomukMetaDataExtractor::Extractor::ExtractorFactory::ExtractorFactory(QObject *parent)
    : QObject(parent)
    , d_ptr( new NepomukMetaDataExtractor::Extractor::ExtractorFactoryPrivate )

{
    loadScriptInfo();
}

NepomukMetaDataExtractor::Extractor::ExtractorFactory::~ExtractorFactory()
{

}

NepomukMetaDataExtractor::Extractor::WebExtractor *NepomukMetaDataExtractor::Extractor::ExtractorFactory::createExtractor( const QString &webEngine )
{
    Q_D( ExtractorFactory );

//    if(webEngine == QLatin1String("tvdbc++"))
//        return d->tvdbExtractor;

    foreach(const WebExtractor::Info i, d->availableScripts) {
        if( i.identifier.contains(webEngine) ) {

            kDebug() << "create KROSS web extractor for:" << i.name;
            KrossExtractor *ke = new KrossExtractor(i.file);
            return ke;
        }
    }

    return 0;
}

NepomukMetaDataExtractor::Extractor::WebExtractor *NepomukMetaDataExtractor::Extractor::ExtractorFactory::createExtractor( const QUrl &uri )
{
    Q_D( ExtractorFactory );

    //bad hack ... its late
//    foreach(const QString &urlregex, d->tvdbExtractor->info().urlregex) {
//        if(uri.toString().contains( urlregex )) {
//            return d->tvdbExtractor;
//        }
//    }

    foreach(const WebExtractor::Info i, d->availableScripts) {
        foreach(const QString &urlregex, i.urlregex) {
            if(uri.toString().contains( urlregex )) {

                kDebug() << "create KROSS web extractor for:" << i.name;
                KrossExtractor *ke = new KrossExtractor(i.file);
                return ke;
            }
        }
    }

    return 0;
}

QList<NepomukMetaDataExtractor::Extractor::WebExtractor::Info> NepomukMetaDataExtractor::Extractor::ExtractorFactory::listAvailablePlugins( const QString &type )
{
    Q_D( ExtractorFactory );

    QList<WebExtractor::Info> pluginList;

    foreach(const WebExtractor::Info i, d->availableScripts) {
        if(i.resource.contains(type) ) {
            pluginList.append(i);
        }
    }

    return pluginList;
}

void NepomukMetaDataExtractor::Extractor::ExtractorFactory::loadScriptInfo()
{
    Q_D( ExtractorFactory );

    QStringList acceptedFileTypes;
    QStringList interpreters = Kross::Manager::self().interpreters();
    kDebug() << "available interpreters" << interpreters;

    foreach(const QString &suffix, interpreters) {
        QString wildcard = Kross::Manager::self().interpreter(suffix)->interpreterInfo()->wildcard();
        acceptedFileTypes << wildcard.remove(QLatin1String("*."));
    }

    Kross::Action action(this, "ExtractorFactory");
    //QString lookupurl = KStandardDirs::locate("data", "nepomukmetadataextractor/plugins/");
    QString lookupurl = QString("/home/joerg/Development/KDE/metadataextractor/lib/webextractor/plugins/");

    kDebug() << "look for plugins on the folder" << lookupurl;

    QDir dir( lookupurl );
    QFileInfoList list = dir.entryInfoList();

    foreach( const QFileInfo &fileInfo, list) {

        if( !acceptedFileTypes.contains(fileInfo.completeSuffix()))
            continue;

        action.setFile( fileInfo.absoluteFilePath() );

        QVariantMap result = action.callFunction("info").toMap();

        WebExtractor::Info scriptInfo;
        scriptInfo.name = result.value("name").toString();
        scriptInfo.identifier = result.value("identifier").toString();
        scriptInfo.icon = result.value("icon").toString();
        scriptInfo.description = result.value("desscription").toString();
        scriptInfo.author = result.value("author").toString();
        scriptInfo.email = result.value("email").toString();
        scriptInfo.file = fileInfo.absoluteFilePath();

        QVariantList resList = result.value("resource").toList();
        kDebug() << resList;
        foreach(const QVariant &res, resList) {
            kDebug() << res;
            scriptInfo.resource << res.toString();
        }

        resList = result.value("urlregex").toList();
        kDebug() << resList;
        foreach(const QVariant &res, resList) {
            kDebug() << res;
            scriptInfo.urlregex << res.toString();
        }

        d->availableScripts.append( scriptInfo );

        kDebug() << "add plugon (" << scriptInfo.name << ") via file: " << scriptInfo.file;
    }

    // also add scriptinfo for the c++ tvdb class
//    d->tvdbExtractor = new TvdbExtractor;
//    d->availableScripts.append( d->tvdbExtractor->info() );

    kDebug() << "found " << d->availableScripts.size() << "search plugins";
}
