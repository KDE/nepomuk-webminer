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
#include "country.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Country::Country()
  : Location( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Country") )
{
}



Nepomuk2::Country::Country( const Country& res )
  : Location( res )
{
}


Nepomuk2::Country::Country( const Nepomuk2::Resource& res )
  : Location( res )
{
}


Nepomuk2::Country::Country( const QString& uri )
  : Location( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Country") )
{
}

Nepomuk2::Country::Country( const QUrl& uri )
  : Location( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Country") )
{
}

Nepomuk2::Country::Country( const QString& uri, const QUrl& type )
  : Location( uri, type )
{
}


Nepomuk2::Country::Country( const QUrl& uri, const QUrl& type )
  : Location( uri, type )
{
}

Nepomuk2::Country::~Country()
{
}

Nepomuk2::Country& Nepomuk2::Country::operator=( const Country& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Country::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Country");
}


