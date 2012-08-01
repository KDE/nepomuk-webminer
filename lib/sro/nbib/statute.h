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

#ifndef _STATUTE_H_
#define _STATUTE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "legislation.h"


namespace Nepomuk2 {

    /**
     * A bill enacted into law. 
     */
    class  Statute : public Legislation
    {
    public:
        /**
         * Create a new empty and invalid Statute instance
         */
        Statute();

        /**
         * Default copy constructor
         */
        Statute( const Statute& );
        Statute( const Resource& );

        /**
         * Create a new Statute instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Statute( const QString& uriOrIdentifier );

        /**
         * Create a new Statute instance representing the resource
         * referenced by \a uri.
         */
        Statute( const QUrl& uri );
        ~Statute();

        Statute& operator=( const Statute& );

        /**
         * Get property 'publicLawNumber'. The public law number of the 
         * statute 
         */
        QStringList publicLawNumbers() const;

        /**
         * Set property 'publicLawNumber'. The public law number of the 
         * statute 
         */
        void setPublicLawNumbers( const QStringList& value );

        /**
         * Add a value to property 'publicLawNumber'. The public law number 
         * of the statute 
         */
        void addPublicLawNumber( const QString& value );

        /**
         * \return The URI of the property 'publicLawNumber'. 
         */
        static QUrl publicLawNumberUri();

        /**
         * Retrieve a list of all available Statute resources. This list 
         * consists of all resource of type Statute that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Statute> allStatutes();


        /**
         * \return The URI of the resource type that is used in Statute instances.
         */
        static QString resourceTypeUri();

    protected:
       Statute( const QString& uri, const QUrl& type );
       Statute( const QUrl& uri, const QUrl& type );
    };
}

#endif
