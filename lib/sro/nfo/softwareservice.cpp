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
#include "softwareservice.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::SoftwareService::SoftwareService()
  : DataObject( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#SoftwareService") )
{
}



Nepomuk2::SoftwareService::SoftwareService( const SoftwareService& res )
  : DataObject( res )
{
}


Nepomuk2::SoftwareService::SoftwareService( const Nepomuk2::Resource& res )
  : DataObject( res )
{
}


Nepomuk2::SoftwareService::SoftwareService( const QString& uri )
  : DataObject( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#SoftwareService") )
{
}

Nepomuk2::SoftwareService::SoftwareService( const QUrl& uri )
  : DataObject( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#SoftwareService") )
{
}

Nepomuk2::SoftwareService::SoftwareService( const QString& uri, const QUrl& type )
  : DataObject( uri, type )
{
}


Nepomuk2::SoftwareService::SoftwareService( const QUrl& uri, const QUrl& type )
  : DataObject( uri, type )
{
}

Nepomuk2::SoftwareService::~SoftwareService()
{
}

Nepomuk2::SoftwareService& Nepomuk2::SoftwareService::operator=( const SoftwareService& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::SoftwareService::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#SoftwareService");
}


