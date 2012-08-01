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

#ifndef _EVENT_H_
#define _EVENT_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "processconcept.h"


namespace Nepomuk2 {

    /**
     * Something that happens An Event is conceived as compact in time. 
     * (Definition from Merriam-Webster) 
     */
    class  Event : public ProcessConcept
    {
    public:
        /**
         * Create a new empty and invalid Event instance
         */
        Event();

        /**
         * Default copy constructor
         */
        Event( const Event& );
        Event( const Resource& );

        /**
         * Create a new Event instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Event( const QString& uriOrIdentifier );

        /**
         * Create a new Event instance representing the resource
         * referenced by \a uri.
         */
        Event( const QUrl& uri );
        ~Event();

        Event& operator=( const Event& );

        /**
         * Retrieve a list of all available Event resources. This list 
         * consists of all resource of type Event that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Event> allEvents();


        /**
         * \return The URI of the resource type that is used in Event instances.
         */
        static QString resourceTypeUri();

    protected:
       Event( const QString& uri, const QUrl& type );
       Event( const QUrl& uri, const QUrl& type );
    };
}

#endif
