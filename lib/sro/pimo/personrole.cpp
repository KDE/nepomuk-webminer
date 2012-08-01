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
#include "personrole.h"

#include "person.h"
#include "thing.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::PersonRole::PersonRole()
  : Association( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#PersonRole") )
{
}



Nepomuk2::PersonRole::PersonRole( const PersonRole& res )
  : Association( res )
{
}


Nepomuk2::PersonRole::PersonRole( const Nepomuk2::Resource& res )
  : Association( res )
{
}


Nepomuk2::PersonRole::PersonRole( const QString& uri )
  : Association( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#PersonRole") )
{
}

Nepomuk2::PersonRole::PersonRole( const QUrl& uri )
  : Association( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#PersonRole") )
{
}

Nepomuk2::PersonRole::PersonRole( const QString& uri, const QUrl& type )
  : Association( uri, type )
{
}


Nepomuk2::PersonRole::PersonRole( const QUrl& uri, const QUrl& type )
  : Association( uri, type )
{
}

Nepomuk2::PersonRole::~PersonRole()
{
}

Nepomuk2::PersonRole& Nepomuk2::PersonRole::operator=( const PersonRole& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::PersonRole::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#PersonRole");
}

QList<Nepomuk2::Person> Nepomuk2::PersonRole::roleHolders() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Person> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleHolder") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Person( *it ) );
    return rl;
}

void Nepomuk2::PersonRole::setRoleHolders( const QList<Nepomuk2::Person>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Person>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleHolder"), Variant( l ) );
}

void Nepomuk2::PersonRole::addRoleHolder( const Nepomuk2::Person& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleHolder") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleHolder"), v );
}

QUrl Nepomuk2::PersonRole::roleHolderUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleHolder");
}

QList<Nepomuk2::Thing> Nepomuk2::PersonRole::roleContexts() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Thing> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleContext") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Thing( *it ) );
    return rl;
}

void Nepomuk2::PersonRole::setRoleContexts( const QList<Nepomuk2::Thing>& value )
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
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleContext"), Variant( l ) );
}

void Nepomuk2::PersonRole::addRoleContext( const Nepomuk2::Thing& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleContext") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleContext"), v );
}

QUrl Nepomuk2::PersonRole::roleContextUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#roleContext");
}


