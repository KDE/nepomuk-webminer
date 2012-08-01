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
#include "video.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Video::Video()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Video") )
{
}



Nepomuk2::Video::Video( const Video& res )
  : Resource( res )
{
}


Nepomuk2::Video::Video( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::Video::Video( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Video") )
{
}

Nepomuk2::Video::Video( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Video") )
{
}

Nepomuk2::Video::Video( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::Video::Video( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::Video::~Video()
{
}

Nepomuk2::Video& Nepomuk2::Video::operator=( const Video& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Video::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Video");
}

QStringList Nepomuk2::Video::synopsises() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis") ).toStringList());
}

void Nepomuk2::Video::setSynopsises( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis"), Variant( value ) );
}

void Nepomuk2::Video::addSynopsis( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis"), v );
}

QString Nepomuk2::Video::synopsis() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis") ).toStringList() << QString() ).first();
}

void Nepomuk2::Video::setSynopsis( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis"), Variant( value ) );
}

QUrl Nepomuk2::Video::synopsisUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#synopsis");
}

QStringList Nepomuk2::Video::audienceRatings() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#audienceRating") ).toStringList());
}

void Nepomuk2::Video::setAudienceRatings( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#audienceRating"), Variant( value ) );
}

void Nepomuk2::Video::addAudienceRating( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#audienceRating") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#audienceRating"), v );
}

QUrl Nepomuk2::Video::audienceRatingUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#audienceRating");
}

QList<Nepomuk2::Resource> Nepomuk2::Video::writers() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#writer") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Video::setWriters( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#writer"), Variant( value ) );
}

void Nepomuk2::Video::addWriter( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#writer") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#writer"), v );
}

QUrl Nepomuk2::Video::writerUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#writer");
}

QList<Nepomuk2::Resource> Nepomuk2::Video::directors() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#director") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Video::setDirectors( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#director"), Variant( value ) );
}

void Nepomuk2::Video::addDirector( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#director") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#director"), v );
}

QUrl Nepomuk2::Video::directorUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#director");
}

QList<Nepomuk2::Resource> Nepomuk2::Video::producers() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#producer") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Video::setProducers( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#producer"), Variant( value ) );
}

void Nepomuk2::Video::addProducer( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#producer") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#producer"), v );
}

QUrl Nepomuk2::Video::producerUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#producer");
}

QList<Nepomuk2::Resource> Nepomuk2::Video::actors() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#actor") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Video::setActors( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#actor"), Variant( value ) );
}

void Nepomuk2::Video::addActor( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#actor") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#actor"), v );
}

QUrl Nepomuk2::Video::actorUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#actor");
}

QList<Nepomuk2::Resource> Nepomuk2::Video::cinematographers() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#cinematographer") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Video::setCinematographers( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#cinematographer"), Variant( value ) );
}

void Nepomuk2::Video::addCinematographer( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#cinematographer") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#cinematographer"), v );
}

QUrl Nepomuk2::Video::cinematographerUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#cinematographer");
}

QList<Nepomuk2::Resource> Nepomuk2::Video::assistantDirectors() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#assistantDirector") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Video::setAssistantDirectors( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#assistantDirector"), Variant( value ) );
}

void Nepomuk2::Video::addAssistantDirector( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#assistantDirector") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#assistantDirector"), v );
}

QUrl Nepomuk2::Video::assistantDirectorUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2009/02/19/nmm#assistantDirector");
}


