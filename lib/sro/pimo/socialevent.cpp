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
#include "socialevent.h"

#include "event.h"
#include "person.h"
#include "locatable.h"
#include "attendee.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::SocialEvent::SocialEvent()
  : Locatable( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#SocialEvent") )
{
}



Nepomuk2::SocialEvent::SocialEvent( const SocialEvent& res )
  : Locatable( res )
{
}


Nepomuk2::SocialEvent::SocialEvent( const Nepomuk2::Resource& res )
  : Locatable( res )
{
}


Nepomuk2::SocialEvent::SocialEvent( const QString& uri )
  : Locatable( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#SocialEvent") )
{
}

Nepomuk2::SocialEvent::SocialEvent( const QUrl& uri )
  : Locatable( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#SocialEvent") )
{
}

Nepomuk2::SocialEvent::SocialEvent( const QString& uri, const QUrl& type )
  : Locatable( uri, type )
{
}


Nepomuk2::SocialEvent::SocialEvent( const QUrl& uri, const QUrl& type )
  : Locatable( uri, type )
{
}

Nepomuk2::SocialEvent::~SocialEvent()
{
}

Nepomuk2::SocialEvent& Nepomuk2::SocialEvent::operator=( const SocialEvent& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::SocialEvent::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#SocialEvent");
}

QList<Nepomuk2::Person> Nepomuk2::SocialEvent::attendees() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Person> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#attendee") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Person( *it ) );
    return rl;
}

void Nepomuk2::SocialEvent::setAttendees( const QList<Nepomuk2::Person>& value )
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
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#attendee"), Variant( l ) );
}

void Nepomuk2::SocialEvent::addAttendee( const Nepomuk2::Person& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#attendee") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#attendee"), v );
}

QUrl Nepomuk2::SocialEvent::attendeeUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#attendee");
}

Nepomuk2::Event Nepomuk2::SocialEvent::toEvent() const
{
    return Nepomuk2::Event( *this );
}

Nepomuk2::Locatable Nepomuk2::SocialEvent::toLocatable() const
{
    return Nepomuk2::Locatable( *this );
}


