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

#ifndef _THING_H_
#define _THING_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class Tag;
    class Association;
    class Thing;
    class ClassOrThing;
    class Resource;
    class PersonRole;

}

#include "classorthing.h"


namespace Nepomuk2 {

    /**
     * Entities that are in the direct attention of the user when doing 
     * knowledge work. 
     */
    class  Thing : public ClassOrThing
    {
    public:
        /**
         * Create a new empty and invalid Thing instance
         */
        Thing();

        /**
         * Default copy constructor
         */
        Thing( const Thing& );
        Thing( const Resource& );

        /**
         * Create a new Thing instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        Thing( const QString& uriOrIdentifier );

        /**
         * Create a new Thing instance representing the resource
         * referenced by \a uri.
         */
        Thing( const QUrl& uri );
        ~Thing();

        Thing& operator=( const Thing& );

        /**
         * Get property 'Tag'. The subject's contents describes the object. 
         * Or the subject can be seen as belonging to the thing described 
         * by the object. Similar semantics as skos:subject. 
         */
        QList<Tag> tags() const;

        /**
         * Set property 'Tag'. The subject's contents describes the object. 
         * Or the subject can be seen as belonging to the thing described 
         * by the object. Similar semantics as skos:subject. 
         */
        void setTags( const QList<Tag>& value );

        /**
         * Add a value to property 'Tag'. The subject's contents describes 
         * the object. Or the subject can be seen as belonging to the thing 
         * described by the object. Similar semantics as skos:subject. 
         */
        void addTag( const Tag& value );

        /**
         * \return The URI of the property 'Tag'. 
         */
        static QUrl tagUri();

        /**
         * Get property 'OtherRepresentation'. hasOtherRepresentation 
         * points from a Thing in your PIMO to a thing in an ontology that 
         * represents the same real world thing. This means that the real 
         * world object O represented by an instance I1 has additional 
         * representations (as instances I2-In of different conceptualizations). 
         * This means: IF (I_i represents O_j in Ontology_k) AND (I_m represents 
         * O_n in Ontology_o) THEN (O_n and O_j are the same object). hasOtherRepresentation 
         * is a transitive relation, but not equivalent (not symmetric 
         * nor reflexive). For example, the URI of a foaf:Person representation 
         * published on the web is a hasOtherRepresentation for the person. 
         * This property is inverse functional, two Things from two information 
         * models having the same hasOtherRepresentation are considered 
         * to be representations of the same entity from the real world. 
         * TODO: rename this to subjectIndicatorRef to resemble topic 
         * maps ideas? 
         */
        QList<Resource> otherRepresentations() const;

        /**
         * Set property 'OtherRepresentation'. hasOtherRepresentation 
         * points from a Thing in your PIMO to a thing in an ontology that 
         * represents the same real world thing. This means that the real 
         * world object O represented by an instance I1 has additional 
         * representations (as instances I2-In of different conceptualizations). 
         * This means: IF (I_i represents O_j in Ontology_k) AND (I_m represents 
         * O_n in Ontology_o) THEN (O_n and O_j are the same object). hasOtherRepresentation 
         * is a transitive relation, but not equivalent (not symmetric 
         * nor reflexive). For example, the URI of a foaf:Person representation 
         * published on the web is a hasOtherRepresentation for the person. 
         * This property is inverse functional, two Things from two information 
         * models having the same hasOtherRepresentation are considered 
         * to be representations of the same entity from the real world. 
         * TODO: rename this to subjectIndicatorRef to resemble topic 
         * maps ideas? 
         */
        void setOtherRepresentations( const QList<Resource>& value );

