/**********************************************************\

  Auto-generated MetaDataFetcherPluginAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "MetaDataFetcherPluginAPI.h"

#include <nepomukmetadataextractor/webextractor.h>

#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QUrl>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant MetaDataFetcherPluginAPI::checkUrl(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant MetaDataFetcherPluginAPI::checkUrl(const FB::variant& inputUrl)
{
    std::string inputString   = inputUrl.cast<std::string>();
    QString urlString = QString::fromUtf8( inputString.data(), inputString.size() );
    QUrl url = QUrl(urlString);
    WebExtractor *we = m_ef->createExtractor( url );

    if(we)
        return true;
    else
        return false;
}

FB::variant MetaDataFetcherPluginAPI::fetchMetaData(const FB::variant& inputUrl)
{
    std::string inputString   = inputUrl.cast<std::string>();
    //system("metadataextractor -a -url ");
    system("conquirere");

    return "finished extraction";
}

///////////////////////////////////////////////////////////////////////////////
/// @fn MetaDataFetcherPluginPtr MetaDataFetcherPluginAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
MetaDataFetcherPluginPtr MetaDataFetcherPluginAPI::getPlugin()
{
    MetaDataFetcherPluginPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}
