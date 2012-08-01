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
#include "tvseries.h"

#include "tvseason.h"
#include "tvshow.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::TVSeries::TVSeries()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#TVSeries") )
{
}



Nepomuk2::TVSeries::TVSeries( const TVSeries& res )
  : Resource( res )
{
}


Nepomuk2::TVSeries::TVSeries( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::TVSeries::TVSeries( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#TVSeries") )
{
}

Nepomuk2::TVSeries::TVSeries( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#TVSeries") )
{
}

Nepomuk2::TVSeries::TVSeries( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::TVSeries::TVSeries( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::TVSeries::~TVSeries()
{
}

Nepomuk2::TVSeries& Nepomuk2::TVSeries::operator=( const TVSeries& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::TVSeries::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#TVSeries");
}

QList<Nepomuk2::TVShow> Nepomuk2::TVSeries::episodes() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<TVShow> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasEpisode") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( TVShow( *it ) );
    return rl;
}

void Nepomuk2::TVSeries::setEpisodes( const QList<Nepomuk2::TVShow>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<TVShow>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasEpisode"), Variant( l ) );
}

void Nepomuk2::TVSeries::addEpisode( const Nepomuk2::TVShow& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasEpisode") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasEpisode"), v );
}

QUrl Nepomuk2::TVSeries::episodeUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasEpisode");
}

QList<Nepomuk2::TVSeason> Nepomuk2::TVSeries::seasons() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<TVSeason> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasSeason") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( TVSeason( *it ) );
    return rl;
}

void Nepomuk2::TVSeries::setSeasons( const QList<Nepomuk2::TVSeason>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<TVSeason>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasSeason"), Variant( l ) );
}

void Nepomuk2::TVSeries::addSeason( const Nepomuk2::TVSeason& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasSeason") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasSeason"), v );
}

QUrl Nepomuk2::TVSeries::seasonUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#hasSeason");
}