        /**
         * Add a value to property 'OtherRepresentation'. hasOtherRepresentation 
         * points from a Thing in your PIMO to a thing in an ontology that 
         * represents the same real world thing. This means that the real 
         * world object O represented by an instance I1 has additional 
         * representations (as instances I2-In of different conceptualizations). 
         * This means: IF (I_i represents O_j in Ontology_k) AND (I_m represents 
         * O_n in Ontology_o) THEN (O_n and O_j are the same object). hasOtherRepresentation 
         * is a transitive relation, but not equivalent (not symmetric 
         * nor reflexive). For example, the URI of a foaf:Person representation 
         * published on the web is a hasOtherRepresentation for the person. 
         * This property is inverse functional, two Things from two information 
         * models having the same hasOtherRepresentation are considered 
         * to be representations of the same entity from the real world. 
         * TODO: rename this to subjectIndicatorRef to resemble topic 
         * maps ideas? 
         */
        void addOtherRepresentation( const Resource& value );

        /**
         * \return The URI of the property 'OtherRepresentation'. 
         */
        static QUrl otherRepresentationUri();

        /**
         * Get property 'objectProperty'. The object of statements is 
         * another Thing. Users should be able to edit statements defined 
         * with this property. Abstract super-property. 
         */
        QList<Thing> objectPropertys() const;

        /**
         * Set property 'objectProperty'. The object of statements is 
         * another Thing. Users should be able to edit statements defined 
         * with this property. Abstract super-property. 
         */
        void setObjectPropertys( const QList<Thing>& value );

        /**
         * Add a value to property 'objectProperty'. The object of statements 
         * is another Thing. Users should be able to edit statements defined 
         * with this property. Abstract super-property. 
         */
        void addObjectProperty( const Thing& value );

        /**
         * \return The URI of the property 'objectProperty'. 
         */
        static QUrl objectPropertyUri();

        /**
         * Get property 'DeprecatedRepresentation'. The subject Thing 
         * was represented previously using the object resource. This 
         * indicates that the object resource was a duplicate representation 
         * of the subject and merged with the subject. Implementations 
         * can use this property to resolve dangling links in distributed 
         * system. When encountering resources that are deprecated representations 
         * of a Thing, they should be replaced with the Thing. The range 
         * is not declared as we assume all knowledge about the object is 
         * gone, including its rdf:type. 
         */
        QList<Resource> deprecatedRepresentations() const;

        /**
         * Set property 'DeprecatedRepresentation'. The subject Thing 
         * was represented previously using the object resource. This 
         * indicates that the object resource was a duplicate representation 
         * of the subject and merged with the subject. Implementations 
         * can use this property to resolve dangling links in distributed 
         * system. When encountering resources that are deprecated representations 
         * of a Thing, they should be replaced with the Thing. The range 
         * is not declared as we assume all knowledge about the object is 
         * gone, including its rdf:type. 
         */
        void setDeprecatedRepresentations( const QList<Resource>& value );

        /**
         * Add a value to property 'DeprecatedRepresentation'. The subject 
         * Thing was represented previously using the object resource. 
         * This indicates that the object resource was a duplicate representation 
         * of the subject and merged with the subject. Implementations 
         * can use this property to resolve dangling links in distributed 
         * system. When encountering resources that are deprecated representations 
         * of a Thing, they should be replaced with the Thing. The range 
         * is not declared as we assume all knowledge about the object is 
         * gone, including its rdf:type. 
         */
        void addDeprecatedRepresentation( const Resource& value );

        /**
         * \return The URI of the property 'DeprecatedRepresentation'. 
         */
        static QUrl deprecatedRepresentationUri();

        /**
         * Get property 'isRelated'. The thing is related to the other 
         * thing. Similar in meaning to skos:related. Symmetric but not 
         * transitive. 
         */
        QList<Thing> isRelateds() const;

        /**
         * Set property 'isRelated'. The thing is related to the other 
         * thing. Similar in meaning to skos:related. Symmetric but not 
         * transitive. 
         */
        void setIsRelateds( const QList<Thing>& value );

        /**
         * Add a value to property 'isRelated'. The thing is related to 
         * the other thing. Similar in meaning to skos:related. Symmetric 
         * but not transitive. 
         */
        void addIsRelated( const Thing& value );

        /**
         * \return The URI of the property 'isRelated'. 
         */
        static QUrl isRelatedUri();

