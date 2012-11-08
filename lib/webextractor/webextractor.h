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

#ifndef WEBEXTRACTOR_H
#define WEBEXTRACTOR_H

#include "nepomukmetadataextractor_export.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

namespace NepomukMetaDataExtractor
{
namespace Extractor
{

/**
 * @brief Interface for all WebExtractor implementations
 *
 * This is the main interface that all Webextractors have to implement.
 * This is mainly used for the @c KrossExtractor that implements the webextractors as plugins written in @c Python , @c Ruby or @c JavaScript
 *
 * If one wants to create a c++ based extractor it must be derived from this class <i>(not recommended, better write them as KROSS script)</i>
 */
class NEPOMUKMETADATAEXTRACTOR_EXPORT WebExtractor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Represents all information sabout a plugin and its capabilities
     */
    struct Info {
        QString name;           /**< Long name of the plugin */
        QString homepage;       /**< Homepage of the site this plugin parses */
        QString identifier;     /**< Short unique identifier of the plugin */
        QString description;    /**< Longer description of the plugin */
        QStringList resource;   /**< List of resources that can be used (@c tvshow , @c movie @c publication ) */
        QStringList urlregex;   /**< Regexp that fits the url scheme to detect the correct plugin based on the input website for a single item */
        QString author;         /**< Plugin author */
        QString email;          /**< Plugin author email */
        QString file;           /**< File path and name of the plugin */
        QString icon;           /**< An icon */
        bool hasConfig;         /**< true if plugin can show a gui config dialog */

        /**
         * @brief operator ==
         * @param other
         * @return @arg true if 2 Info items are the same @arg false if they are different
         */
        bool operator==(const Info& other) const {
            return  identifier == other.identifier;
        }
    };

    /**
     * @brief Standard Constructor
     *
     * @param parent some parent object
     */
    explicit WebExtractor(QObject *parent = 0);

    /**
     * @brief default destructor
     */
    ~WebExtractor();

    /**
     * @brief Creates and returns the @c info object for the plugin
     *
     * @return information about the plugin
     */
    virtual WebExtractor::Info info() = 0;

public slots:
    /**
     * @brief Starts a websearch for all possible items that fit the search parameters
     *
     * The Plugins search for a list of possible matches and emits the results via @c searchResults
     *
     * @param resourceType @c tvshow , @c movie  @c music or  @cpublication
     * @param parameters Additinal parameters defining the search
     *                   @li @c title - the title name
     *                   @li @c alttitle - alternative title name
     *                   @li @c author - author name
     *                   @li @c season - number of the tvshow season
     *                   @li @c episode - number of the tvshow episode
     *                   @li @c yearMin - minimum release year
     *                   @li @c yearMax - maximum release year
     *                   @li @c journal - name of the journal
     *                   @li @c showtitle - name of the tv show
     *
     */
    virtual void search(const QString &resourceType, const QVariantMap &parameters) = 0;

    /**
     * @brief Extracts the single item result from a specific web url
     *
     * Fetches the data from the webpage and transforms the data into @c QVariantMap emits the results via @c itemResults
     *
     * @param url the weburl where the ite mdata can be found
     * @param options additional options
     *
     * @see @c NepomukPipe
     * @see @c MoviePipe
     * @see @c TvShowPipe
     * @see @c PublicationPipe
     * @see @c MusicPipe
     */
    virtual void extractItem(const QUrl &url, const QVariantMap &options) = 0;

    /**
     * @brief Shows plugin-specific config dialog
     *
     * Should simply show the config dialog for the plugin.
     *
     */
    virtual void showConfigDialog() = 0;

signals:
    /**
     * @brief Emits some log message
     *
     * @param logMessage log message
     */
    void log(const QString &logMessage);

    /**
     * @brief Emits an error message thrown either controlled from the plugin or when we catch an exception
     *
     * @param errorMessage the error reason
     */
    void error(const QString &errorMessage);

    /**
     * @brief Emits the search result list with all items that might fit
     *
     * @param entries list of entries where each has the information:
     *                @li @c title - the name of the item
     *                @li @c url - the url where the data can be fetched
     *                @li @c details - some more detail information
     */
    void searchResults(const QVariantList &entries);

    /**
     * @brief Complete item meta data as retrieved from the url
     *
     * @param resourceType type of resoruce that was looked up
     * @param entry map with all ite mdetails
     *
     * @see @c MoviePipe
     * @see @c TvShowPipe
     * @see @c PublicationPipe
     * @see @c MusicPipe
     */
    void itemResults(const QString &resourceType, const QVariantMap &entry);
};
}
}

#endif // WEBEXTRACTOR_H
