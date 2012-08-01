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

#ifndef _LEGISLATION_H_
#define _LEGISLATION_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class Resource;
    class CodeOfLaw;

}

#include "legaldocument.h"


namespace Nepomuk2 {

    /**
     * A legal document proposing or enacting a law or a group of laws. 
     */
    class  Legislation : public LegalDocument
    {
    public:
        /**
         * Create a new empty and invalid Legislation instance
         */
        Legislation();

        /**
         * Default copy constructor
         */
        Legislation( const Legislation& );
        Legislation( const Resource& );

        /**
         * Create a new Legislation instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Legislation( const QString& uriOrIdentifier );

        /**
         * Create a new Legislation instance representing the resource
         * referenced by \a uri.
         */
        Legislation( const QUrl& uri );
        ~Legislation();

        Legislation& operator=( const Legislation& );

        /**
         * Get property 'coSponsor'. The co-sponsor [of a bill] 
         */
        QList<Resource> coSponsors() const;

        /**
         * Set property 'coSponsor'. The co-sponsor [of a bill] 
         */
        void setCoSponsors( const QList<Resource>& value );

        /**
         * Add a value to property 'coSponsor'. The co-sponsor [of a bill] 
         */
        void addCoSponsor( const Resource& value );

        /**
         * \return The URI of the property 'coSponsor'. 
         */
        static QUrl coSponsorUri();

        /**
         * Get property 'codeOfLaw'. The code of law the legislation is 
         * contained in 
         */
        QList<CodeOfLaw> codeOfLaws() const;

        /**
         * Set property 'codeOfLaw'. The code of law the legislation is 
         * contained in 
         */
        void setCodeOfLaws( const QList<CodeOfLaw>& value );

        /**
         * Add a value to property 'codeOfLaw'. The code of law the legislation 
         * is contained in 
         */
        void addCodeOfLaw( const CodeOfLaw& value );

        /**
         * Get property 'codeOfLaw'. The code of law the legislation is 
         * contained in 
         */
        CodeOfLaw codeOfLaw() const;

        /**
         * Set property 'codeOfLaw'. The code of law the legislation is 
         * contained in 
         */
        void setCodeOfLaw( const CodeOfLaw& value );

        /**
         * \return The URI of the property 'codeOfLaw'. 
         */
        static QUrl codeOfLawUri();

        /**
         * Get all resources that have this resource set as property 'legislation'. 
         * links to a legislation (Bill, Statute) in a Code of Law \sa ResourceManager::allResourcesWithProperty 
         */
        QList<CodeOfLaw> legislationOf() const;

        /**
         * Retrieve a list of all available Legislation resources. This 
         * list consists of all resource of type Legislation that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Legislation> allLegislations();


        /**
         * \return The URI of the resource type that is used in Legislation instances.
         */
        static QString resourceTypeUri();

    protected:
       Legislation( const QString& uri, const QUrl& type );
       Legislation( const QUrl& uri, const QUrl& type );
    };
}

#endif
