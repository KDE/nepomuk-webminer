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

#ifndef _PARCELDELIVERYADDRESS_H_
#define _PARCELDELIVERYADDRESS_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "postaladdress.h"


namespace Nepomuk2 {

    /**
     * Parcel Delivery Addresse. Class inspired by TYPE=parcel parameter 
     * of the ADR property defined in RFC 2426 sec. 3.2.1 
     */
    class  ParcelDeliveryAddress : public PostalAddress
    {
    public:
        /**
         * Create a new empty and invalid ParcelDeliveryAddress instance
         */
        ParcelDeliveryAddress();

        /**
         * Default copy constructor
         */
        ParcelDeliveryAddress( const ParcelDeliveryAddress& );
        ParcelDeliveryAddress( const Resource& );

        /**
         * Create a new ParcelDeliveryAddress instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        ParcelDeliveryAddress( const QString& uriOrIdentifier );

        /**
         * Create a new ParcelDeliveryAddress instance representing the resource
         * referenced by \a uri.
         */
        ParcelDeliveryAddress( const QUrl& uri );
        ~ParcelDeliveryAddress();

        ParcelDeliveryAddress& operator=( const ParcelDeliveryAddress& );

        /**
         * Retrieve a list of all available ParcelDeliveryAddress resources. 
         * This list consists of all resource of type ParcelDeliveryAddress 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<ParcelDeliveryAddress> allParcelDeliveryAddresss();


        /**
         * \return The URI of the resource type that is used in ParcelDeliveryAddress instances.
         */
        static QString resourceTypeUri();

    protected:
       ParcelDeliveryAddress( const QString& uri, const QUrl& type );
       ParcelDeliveryAddress( const QUrl& uri, const QUrl& type );
    };
}

#endif
