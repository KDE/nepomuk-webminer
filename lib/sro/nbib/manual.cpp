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
#include "manual.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Manual::Manual()
  : Publication( QUrl(), QUrl::fromEncoded("http://www.example.com/nbib#Manual") )
{
}



Nepomuk2::Manual::Manual( const Manual& res )
  : Publication( res )
{
}


Nepomuk2::Manual::Manual( const Nepomuk2::Resource& res )
  : Publication( res )
{
}


Nepomuk2::Manual::Manual( const QString& uri )
  : Publication( uri, QUrl::fromEncoded("http://www.example.com/nbib#Manual") )
{
}

Nepomuk2::Manual::Manual( const QUrl& uri )
  : Publication( uri, QUrl::fromEncoded("http://www.example.com/nbib#Manual") )
{
}

Nepomuk2::Manual::Manual( const QString& uri, const QUrl& type )
  : Publication( uri, type )
{
}


Nepomuk2::Manual::Manual( const QUrl& uri, const QUrl& type )
  : Publication( uri, type )
{
}

Nepomuk2::Manual::~Manual()
{
}

Nepomuk2::Manual& Nepomuk2::Manual::operator=( const Manual& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Manual::resourceTypeUri()
{
    return QLatin1String("http://www.example.com/nbib#Manual");
}


