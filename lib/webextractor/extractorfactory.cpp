/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
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

using namespace NepomukWebMiner::Extractor;

namespace NepomukWebMiner
{
namespace Extractor
{
class ExtractorFactoryPrivate
{
public:
    QList<NepomukWebMiner::Extractor::WebExtractor::Info> availableScripts;
    QList<NepomukWebMiner::Extractor::WebExtractor::Info> failedScripts;
    QList<NepomukWebMiner::Extractor::WebExtractor*> existingExtractors;
};
}
}

NepomukWebMiner::Extractor::ExtractorFactory::ExtractorFactory(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukWebMiner::Extractor::ExtractorFactoryPrivate)

{
    loadScriptInfo();
}

NepomukWebMiner::Extractor::ExtractorFactory::~ExtractorFactory()
{
    Q_D(ExtractorFactory);
    while (!d->existingExtractors.isEmpty()) {
        WebExtractor *we = d->existingExtractors.takeFirst();
        we->deleteLater();
    }
}

NepomukWebMiner::Extractor::WebExtractor *NepomukWebMiner::Extractor::ExtractorFactory::getExtractor(const QString &webEngine)
{
    Q_D(ExtractorFactory);

    // first check if we already created the extractor and reuse it in this case
    foreach (WebExtractor * we, d->existingExtractors) {
        if (we->info().identifier == webEngine) {
            return we;
        }
    }

    // other wise create a new one
    foreach (const WebExtractor::Info i, d->availableScripts) {
        if (i.identifier == webEngine) {

            kDebug() << "create KROSS web extractor for:" << i.name;
            KrossExtractor *ke = new KrossExtractor(i.file);
            d->existingExtractors.append(ke);
            return ke;
        }
    }

    kWarning() << "Could not find the rigth KrossExtractor with identifier :: " << webEngine;

    return 0;
}

