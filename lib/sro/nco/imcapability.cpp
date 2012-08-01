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
#include "imcapability.h"

#include "imaccount.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::IMCapability::IMCapability()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#IMCapability") )
{
}



Nepomuk2::IMCapability::IMCapability( const IMCapability& res )
  : Resource( res )
{
}


Nepomuk2::IMCapability::IMCapability( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::IMCapability::IMCapability( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#IMCapability") )
{
}

Nepomuk2::IMCapability::IMCapability( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#IMCapability") )
{
}

Nepomuk2::IMCapability::IMCapability( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::IMCapability::IMCapability( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::IMCapability::~IMCapability()
{
}

Nepomuk2::IMCapability& Nepomuk2::IMCapability::operator=( const IMCapability& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::IMCapability::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#IMCapability");
}


