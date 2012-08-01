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

#ifndef _PHDTHESIS_H_
#define _PHDTHESIS_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "thesis.h"


namespace Nepomuk2 {

    /**
     * A PhD thesis. 
     */
    class  PhdThesis : public Thesis
    {
    public:
        /**
         * Create a new empty and invalid PhdThesis instance
         */
        PhdThesis();

        /**
         * Default copy constructor
         */
        PhdThesis( const PhdThesis& );
        PhdThesis( const Resource& );

        /**
         * Create a new PhdThesis instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        PhdThesis( const QString& uriOrIdentifier );

        /**
         * Create a new PhdThesis instance representing the resource
         * referenced by \a uri.
         */
        PhdThesis( const QUrl& uri );
        ~PhdThesis();

        PhdThesis& operator=( const PhdThesis& );

        /**
         * Retrieve a list of all available PhdThesis resources. This 
         * list consists of all resource of type PhdThesis that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<PhdThesis> allPhdThesiss();


        /**
         * \return The URI of the resource type that is used in PhdThesis instances.
         */
        static QString resourceTypeUri();

    protected:
       PhdThesis( const QString& uri, const QUrl& type );
       PhdThesis( const QUrl& uri, const QUrl& type );
    };
}

#endif
