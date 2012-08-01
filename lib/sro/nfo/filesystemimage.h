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

#ifndef _FILESYSTEMIMAGE_H_
#define _FILESYSTEMIMAGE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "filesystem.h"


namespace Nepomuk2 {

    /**
     * An image of a filesystem. Instances of this class may include 
     * CD images, DVD images or hard disk partition images created 
     * by various pieces of software (e.g. Norton Ghost). Deprecated 
     * in favor of nfo:Filesystem. 
     */
    class  FilesystemImage : public Filesystem
    {
    public:
        /**
         * Create a new empty and invalid FilesystemImage instance
         */
        FilesystemImage();

        /**
         * Default copy constructor
         */
        FilesystemImage( const FilesystemImage& );
        FilesystemImage( const Resource& );

        /**
         * Create a new FilesystemImage instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        FilesystemImage( const QString& uriOrIdentifier );

        /**
         * Create a new FilesystemImage instance representing the resource
         * referenced by \a uri.
         */
        FilesystemImage( const QUrl& uri );
        ~FilesystemImage();

        FilesystemImage& operator=( const FilesystemImage& );

        /**
         * Retrieve a list of all available FilesystemImage resources. 
         * This list consists of all resource of type FilesystemImage 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<FilesystemImage> allFilesystemImages();


        /**
         * \return The URI of the resource type that is used in FilesystemImage instances.
         */
        static QString resourceTypeUri();

    protected:
       FilesystemImage( const QString& uri, const QUrl& type );
       FilesystemImage( const QUrl& uri, const QUrl& type );
    };
}

#endif
