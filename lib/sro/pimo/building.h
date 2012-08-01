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

#ifndef _BUILDING_H_
#define _BUILDING_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "location.h"


namespace Nepomuk2 {

    /**
     * A structure that has a roof and walls and stands more or less permanently 
     * in one place; "there was a three-story building on the corner"; 
     * "it was an imposing edifice". (Definition from SUMO). 
     */
    class  Building : public Location
    {
    public:
        /**
         * Create a new empty and invalid Building instance
         */
        Building();

        /**
         * Default copy constructor
         */
        Building( const Building& );
        Building( const Resource& );

        /**
         * Create a new Building instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Building( const QString& uriOrIdentifier );

        /**
         * Create a new Building instance representing the resource
         * referenced by \a uri.
         */
        Building( const QUrl& uri );
        ~Building();

        Building& operator=( const Building& );

        /**
         * Retrieve a list of all available Building resources. This list 
         * consists of all resource of type Building that are stored in 
         * the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Building> allBuildings();


        /**
         * \return The URI of the resource type that is used in Building instances.
         */
        static QString resourceTypeUri();

    protected:
       Building( const QString& uri, const QUrl& type );
       Building( const QUrl& uri, const QUrl& type );
    };
}

#endif
