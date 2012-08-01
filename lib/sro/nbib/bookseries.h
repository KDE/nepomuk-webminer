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

#ifndef _BOOKSERIES_H_
#define _BOOKSERIES_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "series.h"


namespace Nepomuk2 {

    /**
     * The series of a book. 
     */
    class  BookSeries : public Series
    {
    public:
        /**
         * Create a new empty and invalid BookSeries instance
         */
        BookSeries();

        /**
         * Default copy constructor
         */
        BookSeries( const BookSeries& );
        BookSeries( const Resource& );

        /**
         * Create a new BookSeries instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        BookSeries( const QString& uriOrIdentifier );

        /**
         * Create a new BookSeries instance representing the resource
         * referenced by \a uri.
         */
        BookSeries( const QUrl& uri );
        ~BookSeries();

        BookSeries& operator=( const BookSeries& );

        /**
         * Retrieve a list of all available BookSeries resources. This 
         * list consists of all resource of type BookSeries that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<BookSeries> allBookSeriess();


        /**
         * \return The URI of the resource type that is used in BookSeries instances.
         */
        static QString resourceTypeUri();

    protected:
       BookSeries( const QString& uri, const QUrl& type );
       BookSeries( const QUrl& uri, const QUrl& type );
    };
}

#endif
