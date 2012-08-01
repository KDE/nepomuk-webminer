/*
 * This file is part of the Nepomuk KDE project.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file "COPYING" for the exact licensing terms.
 */

/*
 * This file has been generated by the Nepomuk Resource class generator.
 * DO NOT EDIT THIS FILE.
 * ANY CHANGES WILL BE LOST.
 */

#ifndef _BOOKMARK_H_
#define _BOOKMARK_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class BookmarkFolder;
    class DataObject;

}

#include "informationelement.h"


namespace Nepomuk2 {

    /**
     * A bookmark of a webbrowser. Use nie:title for the name/label, 
     * nie:contentCreated to represent the date when the user added 
     * the bookmark, and nie:contentLastModified for modifications. 
     * nfo:bookmarks to store the link. 
     */
    class  Bookmark : public InformationElement
    {
    public:
        /**
         * Create a new empty and invalid Bookmark instance
         */
        Bookmark();

        /**
         * Default copy constructor
         */
        Bookmark( const Bookmark& );
        Bookmark( const Resource& );

        /**
         * Create a new Bookmark instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Bookmark( const QString& uriOrIdentifier );

        /**
         * Create a new Bookmark instance representing the resource
         * referenced by \a uri.
         */
        Bookmark( const QUrl& uri );
        ~Bookmark();

        Bookmark& operator=( const Bookmark& );

        /**
         * Get property 'bookmarks'. The address of the linked object. 
         * Usually a web URI. 
         */
        QList<DataObject> bookmarkses() const;

        /**
         * Set property 'bookmarks'. The address of the linked object. 
         * Usually a web URI. 
         */
        void setBookmarkses( const QList<DataObject>& value );

        /**
         * Add a value to property 'bookmarks'. The address of the linked 
         * object. Usually a web URI. 
         */
        void addBookmarks( const DataObject& value );

        /**
         * Get property 'bookmarks'. The address of the linked object. 
         * Usually a web URI. 
         */
        DataObject bookmarks() const;

        /**
         * Set property 'bookmarks'. The address of the linked object. 
         * Usually a web URI. 
         */
        void setBookmarks( const DataObject& value );

        /**
         * \return The URI of the property 'bookmarks'. 
         */
        static QUrl bookmarksUri();

        /**
         * Get property 'characterPosition'. Character position of 
         * the bookmark. 
         */
        QList<qint64> characterPositions() const;

        /**
         * Set property 'characterPosition'. Character position of 
         * the bookmark. 
         */
        void setCharacterPositions( const QList<qint64>& value );

        /**
         * Add a value to property 'characterPosition'. Character position 
         * of the bookmark. 
         */
        void addCharacterPosition( const qint64& value );

        /**
         * Get property 'characterPosition'. Character position of 
         * the bookmark. 
         */
        qint64 characterPosition() const;

        /**
         * Set property 'characterPosition'. Character position of 
         * the bookmark. 
         */
        void setCharacterPosition( const qint64& value );

        /**
         * \return The URI of the property 'characterPosition'. 
         */
        static QUrl characterPositionUri();

        /**
         * Get property 'pageNumber'. Page linked by the bookmark. 
         */
        QList<qint64> pageNumbers() const;

        /**
         * Set property 'pageNumber'. Page linked by the bookmark. 
         */
        void setPageNumbers( const QList<qint64>& value );

        /**
         * Add a value to property 'pageNumber'. Page linked by the bookmark. 
         */
        void addPageNumber( const qint64& value );

        /**
         * Get property 'pageNumber'. Page linked by the bookmark. 
         */
        qint64 pageNumber() const;

        /**
         * Set property 'pageNumber'. Page linked by the bookmark. 
         */
        void setPageNumber( const qint64& value );

        /**
         * \return The URI of the property 'pageNumber'. 
         */
        static QUrl pageNumberUri();

        /**
         * Get property 'streamPosition'. Stream position of the bookmark, 
         * suitable for e.g. audio books. Expressed in milliseconds 
         */
        QList<qint64> streamPositions() const;

        /**
         * Set property 'streamPosition'. Stream position of the bookmark, 
         * suitable for e.g. audio books. Expressed in milliseconds 
         */
        void setStreamPositions( const QList<qint64>& value );

        /**
         * Add a value to property 'streamPosition'. Stream position 
         * of the bookmark, suitable for e.g. audio books. Expressed in 
         * milliseconds 
         */
        void addStreamPosition( const qint64& value );

        /**
         * Get property 'streamPosition'. Stream position of the bookmark, 
         * suitable for e.g. audio books. Expressed in milliseconds 
         */
        qint64 streamPosition() const;

        /**
         * Set property 'streamPosition'. Stream position of the bookmark, 
         * suitable for e.g. audio books. Expressed in milliseconds 
         */
        void setStreamPosition( const qint64& value );

        /**
         * \return The URI of the property 'streamPosition'. 
         */
        static QUrl streamPositionUri();

        /**
         * Get all resources that have this resource set as property 'containsBookmark'. 
         * The folder contains a bookmark. \sa ResourceManager::allResourcesWithProperty 
         */
        QList<BookmarkFolder> containsBookmarkOf() const;

        /**
         * Retrieve a list of all available Bookmark resources. This list 
         * consists of all resource of type Bookmark that are stored in 
         * the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Bookmark> allBookmarks();


        /**
         * \return The URI of the resource type that is used in Bookmark instances.
         */
        static QString resourceTypeUri();

    protected:
       Bookmark( const QString& uri, const QUrl& type );
       Bookmark( const QUrl& uri, const QUrl& type );
    };
}

#endif
