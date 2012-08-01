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

#ifndef _ELECTRONIC_H_
#define _ELECTRONIC_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "publication.h"


namespace Nepomuk2 {

    /**
     * An electronic publication (for example website or presentations) 
     */
    class  Electronic : public Publication
    {
    public:
        /**
         * Create a new empty and invalid Electronic instance
         */
        Electronic();

        /**
         * Default copy constructor
         */
        Electronic( const Electronic& );
        Electronic( const Resource& );

        /**
         * Create a new Electronic instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Electronic( const QString& uriOrIdentifier );

        /**
         * Create a new Electronic instance representing the resource
         * referenced by \a uri.
         */
        Electronic( const QUrl& uri );
        ~Electronic();

        Electronic& operator=( const Electronic& );

        /**
         * Retrieve a list of all available Electronic resources. This 
         * list consists of all resource of type Electronic that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Electronic> allElectronics();


        /**
         * \return The URI of the resource type that is used in Electronic instances.
         */
        static QString resourceTypeUri();

    protected:
       Electronic( const QString& uri, const QUrl& type );
       Electronic( const QUrl& uri, const QUrl& type );
    };
}

#endif
