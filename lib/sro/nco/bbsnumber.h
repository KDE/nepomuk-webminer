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

#ifndef _BBSNUMBER_H_
#define _BBSNUMBER_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "modemnumber.h"


namespace Nepomuk2 {

    /**
     * A Bulletin Board System (BBS) phone number. Inspired by the 
     * (TYPE=bbsl) parameter of the TEL property as defined in RFC 
     * 2426 sec 3.3.1. 
     */
    class  BbsNumber : public ModemNumber
    {
    public:
        /**
         * Create a new empty and invalid BbsNumber instance
         */
        BbsNumber();

        /**
         * Default copy constructor
         */
        BbsNumber( const BbsNumber& );
        BbsNumber( const Resource& );

        /**
         * Create a new BbsNumber instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        BbsNumber( const QString& uriOrIdentifier );

        /**
         * Create a new BbsNumber instance representing the resource
         * referenced by \a uri.
         */
        BbsNumber( const QUrl& uri );
        ~BbsNumber();

        BbsNumber& operator=( const BbsNumber& );

        /**
         * Retrieve a list of all available BbsNumber resources. This 
         * list consists of all resource of type BbsNumber that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<BbsNumber> allBbsNumbers();


        /**
         * \return The URI of the resource type that is used in BbsNumber instances.
         */
        static QString resourceTypeUri();

    protected:
       BbsNumber( const QString& uri, const QUrl& type );
       BbsNumber( const QUrl& uri, const QUrl& type );
    };
}

#endif