        /**
         * Get property 'groundingOccurrence'. The subject Thing represents 
         * the entity that is described in the object InformationElement. 
         * The subject Thing is the canonical, unique representation 
         * in the personal information model for the entity described 
         * in the object. Multiple InformationElements can be the grounding 
         * occurrence of the same Thing, one InformationElement can be 
         * the groundingOccurrence of only one Thing. 
         */
        QList<Resource> groundingOccurrences() const;

        /**
         * Set property 'groundingOccurrence'. The subject Thing represents 
         * the entity that is described in the object InformationElement. 
         * The subject Thing is the canonical, unique representation 
         * in the personal information model for the entity described 
         * in the object. Multiple InformationElements can be the grounding 
         * occurrence of the same Thing, one InformationElement can be 
         * the groundingOccurrence of only one Thing. 
         */
        void setGroundingOccurrences( const QList<Resource>& value );

        /**
         * Add a value to property 'groundingOccurrence'. The subject 
         * Thing represents the entity that is described in the object 
         * InformationElement. The subject Thing is the canonical, unique 
         * representation in the personal information model for the entity 
         * described in the object. Multiple InformationElements can 
         * be the grounding occurrence of the same Thing, one InformationElement 
         * can be the groundingOccurrence of only one Thing. 
         */
        void addGroundingOccurrence( const Resource& value );

        /**
         * \return The URI of the property 'groundingOccurrence'. 
         */
        static QUrl groundingOccurrenceUri();

        /**
         * Get property 'partOf'. This is part of the object. Like a page 
         * is part of a book or an engine is part of a car. You can make sub-properties 
         * of this to reflect more detailed relations. 
         */
        QList<Thing> partOfs() const;

        /**
         * Set property 'partOf'. This is part of the object. Like a page 
         * is part of a book or an engine is part of a car. You can make sub-properties 
         * of this to reflect more detailed relations. 
         */
        void setPartOfs( const QList<Thing>& value );

        /**
         * Add a value to property 'partOf'. This is part of the object. 
         * Like a page is part of a book or an engine is part of a car. You can 
         * make sub-properties of this to reflect more detailed relations. 
         */
        void addPartOf( const Thing& value );

        /**
         * \return The URI of the property 'partOf'. 
         */
        static QUrl partOfUri();

        /**
         * Get property 'occurrence'. The subject Thing is represented 
         * also in the object resource. All facts added to the object resource 
         * are valid for the subject thing. The subject is the canonical 
         * represtation of the object. In particual, this implies when 
         * (?object ?p ?v) -> (?subject ?p ?v) and (?s ?p ?object) -> (?s 
         * ?p ?subject). The class of the object is not defined, but should 
         * be compatible with the class of the subject. Occurrence relations 
         * can be inferred through same identifiers or referencingOccurrence 
         * relations. 
         */
        QList<Resource> occurrences() const;

        /**
         * Set property 'occurrence'. The subject Thing is represented 
         * also in the object resource. All facts added to the object resource 
         * are valid for the subject thing. The subject is the canonical 
         * represtation of the object. In particual, this implies when 
         * (?object ?p ?v) -> (?subject ?p ?v) and (?s ?p ?object) -> (?s 
         * ?p ?subject). The class of the object is not defined, but should 
         * be compatible with the class of the subject. Occurrence relations 
         * can be inferred through same identifiers or referencingOccurrence 
         * relations. 
         */
        void setOccurrences( const QList<Resource>& value );

        /**
         * Add a value to property 'occurrence'. The subject Thing is represented 
         * also in the object resource. All facts added to the object resource 
         * are valid for the subject thing. The subject is the canonical 
         * represtation of the object. In particual, this implies when 
         * (?object ?p ?v) -> (?subject ?p ?v) and (?s ?p ?object) -> (?s 
         * ?p ?subject). The class of the object is not defined, but should 
         * be compatible with the class of the subject. Occurrence relations 
         * can be inferred through same identifiers or referencingOccurrence 
         * relations. 
         */
        void addOccurrence( const Resource& value );

        /**
         * \return The URI of the property 'occurrence'. 
         */
        static QUrl occurrenceUri();

