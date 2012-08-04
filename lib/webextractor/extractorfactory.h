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

#ifndef EXTRACTORFACTORY_H
#define EXTRACTORFACTORY_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QList>

#include "webextractor.h"
#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Extractor {

class ExtractorFactoryPrivate;

/**
  * @brief This factory takes care of retrieving the right plugin for the metadata retrieval.
  *
  * Its purpose is to get a list of available search engines based on the type of resource that will be looked up
  *
  * @li publication
  * @li tvshow
  * @li movie
  *
  * Or return the correct search engine based on its unique identifier or the webpage url where the item can be extarcted from
  *
  * The WebExtarctor implementations take care of the actual extraction job.
  *
  * @see KrossExtractor - for the python/ruby/javascript part
  * @see WebExtractor - for the general case and if one wants to create a c++ based plugin
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT ExtractorFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Standard Constructor
     * @param parent some parent object
     */
    explicit ExtractorFactory(QObject *parent = 0);
    ~ExtractorFactory();

    /**
      * @brief Creates a new WebExtractor based on the @c "identifier" name
      *
      * Ownership of the created object is transfered to the caller.
      * The caller must delete it, if it is no longer used
      *
      * @p webEngine short identifier name of the plugin
      *
      * @return the created WebExtractor
      */
    NepomukMetaDataExtractor::Extractor::WebExtractor *createExtractor( const QString &webEngine );

    /**
      * @brief Creates a new WebExtractor based in the @c "url" it can fetch data from
      *
      * Ownership of the created object is transfered to the caller.
      * The caller must delete it, if it is no longer used
      *
      * @p uri web url that should be parsed
      *
      * @return the created WebExtractor
      */
    NepomukMetaDataExtractor::Extractor::WebExtractor *createExtractor( const QUrl &uri );

    /**
      * @brief Returns a list of all available webextractor plugins that fetches data for the resource @c type
      *
      * @p type can be:
      * @li publication
      * @li tvshow
      * @li movie
      *
      * @return a list of all WebExtarctor info items
      */
    QList<NepomukMetaDataExtractor::Extractor::WebExtractor::Info> listAvailablePlugins( const QString &type );

private:
    /**
      * @brief Preloads information about all available plugins
      *
      * Plugins are scripts in the kde data folder under @c nepomukmetadataextractor/plugins/
      */
    void loadScriptInfo();

    Q_DECLARE_PRIVATE(ExtractorFactory)
    ExtractorFactoryPrivate *const d_ptr; /**< d-pointer for this class */

};
}
}

#endif // EXTRACTORFACTORY_H
