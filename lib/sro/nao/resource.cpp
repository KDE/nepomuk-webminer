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
#include "resource.h"

#include "instancebase.h"
#include "tag.h"
#include "symbol.h"
#include "agent.h"
#include "party.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Resource::Resource()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.w3.org/2000/01/rdf-schema#Resource") )
{
}



Nepomuk2::Resource::Resource( const Resource& res )
  : Resource( res )
{
}


Nepomuk2::Resource::Resource( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::Resource::Resource( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.w3.org/2000/01/rdf-schema#Resource") )
{
}

Nepomuk2::Resource::Resource( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.w3.org/2000/01/rdf-schema#Resource") )
{
}

Nepomuk2::Resource::Resource( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::Resource::Resource( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::Resource::~Resource()
{
}

Nepomuk2::Resource& Nepomuk2::Resource::operator=( const Resource& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Resource::resourceTypeUri()
{
    return QLatin1String("http://www.w3.org/2000/01/rdf-schema#Resource");
}

QList<double> Nepomuk2::Resource::scores() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score") ).toDoubleList());
}

void Nepomuk2::Resource::setScores( const QList<double>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score"), Variant( value ) );
}

void Nepomuk2::Resource::addScore( const double& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score"), v );
}

double Nepomuk2::Resource::score() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score") ).toDoubleList() << 0.0 ).first();
}

void Nepomuk2::Resource::setScore( const double& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score"), Variant( value ) );
}

QUrl Nepomuk2::Resource::scoreUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score");
}

QList<double> Nepomuk2::Resource::scoreParameters() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter") ).toDoubleList());
}

void Nepomuk2::Resource::setScoreParameters( const QList<double>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter"), Variant( value ) );
}

void Nepomuk2::Resource::addScoreParameter( const double& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter"), v );
}

QUrl Nepomuk2::Resource::scoreParameterUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter");
}

QList<Nepomuk2::Symbol> Nepomuk2::Resource::symbols() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Symbol> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Symbol( *it ) );
    return rl;
}

void Nepomuk2::Resource::setSymbols( const QList<Nepomuk2::Symbol>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Symbol>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol"), Variant( l ) );
}

void Nepomuk2::Resource::addSymbol( const Nepomuk2::Symbol& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol"), v );
}

QUrl Nepomuk2::Resource::symbolUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol");
}

QList<Nepomuk2::Symbol> Nepomuk2::Resource::prefSymbols() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Symbol> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Symbol( *it ) );
    return rl;
}

void Nepomuk2::Resource::setPrefSymbols( const QList<Nepomuk2::Symbol>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Symbol>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol"), Variant( l ) );
}

void Nepomuk2::Resource::addPrefSymbol( const Nepomuk2::Symbol& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol"), v );
}

Nepomuk2::Symbol Nepomuk2::Resource::prefSymbol() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    return Symbol( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol") ).toResource().uri() );
}

void Nepomuk2::Resource::setPrefSymbol( const Nepomuk2::Symbol& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol"), Variant( value ) );
}

QUrl Nepomuk2::Resource::prefSymbolUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol");
}

QList<Nepomuk2::Symbol> Nepomuk2::Resource::altSymbols() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Symbol> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Symbol( *it ) );
    return rl;
}

void Nepomuk2::Resource::setAltSymbols( const QList<Nepomuk2::Symbol>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Symbol>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol"), Variant( l ) );
}

void Nepomuk2::Resource::addAltSymbol( const Nepomuk2::Symbol& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol"), v );
}

QUrl Nepomuk2::Resource::altSymbolUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol");
}

QList<Nepomuk2::Party> Nepomuk2::Resource::creators() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Party> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Party( *it ) );
    return rl;
}

void Nepomuk2::Resource::setCreators( const QList<Nepomuk2::Party>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Party>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator"), Variant( l ) );
}

void Nepomuk2::Resource::addCreator( const Nepomuk2::Party& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator"), v );
}

Nepomuk2::Party Nepomuk2::Resource::creator() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    return Party( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator") ).toResource().uri() );
}

void Nepomuk2::Resource::setCreator( const Nepomuk2::Party& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator"), Variant( value ) );
}

QUrl Nepomuk2::Resource::creatorUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator");
}

QList<qint64> Nepomuk2::Resource::numericRatings() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating") ).toInt64List());
}

void Nepomuk2::Resource::setNumericRatings( const QList<qint64>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating"), Variant( value ) );
}

void Nepomuk2::Resource::addNumericRating( const qint64& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating"), v );
}

