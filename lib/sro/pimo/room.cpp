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
#include "room.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Room::Room()
  : Location( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Room") )
{
}



Nepomuk2::Room::Room( const Room& res )
  : Location( res )
{
}


Nepomuk2::Room::Room( const Nepomuk2::Resource& res )
  : Location( res )
{
}


Nepomuk2::Room::Room( const QString& uri )
  : Location( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Room") )
{
}

Nepomuk2::Room::Room( const QUrl& uri )
  : Location( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Room") )
{
}

Nepomuk2::Room::Room( const QString& uri, const QUrl& type )
  : Location( uri, type )
{
}


Nepomuk2::Room::Room( const QUrl& uri, const QUrl& type )
  : Location( uri, type )
{
}

Nepomuk2::Room::~Room()
{
}

Nepomuk2::Room& Nepomuk2::Room::operator=( const Room& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Room::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Room");
}


