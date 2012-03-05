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

#include <KDE/Kross/Action>
#include <KDE/Kross/Manager>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>

ExtractorFactory::ExtractorFactory(QObject *parent)
    : QObject(parent)
{
    loadScriptInfo();
}

ExtractorFactory::~ExtractorFactory()
{

}

WebExtractor *ExtractorFactory::createExtractor( const QString &webEngine )
{
    foreach(const WebExtractor::Info i, m_availableScripts) {
        if(i.identifier == webEngine) {

            KrossExtractor *ke = new KrossExtractor(i.file);
            return ke;
        }
    }

    return 0;
}

WebExtractor *ExtractorFactory::createExtractor( const QUrl &uri )
{
    return 0;
}

QList<WebExtractor::Info> ExtractorFactory::listAvailablePlugins( const QString &type )
{
    QList<WebExtractor::Info> pluginList;

    foreach(const WebExtractor::Info i, m_availableScripts) {
        if(i.resource == type) {
            pluginList.append(i);
        }
    }

    return pluginList;
}

void ExtractorFactory::loadScriptInfo()
{
    Kross::Action action(this, "MyScript");
    QString lookupurl("/home/joerg/Development/KDE/metadataextractor/lib/webextractor/plugins/");

    QDir dir( lookupurl );
    QFileInfoList list = dir.entryInfoList();

    foreach( const QFileInfo &fileInfo, list) {
        if( fileInfo.fileName() == QString(".") || fileInfo.fileName() == QString("..") ||
            fileInfo.completeSuffix() != QString("py")) {
            continue;
        }

        action.setFile( fileInfo.absoluteFilePath() );

        QVariantMap result = action.callFunction("info").toMap();

        WebExtractor::Info scriptInfo;
        scriptInfo.name = result.value("name").toString();
        scriptInfo.identifier = result.value("identifier").toString();
        scriptInfo.icon = result.value("icon").toString();
        scriptInfo.description = result.value("desscription").toString();
        scriptInfo.author = result.value("author").toString();
        scriptInfo.email = result.value("email").toString();
        scriptInfo.resource = result.value("resource").toString();
        scriptInfo.file = fileInfo.absoluteFilePath();

        m_availableScripts.append( scriptInfo );
    }
}
