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
#include "dictionary.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Dictionary::Dictionary()
  : Book( QUrl(), QUrl::fromEncoded("http://www.example.com/nbib#Dictionary") )
{
}



Nepomuk2::Dictionary::Dictionary( const Dictionary& res )
  : Book( res )
{
}


Nepomuk2::Dictionary::Dictionary( const Nepomuk2::Resource& res )
  : Book( res )
{
}


Nepomuk2::Dictionary::Dictionary( const QString& uri )
  : Book( uri, QUrl::fromEncoded("http://www.example.com/nbib#Dictionary") )
{
}

Nepomuk2::Dictionary::Dictionary( const QUrl& uri )
  : Book( uri, QUrl::fromEncoded("http://www.example.com/nbib#Dictionary") )
{
}

Nepomuk2::Dictionary::Dictionary( const QString& uri, const QUrl& type )
  : Book( uri, type )
{
}


Nepomuk2::Dictionary::Dictionary( const QUrl& uri, const QUrl& type )
  : Book( uri, type )
{
}

Nepomuk2::Dictionary::~Dictionary()
{
}

Nepomuk2::Dictionary& Nepomuk2::Dictionary::operator=( const Dictionary& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Dictionary::resourceTypeUri()
{
    return QLatin1String("http://www.example.com/nbib#Dictionary");
}


