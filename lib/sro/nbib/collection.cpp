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
#include "collection.h"

#include "article.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Collection::Collection()
  : Publication( QUrl(), QUrl::fromEncoded("http://www.example.com/nbib#Collection") )
{
}



Nepomuk2::Collection::Collection( const Collection& res )
  : Publication( res )
{
}


Nepomuk2::Collection::Collection( const Nepomuk2::Resource& res )
  : Publication( res )
{
}


Nepomuk2::Collection::Collection( const QString& uri )
  : Publication( uri, QUrl::fromEncoded("http://www.example.com/nbib#Collection") )
{
}

Nepomuk2::Collection::Collection( const QUrl& uri )
  : Publication( uri, QUrl::fromEncoded("http://www.example.com/nbib#Collection") )
{
}

Nepomuk2::Collection::Collection( const QString& uri, const QUrl& type )
  : Publication( uri, type )
{
}


Nepomuk2::Collection::Collection( const QUrl& uri, const QUrl& type )
  : Publication( uri, type )
{
}

Nepomuk2::Collection::~Collection()
{
}

Nepomuk2::Collection& Nepomuk2::Collection::operator=( const Collection& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Collection::resourceTypeUri()
{
    return QLatin1String("http://www.example.com/nbib#Collection");
}

QList<Nepomuk2::Article> Nepomuk2::Collection::articles() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Article> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.example.com/nbib#article") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Article( *it ) );
    return rl;
}

void Nepomuk2::Collection::setArticles( const QList<Nepomuk2::Article>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Article>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.example.com/nbib#article"), Variant( l ) );
}

void Nepomuk2::Collection::addArticle( const Nepomuk2::Article& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.example.com/nbib#article") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.example.com/nbib#article"), v );
}

QUrl Nepomuk2::Collection::articleUri()
{
    return QUrl::fromEncoded("http://www.example.com/nbib#article");
}