NepomukWebMiner::Extractor::WebExtractor *NepomukWebMiner::Extractor::ExtractorFactory::getExtractor(const QUrl &uri)
{
    Q_D(ExtractorFactory);

    // first check if we already created the extractor and reuse it in this case
    foreach (WebExtractor * we, d->existingExtractors) {
        foreach (const QString & urlregex, we->info().urlregex) {
            if (uri.toString().contains(QRegExp(urlregex))) {
                return we;
            }
        }
    }

    // otherwise create a new one
    foreach (const WebExtractor::Info i, d->availableScripts) {
        foreach (const QString & urlregex, i.urlregex) {
            if (uri.toString().contains(QRegExp(urlregex))) {

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

bool NepomukWebMiner::Extractor::ExtractorFactory::urlSupported(const QUrl &uri)
{
    Q_D(ExtractorFactory);

    foreach (const WebExtractor::Info i, d->availableScripts) {
        foreach (const QString & urlregex, i.urlregex) {
            if (uri.toString().contains(QRegExp(urlregex))) {
                return true;
            }
        }
    }

    return false;
}

QList<NepomukWebMiner::Extractor::WebExtractor::Info> NepomukWebMiner::Extractor::ExtractorFactory::listAvailablePlugins(const QString &type)
{
    Q_D(ExtractorFactory);

    QList<WebExtractor::Info> pluginList;

    foreach (const WebExtractor::Info i, d->availableScripts) {
        if (i.resource.contains(type)) {
            pluginList.append(i);
        }
    }

    return pluginList;
}

QList<NepomukWebMiner::Extractor::WebExtractor::Info> NepomukWebMiner::Extractor::ExtractorFactory::listFailedPlugins(const QString &type)
{
    Q_D(ExtractorFactory);

    QList<WebExtractor::Info> pluginList;

    foreach (const WebExtractor::Info i, d->failedScripts) {
        if (i.resource.contains(type)) {
            pluginList.append(i);
        }
    }

    return pluginList;
}

void NepomukWebMiner::Extractor::ExtractorFactory::loadScriptInfo()
{
    Q_D(ExtractorFactory);

    QStringList acceptedFileTypes;
    QStringList interpreters = Kross::Manager::self().interpreters();
    kDebug() << "available interpreters" << interpreters;

    foreach (const QString & suffix, interpreters) {
        QString wildcard = Kross::Manager::self().interpreter(suffix)->interpreterInfo()->wildcard();
        acceptedFileTypes << wildcard.remove(QLatin1String("*."));
    }

    Kross::Action action(this, "ExtractorFactory");
    QStringList pluginDirs;

    pluginDirs << KStandardDirs::locate("data", "nepomuk-webminer/plugins/");
    if(!MDESettings::additionalPluginFolder().isEmpty()) {
        pluginDirs << MDESettings::additionalPluginFolder();
    }

    QFileInfoList pluginList;
    foreach (const QString & folder, pluginDirs) {

        QDir dir(folder);
        if (dir.exists()) {
            kDebug() << "look for plugins in the folder: " << folder;
            pluginList.append(dir.entryInfoList());
        }
    }

    KConfig config("nepomuk-webminerrc");

    foreach (const QFileInfo & fileInfo, pluginList) {

        if (!acceptedFileTypes.contains(fileInfo.completeSuffix())) {
            continue;
        }

        bool readFromCache = false;
        if (config.hasGroup(fileInfo.absoluteFilePath())) {
            KConfigGroup pluginGroup(&config, fileInfo.absoluteFilePath());

            QDateTime cacheDate = pluginGroup.readEntry("cachedate", QDateTime());
            if (fileInfo.lastModified() <= cacheDate) {
                readFromCache = true;

                WebExtractor::Info scriptInfo;
                scriptInfo.name = pluginGroup.readEntry("name", QString());
                scriptInfo.homepage = pluginGroup.readEntry("homepage", QString());
                scriptInfo.identifier = pluginGroup.readEntry("identifier", QString());
                scriptInfo.icon = pluginGroup.readEntry("icon", QString());
                scriptInfo.description = pluginGroup.readEntry("description", QString());
                scriptInfo.hasConfig = pluginGroup.readEntry("hasConfig", QVariant(false)).toBool();
                scriptInfo.author = pluginGroup.readEntry("author", QString());
                scriptInfo.email = pluginGroup.readEntry("email", QString());
                scriptInfo.file = pluginGroup.readEntry("file", QString());
                scriptInfo.resource = pluginGroup.readEntry("resource", QStringList());
                scriptInfo.urlregex = pluginGroup.readEntry("urlregex", QStringList());

                kDebug() << "add plugin (" << scriptInfo.name << ") via KConfig cache for file: " << scriptInfo.file;
                d->availableScripts.append(scriptInfo);
            }
        }


        if (!readFromCache) {

            action.setFile(fileInfo.absoluteFilePath());

            QVariantMap result = action.callFunction("info").toMap();

            WebExtractor::Info scriptInfo;
            scriptInfo.name = result.value("name").toString();
            scriptInfo.homepage = result.value("homepage").toString();
            scriptInfo.identifier = result.value("identifier").toString();

            QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + result.value("icon").toString();
            scriptInfo.icon = iconPath;

            scriptInfo.description = result.value("description").toString();
            if (result.contains("hasConfig")) {
                scriptInfo.hasConfig = result.value("hasConfig").toBool();
            } else {
                scriptInfo.hasConfig = false;
            }
            scriptInfo.author = result.value("author").toString();
            scriptInfo.email = result.value("email").toString();
            scriptInfo.file = fileInfo.absoluteFilePath();

            QVariantList resList = result.value("resource").toList();
            foreach (const QVariant & res, resList) {
                scriptInfo.resource << res.toString();
            }

            resList = result.value("urlregex").toList();
            foreach (const QVariant & res, resList) {
                scriptInfo.urlregex << res.toString();
            }

            if (result.value(QLatin1String("isAvailable")).toBool() == false) {
                kDebug() << "failed loading the plugin: " << result.value("name").toString() << " from:" << fileInfo.absoluteFilePath()
                         << " Reason:" << result.value("errorMsg").toString();
                scriptInfo.error = result.value("errorMsg").toString();
                d->failedScripts.append(scriptInfo);
                continue;
            }

            scriptInfo.error = ""; // no error
            kDebug() << "add plugin (" << scriptInfo.name << ") via file: " << scriptInfo.file;
            d->availableScripts.append(scriptInfo);

            // and add info to the config as chaced values
            KConfigGroup pluginGroup(&config, scriptInfo.file);
            pluginGroup.writeEntry("cachedate", fileInfo.lastModified());
            pluginGroup.writeEntry("name", scriptInfo.name);
            pluginGroup.writeEntry("homepage", scriptInfo.homepage);
            pluginGroup.writeEntry("identifier", scriptInfo.identifier);
            pluginGroup.writeEntry("icon", scriptInfo.icon);
            pluginGroup.writeEntry("description", scriptInfo.description);
            pluginGroup.writeEntry("hasConfig", QVariant(scriptInfo.hasConfig));
            pluginGroup.writeEntry("icon", scriptInfo.icon);
            pluginGroup.writeEntry("author", scriptInfo.author);
            pluginGroup.writeEntry("email", scriptInfo.email);
            pluginGroup.writeEntry("file", scriptInfo.file);
            pluginGroup.writeEntry("resource", scriptInfo.resource);
            pluginGroup.writeEntry("urlregex", scriptInfo.urlregex);
            pluginGroup.config()->sync();
        }
    }

    kDebug() << "found " << d->availableScripts.size() << "search plugins";
}
