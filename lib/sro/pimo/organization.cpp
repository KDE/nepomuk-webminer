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
#include "organization.h"

#include "agent.h"
#include "organizationmember.h"
#include "locatable.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Organization::Organization()
  : Locatable( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Organization") )
{
}



Nepomuk2::Organization::Organization( const Organization& res )
  : Locatable( res )
{
}


Nepomuk2::Organization::Organization( const Nepomuk2::Resource& res )
  : Locatable( res )
{
}


Nepomuk2::Organization::Organization( const QString& uri )
  : Locatable( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Organization") )
{
}

Nepomuk2::Organization::Organization( const QUrl& uri )
  : Locatable( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Organization") )
{
}

Nepomuk2::Organization::Organization( const QString& uri, const QUrl& type )
  : Locatable( uri, type )
{
}


Nepomuk2::Organization::Organization( const QUrl& uri, const QUrl& type )
  : Locatable( uri, type )
{
}

Nepomuk2::Organization::~Organization()
{
}

Nepomuk2::Organization& Nepomuk2::Organization::operator=( const Organization& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Organization::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Organization");
}

QList<Nepomuk2::Agent> Nepomuk2::Organization::organizationMembers() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Agent> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOrganizationMember") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Agent( *it ) );
    return rl;
}

void Nepomuk2::Organization::setOrganizationMembers( const QList<Nepomuk2::Agent>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Agent>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOrganizationMember"), Variant( l ) );
}

void Nepomuk2::Organization::addOrganizationMember( const Nepomuk2::Agent& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOrganizationMember") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOrganizationMember"), v );
}

QUrl Nepomuk2::Organization::organizationMemberUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#hasOrganizationMember");
}

Nepomuk2::Agent Nepomuk2::Organization::toAgent() const
{
    return Nepomuk2::Agent( *this );
}

Nepomuk2::Locatable Nepomuk2::Organization::toLocatable() const
{
    return Nepomuk2::Locatable( *this );
}


