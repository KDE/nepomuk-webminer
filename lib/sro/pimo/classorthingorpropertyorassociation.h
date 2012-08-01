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

#ifndef _CLASSORTHINGORPROPERTYORASSOCIATION_H_
#define _CLASSORTHINGORPROPERTYORASSOCIATION_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class PersonalInformationModel;

}

#include <Nepomuk2/Resource>


namespace Nepomuk2 {

    /**
     * Superclass of resources that can be generated by the user. 
     */
    class  ClassOrThingOrPropertyOrAssociation : public Resource
    {
    public:
        /**
         * Create a new empty and invalid ClassOrThingOrPropertyOrAssociation instance
         */
        ClassOrThingOrPropertyOrAssociation();

        /**
         * Default copy constructor
         */
        ClassOrThingOrPropertyOrAssociation( const ClassOrThingOrPropertyOrAssociation& );
        ClassOrThingOrPropertyOrAssociation( const Resource& );

        /**
         * Create a new ClassOrThingOrPropertyOrAssociation instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        ClassOrThingOrPropertyOrAssociation( const QString& uriOrIdentifier );

        /**
         * Create a new ClassOrThingOrPropertyOrAssociation instance representing the resource
         * referenced by \a uri.
         */
        ClassOrThingOrPropertyOrAssociation( const QUrl& uri );
        ~ClassOrThingOrPropertyOrAssociation();

        ClassOrThingOrPropertyOrAssociation& operator=( const ClassOrThingOrPropertyOrAssociation& );

        /**
         * Get property 'isDefinedBy'. Each element in a PIMO must be connected 
         * to the PIMO, to be able to track multiple PIMOs in a distributed 
         * scenario. Also, this is the way to find the user that this Thing 
         * belongs to. 
         */
        QList<PersonalInformationModel> isDefinedBys() const;

        /**
         * Set property 'isDefinedBy'. Each element in a PIMO must be connected 
         * to the PIMO, to be able to track multiple PIMOs in a distributed 
         * scenario. Also, this is the way to find the user that this Thing 
         * belongs to. 
         */
        void setIsDefinedBys( const QList<PersonalInformationModel>& value );

        /**
         * Add a value to property 'isDefinedBy'. Each element in a PIMO 
         * must be connected to the PIMO, to be able to track multiple PIMOs 
         * in a distributed scenario. Also, this is the way to find the user 
         * that this Thing belongs to. 
         */
        void addIsDefinedBy( const PersonalInformationModel& value );

        /**
         * Get property 'isDefinedBy'. Each element in a PIMO must be connected 
         * to the PIMO, to be able to track multiple PIMOs in a distributed 
         * scenario. Also, this is the way to find the user that this Thing 
         * belongs to. 
         */
        PersonalInformationModel isDefinedBy() const;

        /**
         * Set property 'isDefinedBy'. Each element in a PIMO must be connected 
         * to the PIMO, to be able to track multiple PIMOs in a distributed 
         * scenario. Also, this is the way to find the user that this Thing 
         * belongs to. 
         */
        void setIsDefinedBy( const PersonalInformationModel& value );

        /**
         * \return The URI of the property 'isDefinedBy'. 
         */
        static QUrl isDefinedByUri();

        /**
         * Retrieve a list of all available ClassOrThingOrPropertyOrAssociation 
         * resources. This list consists of all resource of type ClassOrThingOrPropertyOrAssociation 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<ClassOrThingOrPropertyOrAssociation> allClassOrThingOrPropertyOrAssociations();


        /**
         * \return The URI of the resource type that is used in ClassOrThingOrPropertyOrAssociation instances.
         */
        static QString resourceTypeUri();

    protected:
       ClassOrThingOrPropertyOrAssociation( const QString& uri, const QUrl& type );
       ClassOrThingOrPropertyOrAssociation( const QUrl& uri, const QUrl& type );
    };
}

#endif
