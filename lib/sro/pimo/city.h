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

#ifndef _CITY_H_
#define _CITY_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "location.h"


namespace Nepomuk2 {

    /**
     * A large and densely populated urban area; may include several 
     * independent administrative districts; "Ancient Troy was 
     * a great city". (Definition from SUMO) 
     */
    class  City : public Location
    {
    public:
        /**
         * Create a new empty and invalid City instance
         */
        City();

        /**
         * Default copy constructor
         */
        City( const City& );
        City( const Resource& );

        /**
         * Create a new City instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        City( const QString& uriOrIdentifier );

        /**
         * Create a new City instance representing the resource
         * referenced by \a uri.
         */
        City( const QUrl& uri );
        ~City();

        City& operator=( const City& );

        /**
         * Retrieve a list of all available City resources. This list consists 
         * of all resource of type City that are stored in the local Nepomuk 
         * meta data storage and any changes made locally. Be aware that 
         * in some cases this list can get very big. Then it might be better 
         * to use libKNep directly. 
         */
        static QList<City> allCitys();


        /**
         * \return The URI of the resource type that is used in City instances.
         */
        static QString resourceTypeUri();

    protected:
       City( const QString& uri, const QUrl& type );
       City( const QUrl& uri, const QUrl& type );
    };
}

#endif
