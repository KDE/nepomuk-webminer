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
#include "compressiontype.h"

#include "media.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::CompressionType::CompressionType()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#CompressionType") )
{
}



Nepomuk2::CompressionType::CompressionType( const CompressionType& res )
  : Resource( res )
{
}


Nepomuk2::CompressionType::CompressionType( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::CompressionType::CompressionType( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#CompressionType") )
{
}

Nepomuk2::CompressionType::CompressionType( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#CompressionType") )
{
}

Nepomuk2::CompressionType::CompressionType( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::CompressionType::CompressionType( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::CompressionType::~CompressionType()
{
}

Nepomuk2::CompressionType& Nepomuk2::CompressionType::operator=( const CompressionType& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::CompressionType::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#CompressionType");
}


