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

#ifndef _DELETEDRESOURCE_H_
#define _DELETEDRESOURCE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "filedataobject.h"


namespace Nepomuk2 {

    /**
     * A file entity that has been deleted from the original source. 
     * Usually such entities are stored within various kinds of 'Trash' 
     * or 'Recycle Bin' folders. 
     */
    class  DeletedResource : public FileDataObject
    {
    public:
        /**
         * Create a new empty and invalid DeletedResource instance
         */
        DeletedResource();

        /**
         * Default copy constructor
         */
        DeletedResource( const DeletedResource& );
        DeletedResource( const Resource& );

        /**
         * Create a new DeletedResource instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        DeletedResource( const QString& uriOrIdentifier );

        /**
         * Create a new DeletedResource instance representing the resource
         * referenced by \a uri.
         */
        DeletedResource( const QUrl& uri );
        ~DeletedResource();

        DeletedResource& operator=( const DeletedResource& );

        /**
         * Get property 'deletionDate'. The date and time of the deletion. 
         */
        QList<QDateTime> deletionDates() const;

        /**
         * Set property 'deletionDate'. The date and time of the deletion. 
         */
        void setDeletionDates( const QList<QDateTime>& value );

        /**
         * Add a value to property 'deletionDate'. The date and time of 
         * the deletion. 
         */
        void addDeletionDate( const QDateTime& value );

        /**
         * Get property 'deletionDate'. The date and time of the deletion. 
         */
        QDateTime deletionDate() const;

        /**
         * Set property 'deletionDate'. The date and time of the deletion. 
         */
        void setDeletionDate( const QDateTime& value );

        /**
         * \return The URI of the property 'deletionDate'. 
         */
        static QUrl deletionDateUri();

        /**
         * Get property 'originalLocation'. The original location of 
         * the deleted resource. 
         */
        QStringList originalLocations() const;

        /**
         * Set property 'originalLocation'. The original location of 
         * the deleted resource. 
         */
        void setOriginalLocations( const QStringList& value );

        /**
         * Add a value to property 'originalLocation'. The original location 
         * of the deleted resource. 
         */
        void addOriginalLocation( const QString& value );

        /**
         * Get property 'originalLocation'. The original location of 
         * the deleted resource. 
         */
        QString originalLocation() const;

        /**
         * Set property 'originalLocation'. The original location of 
         * the deleted resource. 
         */
        void setOriginalLocation( const QString& value );

        /**
         * \return The URI of the property 'originalLocation'. 
         */
        static QUrl originalLocationUri();

        /**
         * Retrieve a list of all available DeletedResource resources. 
         * This list consists of all resource of type DeletedResource 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<DeletedResource> allDeletedResources();


        /**
         * \return The URI of the resource type that is used in DeletedResource instances.
         */
        static QString resourceTypeUri();

    protected:
       DeletedResource( const QString& uri, const QUrl& type );
       DeletedResource( const QUrl& uri, const QUrl& type );
    };
}

#endif
