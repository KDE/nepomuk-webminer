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

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "MetaDataFetcherPlugin.h"

#ifndef H_MetaDataFetcherPluginAPI
#define H_MetaDataFetcherPluginAPI

#include <nepomukmetadataextractor/extractorfactory.h>
#include <nepomukmetadataextractor/automaticfetcher.h>

class MetaDataFetcherPluginAPI : public FB::JSAPIAuto
{
public:
    /**
     * @brief Constructor for your JSAPI object.
     *
     * You should register your methods, properties, and events
     * that should be accessible to Javascript from here.
     *
     * @see FB::JSAPIAuto::registerMethod
     * @see FB::JSAPIAuto::registerProperty
     * @see FB::JSAPIAuto::registerEvent
     */
    MetaDataFetcherPluginAPI(const MetaDataFetcherPluginPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host) {
        registerMethod("checkUrl",      make_method(this, &MetaDataFetcherPluginAPI::checkUrl));
        registerMethod("fetchMetaData", make_method(this, &MetaDataFetcherPluginAPI::fetchMetaData));

        m_ef = new NepomukMetaDataExtractor::Extractor::ExtractorFactory;
        m_automaticFetcher = new NepomukMetaDataExtractor::UI::AutomaticFetcher;
    }

    /**
     * @brief  Destructor.
     *
     * Remember that this object will not be released until
     * the browser is done with it; this will almost definitely be after
     * the plugin is released.
     */
    virtual ~MetaDataFetcherPluginAPI() {};

    /**
     * @brief  Gets a reference to the plugin that was passed in when the object was created.
     *
     * If the plugin has already been released then this will throw a @c FB::script_error
     * that will be translated into a javascript exception in the page.
     * */
    MetaDataFetcherPluginPtr getPlugin();

    // Method echo
    FB::variant checkUrl(const FB::variant& inputUrl);
    FB::variant fetchMetaData(const FB::variant& inputUrl);

private:
    MetaDataFetcherPluginWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    NepomukMetaDataExtractor::Extractor::ExtractorFactory *m_ef;
    NepomukMetaDataExtractor::UI::AutomaticFetcher *m_automaticFetcher;
};

#endif // H_MetaDataFetcherPluginAPI

