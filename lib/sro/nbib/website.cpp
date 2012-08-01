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
#include "website.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Website::Website()
  : Collection( QUrl(), QUrl::fromEncoded("http://www.example.com/nbib#Website") )
{
}



Nepomuk2::Website::Website( const Website& res )
  : Collection( res )
{
}


Nepomuk2::Website::Website( const Nepomuk2::Resource& res )
  : Collection( res )
{
}


Nepomuk2::Website::Website( const QString& uri )
  : Collection( uri, QUrl::fromEncoded("http://www.example.com/nbib#Website") )
{
}

Nepomuk2::Website::Website( const QUrl& uri )
  : Collection( uri, QUrl::fromEncoded("http://www.example.com/nbib#Website") )
{
}

Nepomuk2::Website::Website( const QString& uri, const QUrl& type )
  : Collection( uri, type )
{
}


Nepomuk2::Website::Website( const QUrl& uri, const QUrl& type )
  : Collection( uri, type )
{
}

Nepomuk2::Website::~Website()
{
}

Nepomuk2::Website& Nepomuk2::Website::operator=( const Website& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Website::resourceTypeUri()
{
    return QLatin1String("http://www.example.com/nbib#Website");
}


