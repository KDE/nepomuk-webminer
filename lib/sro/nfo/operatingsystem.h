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

#ifndef _OPERATINGSYSTEM_H_
#define _OPERATINGSYSTEM_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "software.h"


namespace Nepomuk2 {

    /**
     * An OperatingSystem 
     */
    class  OperatingSystem : public Software
    {
    public:
        /**
         * Create a new empty and invalid OperatingSystem instance
         */
        OperatingSystem();

        /**
         * Default copy constructor
         */
        OperatingSystem( const OperatingSystem& );
        OperatingSystem( const Resource& );

        /**
         * Create a new OperatingSystem instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        OperatingSystem( const QString& uriOrIdentifier );

        /**
         * Create a new OperatingSystem instance representing the resource
         * referenced by \a uri.
         */
        OperatingSystem( const QUrl& uri );
        ~OperatingSystem();

        OperatingSystem& operator=( const OperatingSystem& );

        /**
         * Retrieve a list of all available OperatingSystem resources. 
         * This list consists of all resource of type OperatingSystem 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<OperatingSystem> allOperatingSystems();


        /**
         * \return The URI of the resource type that is used in OperatingSystem instances.
         */
        static QString resourceTypeUri();

    protected:
       OperatingSystem( const QString& uri, const QUrl& type );
       OperatingSystem( const QUrl& uri, const QUrl& type );
    };
}

#endif
