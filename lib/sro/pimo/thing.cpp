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

#include <nepomuk2/variant.h>
#include <nepomuk2/resourcemanager.h>
#include "thing.h"

#include "personrole.h"
#include "tag.h"
#include "classorthing.h"
#include "thing.h"
#include "association.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Thing::Thing()
  : ClassOrThing( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Thing") )
{
}



Nepomuk2::Thing::Thing( const Thing& res )
  : ClassOrThing( res )
{
}


Nepomuk2::Thing::Thing( const Nepomuk2::Resource& res )
  : ClassOrThing( res )
{
}


Nepomuk2::Thing::Thing( const QString& uri )
  : ClassOrThing( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Thing") )
{
}

Nepomuk2::Thing::Thing( const QUrl& uri )
  : ClassOrThing( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Thing") )
{
}

Nepomuk2::Thing::Thing( const QString& uri, const QUrl& type )
  : ClassOrThing( uri, type )
{
}


Nepomuk2::Thing::Thing( const QUrl& uri, const QUrl& type )
  : ClassOrThing( uri, type )
{
}

Nepomuk2::Thing::~Thing()
{
}

Nepomuk2::Thing& Nepomuk2::Thing::operator=( const Thing& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Thing::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Thing");
}

QList<Nepomuk2::Tag> Nepomuk2::Thing::tags() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Tag> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasTag") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Tag( *it ) );
    return rl;
}

void Nepomuk2::Thing::setTags( const QList<Nepomuk2::Tag>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Tag>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasTag"), Variant( l ) );
}

void Nepomuk2::Thing::addTag( const Nepomuk2::Tag& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasTag") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasTag"), v );
}

QUrl Nepomuk2::Thing::tagUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasTag");
}

QList<Nepomuk2::Resource> Nepomuk2::Thing::otherRepresentations() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOtherRepresentation") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Thing::setOtherRepresentations( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOtherRepresentation"), Variant( value ) );
}

void Nepomuk2::Thing::addOtherRepresentation( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOtherRepresentation") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOtherRepresentation"), v );
}

QUrl Nepomuk2::Thing::otherRepresentationUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOtherRepresentation");
}

QList<Nepomuk2::Thing> Nepomuk2::Thing::objectPropertys() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Thing> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#objectProperty") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Thing( *it ) );
    return rl;
}

void Nepomuk2::Thing::setObjectPropertys( const QList<Nepomuk2::Thing>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Thing>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#objectProperty"), Variant( l ) );
}

void Nepomuk2::Thing::addObjectProperty( const Nepomuk2::Thing& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#objectProperty") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#objectProperty"), v );
}

QUrl Nepomuk2::Thing::objectPropertyUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#objectProperty");
}

QList<Nepomuk2::Resource> Nepomuk2::Thing::deprecatedRepresentations() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasDeprecatedRepresentation") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Thing::setDeprecatedRepresentations( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasDeprecatedRepresentation"), Variant( value ) );
}

void Nepomuk2::Thing::addDeprecatedRepresentation( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasDeprecatedRepresentation") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasDeprecatedRepresentation"), v );
}

QUrl Nepomuk2::Thing::deprecatedRepresentationUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasDeprecatedRepresentation");
}

QList<Nepomuk2::Thing> Nepomuk2::Thing::isRelateds() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Thing> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isRelated") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Thing( *it ) );
    return rl;
}

void Nepomuk2::Thing::setIsRelateds( const QList<Nepomuk2::Thing>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Thing>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isRelated"), Variant( l ) );
}

void Nepomuk2::Thing::addIsRelated( const Nepomuk2::Thing& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isRelated") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isRelated"), v );
}

QUrl Nepomuk2::Thing::isRelatedUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isRelated");
}

QList<Nepomuk2::Resource> Nepomuk2::Thing::groundingOccurrences() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingOccurrence") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Thing::setGroundingOccurrences( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingOccurrence"), Variant( value ) );
}

void Nepomuk2::Thing::addGroundingOccurrence( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingOccurrence") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingOccurrence"), v );
}

QUrl Nepomuk2::Thing::groundingOccurrenceUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingOccurrence");
}

QList<Nepomuk2::Thing> Nepomuk2::Thing::partOfs() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Thing> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#partOf") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Thing( *it ) );
    return rl;
}

void Nepomuk2::Thing::setPartOfs( const QList<Nepomuk2::Thing>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Thing>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#partOf"), Variant( l ) );
}

void Nepomuk2::Thing::addPartOf( const Nepomuk2::Thing& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#partOf") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#partOf"), v );
}

QUrl Nepomuk2::Thing::partOfUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#partOf");
}

QList<Nepomuk2::Resource> Nepomuk2::Thing::occurrences() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#occurrence") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Thing::setOccurrences( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#occurrence"), Variant( value ) );
}

void Nepomuk2::Thing::addOccurrence( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#occurrence") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#occurrence"), v );
}

QUrl Nepomuk2::Thing::occurrenceUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#occurrence");
}

QList<Nepomuk2::Resource> Nepomuk2::Thing::referencingOccurrences() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#referencingOccurrence") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Thing::setReferencingOccurrences( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#referencingOccurrence"), Variant( value ) );
}

void Nepomuk2::Thing::addReferencingOccurrence( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#referencingOccurrence") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#referencingOccurrence"), v );
}

QUrl Nepomuk2::Thing::referencingOccurrenceUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#referencingOccurrence");
}

QList<Nepomuk2::Thing> Nepomuk2::Thing::parts() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Thing> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasPart") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Thing( *it ) );
    return rl;
}

void Nepomuk2::Thing::setParts( const QList<Nepomuk2::Thing>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Thing>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasPart"), Variant( l ) );
}

void Nepomuk2::Thing::addPart( const Nepomuk2::Thing& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasPart") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasPart"), v );
}

QUrl Nepomuk2::Thing::partUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasPart");
}

Nepomuk2::ClassOrThing Nepomuk2::Thing::toClassOrThing() const
{
    return Nepomuk2::ClassOrThing( *this );
}


