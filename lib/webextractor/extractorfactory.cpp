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
#include <mdesettings.h>

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
    QList<NepomukMetaDataExtractor::Extractor::WebExtractor*> existingExtractors;
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
    Q_D( ExtractorFactory );
    while( !d->existingExtractors.isEmpty() ) {
        WebExtractor *we = d->existingExtractors.takeFirst();
        we->deleteLater();
    }
}

NepomukMetaDataExtractor::Extractor::WebExtractor *NepomukMetaDataExtractor::Extractor::ExtractorFactory::getExtractor( const QString &webEngine )
{
    Q_D( ExtractorFactory );

    // first check if we already created the extractor and reuse it in this case
    foreach(WebExtractor *we, d->existingExtractors) {
        if(we->info().identifier == webEngine) {
            return we;
        }
    }

    // other wise create a new one
    foreach(const WebExtractor::Info i, d->availableScripts) {
        if( i.identifier == webEngine ) {

            kDebug() << "create KROSS web extractor for:" << i.name;
            KrossExtractor *ke = new KrossExtractor(i.file);
            d->existingExtractors.append(ke);
            return ke;
        }
    }

    kWarning() << "Could not find the rigth KrossExtractor with identifier :: " << webEngine;

    return 0;
}

NepomukMetaDataExtractor::Extractor::WebExtractor *NepomukMetaDataExtractor::Extractor::ExtractorFactory::getExtractor( const QUrl &uri )
{
    Q_D( ExtractorFactory );

    // first check if we already created the extractor and reuse it in this case
    foreach(WebExtractor *we, d->existingExtractors) {
        foreach(const QString &urlregex, we->info().urlregex) {
            if(uri.toString().contains( QRegExp(urlregex) )) {
                return we;
            }
        }
    }

    // other wise create a new one
    foreach(const WebExtractor::Info i, d->availableScripts) {
        foreach(const QString &urlregex, i.urlregex) {
            if(uri.toString().contains( QRegExp(urlregex) )) {

                kDebug() << "create KROSS web extractor for:" << i.name;
                KrossExtractor *ke = new KrossExtractor(i.file);
                d->existingExtractors.append(ke);
                return ke;
            }
        }
    }

    kWarning() << "Could not find the rigth KrossExtractor for url :: " << uri;
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
    QStringList pluginDirs;

    pluginDirs << KStandardDirs::locate("data", "nepomukmetadataextractor/plugins/");
    pluginDirs << MDESettings::additionalPluginFolder();;

    QFileInfoList pluginList;
    foreach(const QString &folder, pluginDirs) {

        QDir dir( folder );
        if( dir.exists() ) {
            kDebug() << "look for plugins in the folder: " << folder;
            pluginList.append( dir.entryInfoList() );
        }
    }

    foreach( const QFileInfo &fileInfo, pluginList) {

        if( !acceptedFileTypes.contains(fileInfo.completeSuffix())) {
            continue;
        }

        action.setFile( fileInfo.absoluteFilePath() );

        QVariantMap result = action.callFunction("info").toMap();

        if(result.value(QLatin1String("isAvailable")).toBool() == false) {
            kDebug() << "failed loading the plugin: " << result.value("name").toString() << " from:" << fileInfo.absoluteFilePath()
                     << " Reason:" << result.value("errorMsg").toString();
            continue;
        }

        WebExtractor::Info scriptInfo;
        scriptInfo.name = result.value("name").toString();
        scriptInfo.identifier = result.value("identifier").toString();
        scriptInfo.icon = result.value("icon").toString();
        scriptInfo.description = result.value("desscription").toString();
        scriptInfo.author = result.value("author").toString();
        scriptInfo.email = result.value("email").toString();
        scriptInfo.file = fileInfo.absoluteFilePath();

        QVariantList resList = result.value("resource").toList();
        foreach(const QVariant &res, resList) {
            scriptInfo.resource << res.toString();
        }

        resList = result.value("urlregex").toList();
        foreach(const QVariant &res, resList) {
            scriptInfo.urlregex << res.toString();
        }

        d->availableScripts.append( scriptInfo );

        kDebug() << "add plugin (" << scriptInfo.name << ") via file: " << scriptInfo.file;
    }

    kDebug() << "found " << d->availableScripts.size() << "search plugins";
}
