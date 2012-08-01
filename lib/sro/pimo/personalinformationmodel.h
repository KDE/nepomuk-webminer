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

#ifndef _PERSONALINFORMATIONMODEL_H_
#define _PERSONALINFORMATIONMODEL_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class Agent;
    class ClassOrThingOrPropertyOrAssociation;
    class Topic;

}

#include <Nepomuk2/Resource>


namespace Nepomuk2 {

    /**
     * A Personal Information Model (PIMO) of a user. Represents the 
     * sum of all information from the personal knowledge workspace 
     * (in literature also referred to as Personal Space of Information 
     * (PSI)) which a user needs for Personal Information Management 
     * (PIM). 
     */
    class  PersonalInformationModel : public Resource
    {
    public:
        /**
         * Create a new empty and invalid PersonalInformationModel instance
         */
        PersonalInformationModel();

        /**
         * Default copy constructor
         */
        PersonalInformationModel( const PersonalInformationModel& );
        PersonalInformationModel( const Resource& );

        /**
         * Create a new PersonalInformationModel instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        PersonalInformationModel( const QString& uriOrIdentifier );

        /**
         * Create a new PersonalInformationModel instance representing the resource
         * referenced by \a uri.
         */
        PersonalInformationModel( const QUrl& uri );
        ~PersonalInformationModel();

        PersonalInformationModel& operator=( const PersonalInformationModel& );

        /**
         * Get property 'GlobalNamespace'. The global namespace of this 
         * user using the semdesk uri scheme, based on the Global Identifier 
         * of the user. Example semdesk://bob@example.com/things/. 
         * See http://dev.nepomuk.semanticdesktop.org/repos/trunk/doc/2008_09_semdeskurischeme/index.html 
         */
        QStringList globalNamespaces() const;

        /**
         * Set property 'GlobalNamespace'. The global namespace of this 
         * user using the semdesk uri scheme, based on the Global Identifier 
         * of the user. Example semdesk://bob@example.com/things/. 
         * See http://dev.nepomuk.semanticdesktop.org/repos/trunk/doc/2008_09_semdeskurischeme/index.html 
         */
        void setGlobalNamespaces( const QStringList& value );

        /**
         * Add a value to property 'GlobalNamespace'. The global namespace 
         * of this user using the semdesk uri scheme, based on the Global 
         * Identifier of the user. Example semdesk://bob@example.com/things/. 
         * See http://dev.nepomuk.semanticdesktop.org/repos/trunk/doc/2008_09_semdeskurischeme/index.html 
         */
        void addGlobalNamespace( const QString& value );

        /**
         * \return The URI of the property 'GlobalNamespace'. 
         */
        static QUrl globalNamespaceUri();

        /**
         * Get property 'LocalNamespace'. The local namespace of this 
         * user using the semdesk uri scheme, based on the Local Identifier 
         * of the user. Example semdesk://bob@/things/. See http://dev.nepomuk.semanticdesktop.org/repos/trunk/doc/2008_09_semdeskurischeme/index.html 
         */
        QStringList localNamespaces() const;

        /**
         * Set property 'LocalNamespace'. The local namespace of this 
         * user using the semdesk uri scheme, based on the Local Identifier 
         * of the user. Example semdesk://bob@/things/. See http://dev.nepomuk.semanticdesktop.org/repos/trunk/doc/2008_09_semdeskurischeme/index.html 
         */
        void setLocalNamespaces( const QStringList& value );

        /**
         * Add a value to property 'LocalNamespace'. The local namespace 
         * of this user using the semdesk uri scheme, based on the Local 
         * Identifier of the user. Example semdesk://bob@/things/. 
         * See http://dev.nepomuk.semanticdesktop.org/repos/trunk/doc/2008_09_semdeskurischeme/index.html 
         */
        void addLocalNamespace( const QString& value );

        /**
         * \return The URI of the property 'LocalNamespace'. 
         */
        static QUrl localNamespaceUri();

        /**
         * Get property 'RootTopic'. The root topics of this PersonalInformationModel's 
         * topic hierarchy. Every topic that has no pimo:superTopic is 
         * a root topic. Semantically equivalent to skos:hasTopConcept. 
         */
        QList<Topic> rootTopics() const;

        /**
         * Set property 'RootTopic'. The root topics of this PersonalInformationModel's 
         * topic hierarchy. Every topic that has no pimo:superTopic is 
         * a root topic. Semantically equivalent to skos:hasTopConcept. 
         */
        void setRootTopics( const QList<Topic>& value );

        /**
         * Add a value to property 'RootTopic'. The root topics of this 
         * PersonalInformationModel's topic hierarchy. Every topic 
         * that has no pimo:superTopic is a root topic. Semantically equivalent 
         * to skos:hasTopConcept. 
         */
        void addRootTopic( const Topic& value );

        /**
         * \return The URI of the property 'RootTopic'. 
         */
        static QUrl rootTopicUri();

        /**
         * Get property 'creator'. The creator of the Personal Information 
         * Model. A subproperty of NAO:creator. The human being whose 
         * mental models are represented in the PIMO. Range is an Agent. 
         */
        QList<Agent> creators() const;

        /**
         * Set property 'creator'. The creator of the Personal Information 
         * Model. A subproperty of NAO:creator. The human being whose 
         * mental models are represented in the PIMO. Range is an Agent. 
         */
        void setCreators( const QList<Agent>& value );

        /**
         * Add a value to property 'creator'. The creator of the Personal 
         * Information Model. A subproperty of NAO:creator. The human 
         * being whose mental models are represented in the PIMO. Range 
         * is an Agent. 
         */
        void addCreator( const Agent& value );

        /**
         * Get property 'creator'. The creator of the Personal Information 
         * Model. A subproperty of NAO:creator. The human being whose 
         * mental models are represented in the PIMO. Range is an Agent. 
         */
        Agent creator() const;

        /**
         * Set property 'creator'. The creator of the Personal Information 
         * Model. A subproperty of NAO:creator. The human being whose 
         * mental models are represented in the PIMO. Range is an Agent. 
         */
        void setCreator( const Agent& value );

        /**
         * \return The URI of the property 'creator'. 
         */
        static QUrl creatorUri();

        /**
         * Get all resources that have this resource set as property 'isDefinedBy'. 
         * Each element in a PIMO must be connected to the PIMO, to be able 
         * to track multiple PIMOs in a distributed scenario. Also, this 
         * is the way to find the user that this Thing belongs to. \sa ResourceManager::allResourcesWithProperty 
         */
        QList<ClassOrThingOrPropertyOrAssociation> isDefinedByOf() const;

        /**
         * Retrieve a list of all available PersonalInformationModel 
         * resources. This list consists of all resource of type PersonalInformationModel 
         * that are stored in the local Nepomuk meta data storage and any 
         * changes made locally. Be aware that in some cases this list can 
         * get very big. Then it might be better to use libKNep directly. 
         */
        static QList<PersonalInformationModel> allPersonalInformationModels();


        /**
         * \return The URI of the resource type that is used in PersonalInformationModel instances.
         */
        static QString resourceTypeUri();

    protected:
       PersonalInformationModel( const QString& uri, const QUrl& type );
       PersonalInformationModel( const QUrl& uri, const QUrl& type );
    };
}

#endif
