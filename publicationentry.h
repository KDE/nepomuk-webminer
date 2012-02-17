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

#ifndef PUBLICATIONENTRY_H
#define PUBLICATIONENTRY_H

#include <KDE/KUrl>

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtXml/QDomDocument>

/**
  * @brief represents one publication entry and all its properties
  */
struct PublicationEntry {
    QString type;                           /**< The type (@article, @book etc.) */
    QString citeKey;                        /**< The commonly used citykey for the reference */
    KUrl fileUrl;                           /**< The url of the local file or nepomuk resource uri */
    QDomDocument chapter;                   /**< Hierachical chapter entries as retrieved from the file parser */
    QMap<QString, QString> dataMap;         /**< bibtex like key/value pairs */
    QList<PublicationEntry*> citations;     /**< All entries cited by the publication */
};

struct MetaDataParameters {
    QString searchTitle;     /**< title of the entry to search for */
    QString searchAltTitle;  /**< alternative title of the entry to search for in case the first one does not return anything */
    QString searchPerson;    /**< person contact to search for (author, director, artist) */
    QString searchYear;      /**< year t osearch for (publication date etc) */
    KUrl resourceType;       /**< used to select the right set of python modules that allow retrieving data for this resoruce */
    KUrl resourceUri;        /**< the nepomuk or file url where the retrieved data will be added to */
    QVariantMap metaData;    /**< here all fetched data is pushed into @todo define proper api how the data should be saved in her (key=value list) */
};

#endif // PUBLICATIONENTRY_H
