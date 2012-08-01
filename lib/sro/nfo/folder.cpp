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
#include "folder.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Folder::Folder()
  : DataContainer( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder") )
{
}



Nepomuk2::Folder::Folder( const Folder& res )
  : DataContainer( res )
{
}


Nepomuk2::Folder::Folder( const Nepomuk2::Resource& res )
  : DataContainer( res )
{
}


Nepomuk2::Folder::Folder( const QString& uri )
  : DataContainer( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder") )
{
}

Nepomuk2::Folder::Folder( const QUrl& uri )
  : DataContainer( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder") )
{
}

Nepomuk2::Folder::Folder( const QString& uri, const QUrl& type )
  : DataContainer( uri, type )
{
}


Nepomuk2::Folder::Folder( const QUrl& uri, const QUrl& type )
  : DataContainer( uri, type )
{
}

Nepomuk2::Folder::~Folder()
{
}

Nepomuk2::Folder& Nepomuk2::Folder::operator=( const Folder& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Folder::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder");
}


