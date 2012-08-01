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
#include "document.h"

#include "publication.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Document::Document()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Document") )
{
}



Nepomuk2::Document::Document( const Document& res )
  : Resource( res )
{
}


Nepomuk2::Document::Document( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::Document::Document( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Document") )
{
}

Nepomuk2::Document::Document( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Document") )
{
}

Nepomuk2::Document::Document( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::Document::Document( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::Document::~Document()
{
}

Nepomuk2::Document& Nepomuk2::Document::operator=( const Document& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Document::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Document");
}

QList<Nepomuk2::Publication> Nepomuk2::Document::publishedAses() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Publication> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.example.com/nbib#publishedAs") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Publication( *it ) );
    return rl;
}

void Nepomuk2::Document::setPublishedAses( const QList<Nepomuk2::Publication>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Publication>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.example.com/nbib#publishedAs"), Variant( l ) );
}

void Nepomuk2::Document::addPublishedAs( const Nepomuk2::Publication& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.example.com/nbib#publishedAs") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.example.com/nbib#publishedAs"), v );
}

QUrl Nepomuk2::Document::publishedAsUri()
{
    return QUrl::fromEncoded("http://www.example.com/nbib#publishedAs");
}


