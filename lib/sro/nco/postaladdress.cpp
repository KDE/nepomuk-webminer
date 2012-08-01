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
#include "postaladdress.h"

#include "role.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::PostalAddress::PostalAddress()
  : ContactMedium( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PostalAddress") )
{
}



Nepomuk2::PostalAddress::PostalAddress( const PostalAddress& res )
  : ContactMedium( res )
{
}


Nepomuk2::PostalAddress::PostalAddress( const Nepomuk2::Resource& res )
  : ContactMedium( res )
{
}


Nepomuk2::PostalAddress::PostalAddress( const QString& uri )
  : ContactMedium( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PostalAddress") )
{
}

Nepomuk2::PostalAddress::PostalAddress( const QUrl& uri )
  : ContactMedium( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PostalAddress") )
{
}

Nepomuk2::PostalAddress::PostalAddress( const QString& uri, const QUrl& type )
  : ContactMedium( uri, type )
{
}


Nepomuk2::PostalAddress::PostalAddress( const QUrl& uri, const QUrl& type )
  : ContactMedium( uri, type )
{
}

Nepomuk2::PostalAddress::~PostalAddress()
{
}

Nepomuk2::PostalAddress& Nepomuk2::PostalAddress::operator=( const PostalAddress& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::PostalAddress::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PostalAddress");
}

QStringList Nepomuk2::PostalAddress::regions() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region") ).toStringList());
}

void Nepomuk2::PostalAddress::setRegions( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addRegion( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region"), v );
}

QString Nepomuk2::PostalAddress::region() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setRegion( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::regionUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#region");
}

QStringList Nepomuk2::PostalAddress::countrys() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country") ).toStringList());
}

void Nepomuk2::PostalAddress::setCountrys( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addCountry( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country"), v );
}

QString Nepomuk2::PostalAddress::country() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setCountry( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::countryUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#country");
}

QStringList Nepomuk2::PostalAddress::extendedAddresses() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress") ).toStringList());
}

void Nepomuk2::PostalAddress::setExtendedAddresses( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addExtendedAddress( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress"), v );
}

QString Nepomuk2::PostalAddress::extendedAddress() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setExtendedAddress( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::extendedAddressUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#extendedAddress");
}

QList<Nepomuk2::Resource> Nepomuk2::PostalAddress::addressLocations() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    QList<Resource> rl;
    QList<Resource> l = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation") ).toResourceList() ;
    for( QList<Resource>::const_iterator it = l.constBegin();
        it != l.constEnd(); ++it )
    rl.append( Resource( *it ) );
    return rl;
}

void Nepomuk2::PostalAddress::setAddressLocations( const QList<Nepomuk2::Resource>& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addAddressLocation( const Nepomuk2::Resource& value )
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation") );
    v.append( Resource( value ) );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation"), v );
}

Nepomuk2::Resource Nepomuk2::PostalAddress::addressLocation() const
{
    // We always store all Resource types as plain Resource objects.
    // It does not introduce any overhead (due to the implicit sharing of
    // the data and has the advantage that we can mix setProperty calls
    // with the special Resource subclass methods.
    // More importantly Resource loads the data as Resource objects anyway.
    return Resource( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation") ).toResource().uri() );
}

void Nepomuk2::PostalAddress::setAddressLocation( const Nepomuk2::Resource& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::addressLocationUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#addressLocation");
}

QStringList Nepomuk2::PostalAddress::streetAddresses() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress") ).toStringList());
}

void Nepomuk2::PostalAddress::setStreetAddresses( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addStreetAddress( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress"), v );
}

QString Nepomuk2::PostalAddress::streetAddress() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setStreetAddress( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::streetAddressUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#streetAddress");
}

QStringList Nepomuk2::PostalAddress::postalcodes() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode") ).toStringList());
}

void Nepomuk2::PostalAddress::setPostalcodes( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addPostalcode( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode"), v );
}

QString Nepomuk2::PostalAddress::postalcode() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setPostalcode( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::postalcodeUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#postalcode");
}

QStringList Nepomuk2::PostalAddress::localitys() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality") ).toStringList());
}

void Nepomuk2::PostalAddress::setLocalitys( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addLocality( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality"), v );
}

QString Nepomuk2::PostalAddress::locality() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setLocality( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::localityUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#locality");
}

QStringList Nepomuk2::PostalAddress::poboxs() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox") ).toStringList());
}

void Nepomuk2::PostalAddress::setPoboxs( const QStringList& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox"), Variant( value ) );
}

void Nepomuk2::PostalAddress::addPobox( const QString& value )
{
    Variant v = property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox") );
    v.append( value );
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox"), v );
}

QString Nepomuk2::PostalAddress::pobox() const
{
    return ( property( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox") ).toStringList() << QString() ).first();
}

void Nepomuk2::PostalAddress::setPobox( const QString& value )
{
    setProperty( QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox"), Variant( value ) );
}

QUrl Nepomuk2::PostalAddress::poboxUri()
{
    return QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#pobox");
}


