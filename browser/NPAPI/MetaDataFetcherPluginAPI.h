/**********************************************************\

  Auto-generated MetaDataFetcherPluginAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "MetaDataFetcherPlugin.h"

#ifndef H_MetaDataFetcherPluginAPI
#define H_MetaDataFetcherPluginAPI

#include <nepomukmetadataextractor/extractorfactory.h>

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class ExtractorFactory;
    }
}

class MetaDataFetcherPluginAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn MetaDataFetcherPluginAPI::MetaDataFetcherPluginAPI(const MetaDataFetcherPluginPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    MetaDataFetcherPluginAPI(const MetaDataFetcherPluginPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("checkUrl",      make_method(this, &MetaDataFetcherPluginAPI::checkUrl));
        registerMethod("fetchMetaData", make_method(this, &MetaDataFetcherPluginAPI::fetchMetaData));

        m_ef = new NepomukMetaDataExtractor::Extractor::ExtractorFactory;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn MetaDataFetcherPluginAPI::~MetaDataFetcherPluginAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~MetaDataFetcherPluginAPI() {};

    MetaDataFetcherPluginPtr getPlugin();

    // Method echo
    FB::variant checkUrl(const FB::variant& inputUrl);
    FB::variant fetchMetaData(const FB::variant& inputUrl);

private:
    MetaDataFetcherPluginWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    NepomukMetaDataExtractor::Extractor::ExtractorFactory *m_ef;
};

#endif // H_MetaDataFetcherPluginAPI

