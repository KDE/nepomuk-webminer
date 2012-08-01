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

#ifndef _IMSTATUSTYPE_H_
#define _IMSTATUSTYPE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class IMAccount;

}

#include <Nepomuk2/Resource>


namespace Nepomuk2 {

    /**
     * The status type of an IMAccount. Based on the Connection_Presence_Type 
     * enumeration of the Telepathy project: http://telepathy.freedesktop.org/spec/Connection_Interface_Simple_Presence.html#Enum:Connection_Presence_Type 
     */
    class  IMStatusType : public Resource
    {
    public:
        /**
         * Create a new empty and invalid IMStatusType instance
         */
        IMStatusType();

        /**
         * Default copy constructor
         */
        IMStatusType( const IMStatusType& );
        IMStatusType( const Resource& );

        /**
         * Create a new IMStatusType instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        IMStatusType( const QString& uriOrIdentifier );

        /**
         * Create a new IMStatusType instance representing the resource
         * referenced by \a uri.
         */
        IMStatusType( const QUrl& uri );
        ~IMStatusType();

        IMStatusType& operator=( const IMStatusType& );

        /**
         * Get all resources that have this resource set as property 'imStatusType'. 
         * Current status type of the given IM account. When this property 
         * is set, the nco:imStatus property should also always be set. 
         * Applications should attempt to parse the nco:imStatus property 
         * to determine the presence, only falling back to this property 
         * in the case that the nco:imStatus property's value is unrecognised. 
         * \sa ResourceManager::allResourcesWithProperty 
         */
        QList<IMAccount> imStatusTypeOf() const;

        /**
         * Retrieve a list of all available IMStatusType resources. This 
         * list consists of all resource of type IMStatusType that are 
         * stored in the local Nepomuk meta data storage and any changes 
         * made locally. Be aware that in some cases this list can get very 
         * big. Then it might be better to use libKNep directly. 
         */
        static QList<IMStatusType> allIMStatusTypes();


        /**
         * \return The URI of the resource type that is used in IMStatusType instances.
         */
        static QString resourceTypeUri();

    protected:
       IMStatusType( const QString& uri, const QUrl& type );
       IMStatusType( const QUrl& uri, const QUrl& type );
    };
}

#endif
