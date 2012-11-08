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

#include "MetaDataFetcherPluginAPI.h"

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include <nepomukmetadataextractor/webextractor.h>

#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QUrl>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;
using namespace UI;

FB::variant MetaDataFetcherPluginAPI::checkUrl(const FB::variant& inputUrl)
{
    std::string inputString   = inputUrl.cast<std::string>();
    QString urlString = QString::fromUtf8(inputString.data(), inputString.size());
    QUrl url = QUrl(urlString);
    WebExtractor *we = m_ef->createExtractor(url);

    if (we) {
        delete we;
        return true;
    } else {
        delete we;
        return false;
    }
}

FB::variant MetaDataFetcherPluginAPI::fetchMetaData(const FB::variant& inputUrl)
{
    std::string inputUrlString   = inputUrl.cast<std::string>();

    std::string command = "metadataextractor -auto -url ";
    command.append(inputUrlString);

    // calls the commandline program to do the data fetching and nepomuk importing
    // even better would be to use the WebExtractor and NepomukPipe classes directly
    // but I need the Qt signal/slot mechanism then :/
    system(command.c_str());

    return "finished extraction of :: " + inputUrlString;
}

MetaDataFetcherPluginPtr MetaDataFetcherPluginAPI::getPlugin()
{
    MetaDataFetcherPluginPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}
