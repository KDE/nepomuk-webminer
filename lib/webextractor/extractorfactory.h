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

#ifndef EXTRACTORFACTORY_H
#define EXTRACTORFACTORY_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QList>

#include "webextractor.h"
#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace Extractor
{

class ExtractorFactoryPrivate;

/**
  * @brief This factory takes care of retrieving the right plugin for the metadata retrieval.
  *
  * Its purpose is to get a list of available search engines based on the type of resource that will be looked up
  *
  * @li publication
  * @li tvshow
  * @li movie
  * @li music
  *
  * Or return the correct search engine based on its <i>unique identifier</i> or the <i>webpage url</i> where the item can be extracted from
  *
  * The WebExtarctor implementations take care of the actual item search and metadata extraction from the web resource.
  *
  * @see KrossExtractor - for the python/ruby/javascript part
  * @see WebExtractor - for the general case and if one wants to create a c++ based plugin
  */
class NEPOMUKWEBMINER_EXPORT ExtractorFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Standard Constructor
     *
     * @param parent some parent object
     */
    explicit ExtractorFactory(QObject *parent = 0);

    /**
     * @brief Destructor
     *
     * Cleans up all existing WebExtractor resources
     */
    ~ExtractorFactory();

    /**
      * @brief Creates a new WebExtractor based on the @c "identifier" name
      *
      * The Factory manag the extractor resources and deletes them once the factory gets deleted
      *
      * @p webEngine short identifier name of the plugin
      *
      * @return the WebExtractor for the identifier
      */
    NepomukWebMiner::Extractor::WebExtractor *getExtractor(const QString &webEngine);

    /**
      * @brief Creates a new WebExtractor based in the @c "url" it can fetch data from
      *
      * The Factory manag the extractor resources and deletes them once the factory gets deleted
      *
      * @p uri web url that should be parsed
      *
      * @return the WebExtractor that can handle the uri
      */
    NepomukWebMiner::Extractor::WebExtractor *getExtractor(const QUrl &uri);

    /**
     * @brief Checks the already parsed WebExtractor::Info objects if the given url is supported
     *
     * Used for the NPAPI plugin to check if we can use the webminer
     *
     * @param uri  web url that should be parsed
     * @return @arg @c true if there exist a plugin that supports the url
     *         @arg @c false if no plugin could parse the url
     */
    bool urlSupported(const QUrl &uri);

    /**
      * @brief Returns a list of all available webextractor plugins that fetches data for the resource @c type
      *
      * @p type can be:
      * @li publication
      * @li tvshow
      * @li movie
      * @li music
      *
      * @return a list of all WebExtarctor info items
      */
    QList<NepomukWebMiner::Extractor::WebExtractor::Info> listAvailablePlugins(const QString &type);

    /**
      * @brief Returns a list of webextractor plugins for the resource @c type that failed to load
      *
      * @p type can be:
      * @li publication
      * @li tvshow
      * @li movie
      * @li music
      *
      * @return a list of all WebExtarctor info items
      */
    QList<NepomukWebMiner::Extractor::WebExtractor::Info> listFailedPlugins(const QString &type);

private:
    /**
      * @brief Preloads information about all available plugins
      *
      * Plugins are scripts in the kde data folder under @c nepomukwebminer/plugins/
      */
    void loadScriptInfo();

    Q_DECLARE_PRIVATE(ExtractorFactory)
    ExtractorFactoryPrivate *const d_ptr; /**< d-pointer for this class */

};
}
}

#endif // EXTRACTORFACTORY_H
