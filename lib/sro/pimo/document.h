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

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {

}

#include "logicalmediatype.h"


namespace Nepomuk2 {

    /**
     * A generic document. This is a placeholder class for document-management 
     * domain ontologies to subclass. Create more and specified subclasses 
     * of pimo:Document for the document types in your domain. Documents 
     * are typically instances of both NFO:Document (modeling the 
     * information element used to store the document) and a LogicalMediaType 
     * subclass. Two examples are given for what to model here: a contract 
     * for a business domain, a BlogPost for an informal domain. 
     */
    class  Document : public LogicalMediaType
    {
    public:
        /**
         * Create a new empty and invalid Document instance
         */
        Document();

        /**
         * Default copy constructor
         */
        Document( const Document& );
        Document( const Resource& );

        /**
         * Create a new Document instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Document( const QString& uriOrIdentifier );

        /**
         * Create a new Document instance representing the resource
         * referenced by \a uri.
         */
        Document( const QUrl& uri );
        ~Document();

        Document& operator=( const Document& );

        /**
         * Retrieve a list of all available Document resources. This list 
         * consists of all resource of type Document that are stored in 
         * the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Document> allDocuments();


        /**
         * \return The URI of the resource type that is used in Document instances.
         */
        static QString resourceTypeUri();

    protected:
       Document( const QString& uri, const QUrl& type );
       Document( const QUrl& uri, const QUrl& type );
    };
}

#endif
