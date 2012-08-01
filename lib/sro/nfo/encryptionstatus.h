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

#ifndef _ENCRYPTIONSTATUS_H_
#define _ENCRYPTIONSTATUS_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class InformationElement;

}

#include "resource.h"


namespace Nepomuk2 {

    /**
     * The status of the encryption of an InformationElement. nfo:encryptedStatus 
     * means that the InformationElement has been encrypted and couldn't 
     * be decrypted by the extraction software, thus no content is 
     * available. nfo:decryptedStatus means that decryption was 
     * successfull and the content is available. 
     */
    class  EncryptionStatus : public Resource
    {
    public:
        /**
         * Create a new empty and invalid EncryptionStatus instance
         */
        EncryptionStatus();

        /**
         * Default copy constructor
         */
        EncryptionStatus( const EncryptionStatus& );
        EncryptionStatus( const Resource& );

        /**
         * Create a new EncryptionStatus instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        EncryptionStatus( const QString& uriOrIdentifier );

        /**
         * Create a new EncryptionStatus instance representing the resource
         * referenced by \a uri.
         */
        EncryptionStatus( const QUrl& uri );
        ~EncryptionStatus();

        EncryptionStatus& operator=( const EncryptionStatus& );

        /**
         * Get all resources that have this resource set as property 'encryptionStatus'. 
         * The status of the encryption of the InformationElement. \sa 
         * ResourceManager::allResourcesWithProperty 
         */
        QList<InformationElement> encryptionStatusOf() const;

        /**
         * Retrieve a list of all available EncryptionStatus resources. 
         * This list consists of all resource of type EncryptionStatus 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<EncryptionStatus> allEncryptionStatuss();


        /**
         * \return The URI of the resource type that is used in EncryptionStatus instances.
         */
        static QString resourceTypeUri();

    protected:
       EncryptionStatus( const QString& uri, const QUrl& type );
       EncryptionStatus( const QUrl& uri, const QUrl& type );
    };
}

#endif
