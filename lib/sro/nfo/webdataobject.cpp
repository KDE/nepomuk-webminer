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
#include "webdataobject.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::WebDataObject::WebDataObject()
  : DataObject( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#WebDataObject") )
{
}



Nepomuk2::WebDataObject::WebDataObject( const WebDataObject& res )
  : DataObject( res )
{
}


Nepomuk2::WebDataObject::WebDataObject( const Nepomuk2::Resource& res )
  : DataObject( res )
{
}


Nepomuk2::WebDataObject::WebDataObject( const QString& uri )
  : DataObject( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#WebDataObject") )
{
}

Nepomuk2::WebDataObject::WebDataObject( const QUrl& uri )
  : DataObject( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#WebDataObject") )
{
}

Nepomuk2::WebDataObject::WebDataObject( const QString& uri, const QUrl& type )
  : DataObject( uri, type )
{
}


Nepomuk2::WebDataObject::WebDataObject( const QUrl& uri, const QUrl& type )
  : DataObject( uri, type )
{
}

Nepomuk2::WebDataObject::~WebDataObject()
{
}

Nepomuk2::WebDataObject& Nepomuk2::WebDataObject::operator=( const WebDataObject& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::WebDataObject::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#WebDataObject");
}


