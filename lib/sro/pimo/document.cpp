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
#include "document.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Document::Document()
  : LogicalMediaType( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Document") )
{
}



Nepomuk2::Document::Document( const Document& res )
  : LogicalMediaType( res )
{
}


Nepomuk2::Document::Document( const Nepomuk2::Resource& res )
  : LogicalMediaType( res )
{
}


Nepomuk2::Document::Document( const QString& uri )
  : LogicalMediaType( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Document") )
{
}

Nepomuk2::Document::Document( const QUrl& uri )
  : LogicalMediaType( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Document") )
{
}

Nepomuk2::Document::Document( const QString& uri, const QUrl& type )
  : LogicalMediaType( uri, type )
{
}


Nepomuk2::Document::Document( const QUrl& uri, const QUrl& type )
  : LogicalMediaType( uri, type )
{
}

Nepomuk2::Document::~Document()
{
}

Nepomuk2::Document& Nepomuk2::Document::operator=( const Document& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Document::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Document");
}


