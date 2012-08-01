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
#include "datasource.h"

#include "dataobject.h"
#include "informationelement.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::DataSource::DataSource()
  : Resource( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#DataSource") )
{
}



Nepomuk2::DataSource::DataSource( const DataSource& res )
  : Resource( res )
{
}


Nepomuk2::DataSource::DataSource( const Nepomuk2::Resource& res )
  : Resource( res )
{
}


Nepomuk2::DataSource::DataSource( const QString& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#DataSource") )
{
}

Nepomuk2::DataSource::DataSource( const QUrl& uri )
  : Resource( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#DataSource") )
{
}

Nepomuk2::DataSource::DataSource( const QString& uri, const QUrl& type )
  : Resource( uri, type )
{
}


Nepomuk2::DataSource::DataSource( const QUrl& uri, const QUrl& type )
  : Resource( uri, type )
{
}

Nepomuk2::DataSource::~DataSource()
{
}

Nepomuk2::DataSource& Nepomuk2::DataSource::operator=( const DataSource& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::DataSource::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#DataSource");
}


