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

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "book.h"


namespace Nepomuk2 {

    /**
     * A dictionary 
     */
    class  Dictionary : public Book
    {
    public:
        /**
         * Create a new empty and invalid Dictionary instance
         */
        Dictionary();

        /**
         * Default copy constructor
         */
        Dictionary( const Dictionary& );
        Dictionary( const Resource& );

        /**
         * Create a new Dictionary instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Dictionary( const QString& uriOrIdentifier );

        /**
         * Create a new Dictionary instance representing the resource
         * referenced by \a uri.
         */
        Dictionary( const QUrl& uri );
        ~Dictionary();

        Dictionary& operator=( const Dictionary& );

        /**
         * Retrieve a list of all available Dictionary resources. This 
         * list consists of all resource of type Dictionary that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Dictionary> allDictionarys();


        /**
         * \return The URI of the resource type that is used in Dictionary instances.
         */
        static QString resourceTypeUri();

    protected:
       Dictionary( const QString& uri, const QUrl& type );
       Dictionary( const QUrl& uri, const QUrl& type );
    };
}

#endif
