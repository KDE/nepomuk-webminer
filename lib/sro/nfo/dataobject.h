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

#ifndef _DATAOBJECT_H_
#define _DATAOBJECT_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class Media;
    class Bookmark;
    class DataContainer;

}

#include "resource.h"


namespace Nepomuk2 {


    class  DataObject : public Resource
    {
    public:
        /**
         * Create a new empty and invalid DataObject instance
         */
        DataObject();

        /**
         * Default copy constructor
         */
        DataObject( const DataObject& );
        DataObject( const Resource& );

        /**
         * Create a new DataObject instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        DataObject( const QString& uriOrIdentifier );

        /**
         * Create a new DataObject instance representing the resource
         * referenced by \a uri.
         */
        DataObject( const QUrl& uri );
        ~DataObject();

        DataObject& operator=( const DataObject& );

        /**
         * Get property 'belongsToContainer'. Models the containment 
         * relations between Files and Folders (or CompressedFiles). 
         */
        QList<DataContainer> belongsToContainers() const;

        /**
         * Set property 'belongsToContainer'. Models the containment 
         * relations between Files and Folders (or CompressedFiles). 
         */
        void setBelongsToContainers( const QList<DataContainer>& value );

        /**
         * Add a value to property 'belongsToContainer'. Models the containment 
         * relations between Files and Folders (or CompressedFiles). 
         */
        void addBelongsToContainer( const DataContainer& value );

        /**
         * \return The URI of the property 'belongsToContainer'. 
         */
        static QUrl belongsToContainerUri();

        /**
         * Get all resources that have this resource set as property 'bookmarks'. 
         * The address of the linked object. Usually a web URI. \sa ResourceManager::allResourcesWithProperty 
         */
        QList<Bookmark> bookmarksOf() const;

        /**
         * Get all resources that have this resource set as property 'MediaStream'. 
         * Connects a media container with a single media stream contained 
         * within. \sa ResourceManager::allResourcesWithProperty 
         */
        QList<Media> mediaStreamOf() const;

        /**
         * Retrieve a list of all available DataObject resources. This 
         * list consists of all resource of type DataObject that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<DataObject> allDataObjects();


        /**
         * \return The URI of the resource type that is used in DataObject instances.
         */
        static QString resourceTypeUri();

    protected:
       DataObject( const QString& uri, const QUrl& type );
       DataObject( const QUrl& uri, const QUrl& type );
    };
}

#endif
