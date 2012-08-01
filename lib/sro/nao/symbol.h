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

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class Resource;

}

#include "resource.h"


namespace Nepomuk2 {

    /**
     * Represents a symbol, a visual representation of a resource. 
     * Typically a local or remote file would be double-typed to be 
     * used as a symbol. An alternative is nao:FreeDesktopIcon. 
     */
    class  Symbol : public Resource
    {
    public:
        /**
         * Create a new empty and invalid Symbol instance
         */
        Symbol();

        /**
         * Default copy constructor
         */
        Symbol( const Symbol& );
        Symbol( const Resource& );

        /**
         * Create a new Symbol instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Symbol( const QString& uriOrIdentifier );

        /**
         * Create a new Symbol instance representing the resource
         * referenced by \a uri.
         */
        Symbol( const QUrl& uri );
        ~Symbol();

        Symbol& operator=( const Symbol& );

        /**
         * Get all resources that have this resource set as property 'altSymbol'. 
         * An alternative symbol representation for a resource \sa ResourceManager::allResourcesWithProperty 
         */
        QList<Resource> altSymbolOf() const;

        /**
         * Get all resources that have this resource set as property 'Symbol'. 
         * Annotation for a resource in the form of a visual representation. 
         * Typically the symbol is a double-typed image file or a nao:FreeDesktopIcon. 
         * \sa ResourceManager::allResourcesWithProperty 
         */
        QList<Resource> symbolOf() const;

        /**
         * Get all resources that have this resource set as property 'prefSymbol'. 
         * A unique preferred symbol representation for a resource \sa 
         * ResourceManager::allResourcesWithProperty 
         */
        QList<Resource> prefSymbolOf() const;

        /**
         * Retrieve a list of all available Symbol resources. This list 
         * consists of all resource of type Symbol that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Symbol> allSymbols();


        /**
         * \return The URI of the resource type that is used in Symbol instances.
         */
        static QString resourceTypeUri();

    protected:
       Symbol( const QString& uri, const QUrl& type );
       Symbol( const QUrl& uri, const QUrl& type );
    };
}

#endif
