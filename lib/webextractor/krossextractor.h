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

#ifndef KROSSEXTRACTOR_H
#define KROSSEXTRACTOR_H

#include "webextractor.h"

#include <KDE/Kross/Action>

#include "nepomukmetadataextractor_export.h"

namespace NepomukMetaDataExtractor {
namespace Extractor {

class KrossExtractorPrivate;

/**
  * @brief Wrapper around the Kross framework to implement plugins in @c Python, @c Ruby or @c JavaScript
  *
  * The plugin itself is implemented in the scriptfile that will be passed via its constructor.
  * Afterwards the signals/slots are passed directly to the script.
  *
  * @see http://techbase.kde.org/Development/Tutorials#Kross
  *
  * All script plugins must implement the slots:
  * @li searchItems(QString, QVariantMap)
  * @li extractItemFromUri(QUrl, QVariantMap)
  *
  * and return its data via:
  * @li error(QString);
  * @li log(QString);
  * @li searchResults(QVariantList);
  * @li itemResults(QString, QVariantMap);
  *
  * For more details see \ref webextractor Overview
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT KrossExtractor : public WebExtractor
{
    Q_OBJECT
public:
    explicit KrossExtractor(const QString &scriptFile, QObject *parent = 0);
    ~KrossExtractor();

    WebExtractor::Info info();

    void search(const QString &resourceType, const QVariantMap &parameters);
    void extractItem(const QUrl &url, const QVariantMap &options);
    void showConfigDialog();

signals:
    void searchItems(const QString &resourceType, const QVariantMap &parameters);
    void extractItemFromUri(const QUrl &url, const QVariantMap &options);

    /**
     * @brief Internally used to overcome a problem with multiple nested dictionaries as return value in itemResults()
     *
     * Calls transformJSONResult and emits itemResults() with the transformed data
     * @param resourceType the resource type
     * @param jsonMap return values in json format
     */
    void itemResultsJSON(const QString &resourceType, const QString &jsonMap);

private slots:
    /**
     * @brief Little helper function to workaround a small bug with multiple nested dictionaries
     *
     * Transforms the metadata from a JSON representation into QVariantMap and emits itemResults
     *
     * @param resourceType resource type of the result
     * @param jsonMap json version of the meta data map
     */
    void transformJSONResult(const QString &resourceType, const QString &jsonMap);

private:
    Q_DECLARE_PRIVATE(KrossExtractor)
    KrossExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // KROSSEXTRACTOR_H