        /**
         * Get property 'referencingOccurrence'. The subject thing 
         * is described in the object document. Ideally, the document 
         * is public and its primary topic is the thing. Although this property 
         * is not inverse-functional (because the Occurrences are not 
         * canonical elements of a formal ontology) this property allows 
         * to use public documents, such as wikipedia pages, as indicators 
         * identity. The more formal hasOtherRepresentation property 
         * can be used when an ontology about the subject exists. 
         */
        QList<Resource> referencingOccurrences() const;

        /**
         * Set property 'referencingOccurrence'. The subject thing 
         * is described in the object document. Ideally, the document 
         * is public and its primary topic is the thing. Although this property 
         * is not inverse-functional (because the Occurrences are not 
         * canonical elements of a formal ontology) this property allows 
         * to use public documents, such as wikipedia pages, as indicators 
         * identity. The more formal hasOtherRepresentation property 
         * can be used when an ontology about the subject exists. 
         */
        void setReferencingOccurrences( const QList<Resource>& value );

        /**
         * Add a value to property 'referencingOccurrence'. The subject 
         * thing is described in the object document. Ideally, the document 
         * is public and its primary topic is the thing. Although this property 
         * is not inverse-functional (because the Occurrences are not 
         * canonical elements of a formal ontology) this property allows 
         * to use public documents, such as wikipedia pages, as indicators 
         * identity. The more formal hasOtherRepresentation property 
         * can be used when an ontology about the subject exists. 
         */
        void addReferencingOccurrence( const Resource& value );

        /**
         * \return The URI of the property 'referencingOccurrence'. 
         */
        static QUrl referencingOccurrenceUri();

        /**
         * Get property 'Part'. The object is part of the subject. Like 
         * a page is part of a book or an engine is part of a car. You can make 
         * sub-properties of this to reflect more detailed relations. 
         * The semantics of this relations is the same as skos:narrowerPartitive 
         */
        QList<Thing> parts() const;

        /**
         * Set property 'Part'. The object is part of the subject. Like 
         * a page is part of a book or an engine is part of a car. You can make 
         * sub-properties of this to reflect more detailed relations. 
         * The semantics of this relations is the same as skos:narrowerPartitive 
         */
        void setParts( const QList<Thing>& value );

        /**
         * Add a value to property 'Part'. The object is part of the subject. 
         * Like a page is part of a book or an engine is part of a car. You can 
         * make sub-properties of this to reflect more detailed relations. 
         * The semantics of this relations is the same as skos:narrowerPartitive 
         */
        void addPart( const Thing& value );

        /**
         * \return The URI of the property 'Part'. 
         */
        static QUrl partUri();

        /**
         * Get all resources that have this resource set as property 'associationMember'. 
         * An super-property of all roles that an entity can have in an association. 
         * Member is the generic role of a thing in an association. Association 
         * subclasses should define sub-properties of this property. 
         * Associations can have Things as \sa ResourceManager::allResourcesWithProperty 
         */
        QList<Association> associationMemberOf() const;

        /**
         * Get all resources that have this resource set as property 'objectProperty'. 
         * The object of statements is another Thing. Users should be able 
         * to edit statements defined with this property. Abstract super-property. 
         * \sa ResourceManager::allResourcesWithProperty 
         */
        QList<Thing> objectPropertyOf() const;

        /**
         * Get all resources that have this resource set as property 'roleContext'. 
         * The context where the role-holder impersonates this role. 
         * For example, the company where a person is employed. \sa ResourceManager::allResourcesWithProperty 
         */
        QList<PersonRole> roleContextOf() const;

        /**
         * Nepomuk does not support multiple inheritance. Thus, to access 
         * properties from all parent classes helper methods like this 
         * are introduced. The object returned represents the exact same 
         * resource. 
         */
        ClassOrThing toClassOrThing() const;

        /**
         * Retrieve a list of all available Thing resources. This list 
         * consists of all resource of type Thing that are stored in the 
         * local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<Thing> allThings();


        /**
         * \return The URI of the resource type that is used in Thing instances.
         */
        static QString resourceTypeUri();

    protected:
       Thing( const QString& uri, const QUrl& type );
       Thing( const QUrl& uri, const QUrl& type );
    };
}

#endif
