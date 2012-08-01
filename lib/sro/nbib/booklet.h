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

#ifndef _BOOKLET_H_
#define _BOOKLET_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "publication.h"


namespace Nepomuk2 {

    /**
     * A work that is printed and bound, but without a named publisher 
     * or sponsoring institution. 
     */
    class  Booklet : public Publication
    {
    public:
        /**
         * Create a new empty and invalid Booklet instance
         */
        Booklet();

        /**
         * Default copy constructor
         */
        Booklet( const Booklet& );
        Booklet( const Resource& );

        /**
         * Create a new Booklet instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Booklet( const QString& uriOrIdentifier );

        /**
         * Create a new Booklet instance representing the resource
         * referenced by \a uri.
         */
        Booklet( const QUrl& uri );
        ~Booklet();

        Booklet& operator=( const Booklet& );

        /**
         * Retrieve a list of all available Booklet resources. This list 
         * consists of all resource of type Booklet that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Booklet> allBooklets();


        /**
         * \return The URI of the resource type that is used in Booklet instances.
         */
        static QString resourceTypeUri();

    protected:
       Booklet( const QString& uri, const QUrl& type );
       Booklet( const QUrl& uri, const QUrl& type );
    };
}

#endif