qint64 Nepomuk2::Resource::numericRating() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating") ).toInt64List() << 0 ).first();
}

void Nepomuk2::Resource::setNumericRating( const qint64& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating"), Variant( value ) );
}

QUrl Nepomuk2::Resource::numericRatingUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating");
}

QList<Nepomuk2::Party> Nepomuk2::Resource::contributors() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Party> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Party( *it ) );
    return rl;
}

void Nepomuk2::Resource::setContributors( const QList<Nepomuk2::Party>& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> l;
    for( QList<Party>::const_iterator it = value.constBegin();
         it != value.constEnd(); ++it ) {
        l.append( Resource( (*it) ) );
    }
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor"), Variant( l ) );
}

void Nepomuk2::Resource::addContributor( const Nepomuk2::Party& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor"), v );
}

QUrl Nepomuk2::Resource::contributorUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor");
}

QList<QDateTime> Nepomuk2::Resource::modifieds() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified") ).toDateTimeList());
}

void Nepomuk2::Resource::setModifieds( const QList<QDateTime>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified"), Variant( value ) );
}

void Nepomuk2::Resource::addModified( const QDateTime& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified"), v );
}

QUrl Nepomuk2::Resource::modifiedUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified");
}

QList<QDateTime> Nepomuk2::Resource::createds() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created") ).toDateTimeList());
}

void Nepomuk2::Resource::setCreateds( const QList<QDateTime>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created"), Variant( value ) );
}

void Nepomuk2::Resource::addCreated( const QDateTime& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created"), v );
}

QDateTime Nepomuk2::Resource::created() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created") ).toDateTimeList() << QDateTime() ).first();
}

void Nepomuk2::Resource::setCreated( const QDateTime& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created"), Variant( value ) );
}

QUrl Nepomuk2::Resource::createdUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created");
}

QList<QDateTime> Nepomuk2::Resource::lastModifieds() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified") ).toDateTimeList());
}

void Nepomuk2::Resource::setLastModifieds( const QList<QDateTime>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified"), Variant( value ) );
}

void Nepomuk2::Resource::addLastModified( const QDateTime& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified"), v );
}

QDateTime Nepomuk2::Resource::lastModified() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified") ).toDateTimeList() << QDateTime() ).first();
}

void Nepomuk2::Resource::setLastModified( const QDateTime& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified"), Variant( value ) );
}

QUrl Nepomuk2::Resource::lastModifiedUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified");
}

QList<Nepomuk2::Tag> Nepomuk2::Resource::tags() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Tag> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Tag( *it ) );
    return rl;
}

void Nepomuk2::Resource::setTags( const QList<Nepomuk2::Tag>& value )
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
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag"), Variant( l ) );
}

void Nepomuk2::Resource::addTag( const Nepomuk2::Tag& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag"), v );
}

QUrl Nepomuk2::Resource::tagUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag");
}

QList<Nepomuk2::Resource> Nepomuk2::Resource::deprecateds() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::Resource::setDeprecateds( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated"), Variant( value ) );
}

void Nepomuk2::Resource::addDeprecated( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated"), v );
}

Nepomuk2::Resource Nepomuk2::Resource::deprecated() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    return Resource( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated") ).toResource().uri() );
}

void Nepomuk2::Resource::setDeprecated( const Nepomuk2::Resource& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated"), Variant( value ) );
}

QUrl Nepomuk2::Resource::deprecatedUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated");
}

QList<bool> Nepomuk2::Resource::userVisibles() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible") ).toBoolList());
}

void Nepomuk2::Resource::setUserVisibles( const QList<bool>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible"), Variant( value ) );
}

void Nepomuk2::Resource::addUserVisible( const bool& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible"), v );
}

bool Nepomuk2::Resource::userVisible() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible") ).toBoolList() << false ).first();
}

void Nepomuk2::Resource::setUserVisible( const bool& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible"), Variant( value ) );
}

QUrl Nepomuk2::Resource::userVisibleUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible");
}

QList<Nepomuk2::Agent> Nepomuk2::Resource::maintainedBys() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Agent> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Agent( *it ) );
    return rl;
}

void Nepomuk2::Resource::setMaintainedBys( const QList<Nepomuk2::Agent>& value )
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
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy"), Variant( l ) );
}

void Nepomuk2::Resource::addMaintainedBy( const Nepomuk2::Agent& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy"), v );
}

QUrl Nepomuk2::Resource::maintainedByUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy");
}


