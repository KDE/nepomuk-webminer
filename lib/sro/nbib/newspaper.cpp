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
#include "newspaper.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Newspaper::Newspaper()
  : Series( QUrl(), QUrl::fromEncoded("http://www.example.com/nbib#Newspaper") )
{
}



Nepomuk2::Newspaper::Newspaper( const Newspaper& res )
  : Series( res )
{
}


Nepomuk2::Newspaper::Newspaper( const Nepomuk2::Resource& res )
  : Series( res )
{
}


Nepomuk2::Newspaper::Newspaper( const QString& uri )
  : Series( uri, QUrl::fromEncoded("http://www.example.com/nbib#Newspaper") )
{
}

Nepomuk2::Newspaper::Newspaper( const QUrl& uri )
  : Series( uri, QUrl::fromEncoded("http://www.example.com/nbib#Newspaper") )
{
}

Nepomuk2::Newspaper::Newspaper( const QString& uri, const QUrl& type )
  : Series( uri, type )
{
}


Nepomuk2::Newspaper::Newspaper( const QUrl& uri, const QUrl& type )
  : Series( uri, type )
{
}

Nepomuk2::Newspaper::~Newspaper()
{
}

Nepomuk2::Newspaper& Nepomuk2::Newspaper::operator=( const Newspaper& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Newspaper::resourceTypeUri()
{
    return QLatin1String("http://www.example.com/nbib#Newspaper");
}


