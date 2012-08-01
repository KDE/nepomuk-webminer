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

#ifndef _STATE_H_
#define _STATE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "location.h"


namespace Nepomuk2 {

    /**
     * Administrative subdivisions of a Nation that are broader than 
     * any other political subdivisions that may exist. This Class 
     * includes the states of the United States, as well as the provinces 
     * of Canada and European countries. (Definition from SUMO). 
     */
    class  State : public Location
    {
    public:
        /**
         * Create a new empty and invalid State instance
         */
        State();

        /**
         * Default copy constructor
         */
        State( const State& );
        State( const Resource& );

        /**
         * Create a new State instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        State( const QString& uriOrIdentifier );

        /**
         * Create a new State instance representing the resource
         * referenced by \a uri.
         */
        State( const QUrl& uri );
        ~State();

        State& operator=( const State& );

        /**
         * Retrieve a list of all available State resources. This list 
         * consists of all resource of type State that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<State> allStates();


        /**
         * \return The URI of the resource type that is used in State instances.
         */
        static QString resourceTypeUri();

    protected:
       State( const QString& uri, const QUrl& type );
       State( const QUrl& uri, const QUrl& type );
    };
}

#endif
