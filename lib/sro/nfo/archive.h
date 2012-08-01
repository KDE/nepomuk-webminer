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

#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "datacontainer.h"


namespace Nepomuk2 {

    /**
     * A compressed file. May contain other files or folder inside. 
     */
    class  Archive : public DataContainer
    {
    public:
        /**
         * Create a new empty and invalid Archive instance
         */
        Archive();

        /**
         * Default copy constructor
         */
        Archive( const Archive& );
        Archive( const Resource& );

        /**
         * Create a new Archive instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Archive( const QString& uriOrIdentifier );

        /**
         * Create a new Archive instance representing the resource
         * referenced by \a uri.
         */
        Archive( const QUrl& uri );
        ~Archive();

        Archive& operator=( const Archive& );

        /**
         * Get property 'uncompressedSize'. Uncompressed size of the 
         * content of a compressed file. 
         */
        QList<qint64> uncompressedSizes() const;

        /**
         * Set property 'uncompressedSize'. Uncompressed size of the 
         * content of a compressed file. 
         */
        void setUncompressedSizes( const QList<qint64>& value );

        /**
         * Add a value to property 'uncompressedSize'. Uncompressed 
         * size of the content of a compressed file. 
         */
        void addUncompressedSize( const qint64& value );

        /**
         * Get property 'uncompressedSize'. Uncompressed size of the 
         * content of a compressed file. 
         */
        qint64 uncompressedSize() const;

        /**
         * Set property 'uncompressedSize'. Uncompressed size of the 
         * content of a compressed file. 
         */
        void setUncompressedSize( const qint64& value );

        /**
         * \return The URI of the property 'uncompressedSize'. 
         */
        static QUrl uncompressedSizeUri();

        /**
         * Retrieve a list of all available Archive resources. This list 
         * consists of all resource of type Archive that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Archive> allArchives();


        /**
         * \return The URI of the resource type that is used in Archive instances.
         */
        static QString resourceTypeUri();

    protected:
       Archive( const QString& uri, const QUrl& type );
       Archive( const QUrl& uri, const QUrl& type );
    };
}

#endif
