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
#include "application.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::Application::Application()
  : Software( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Application") )
{
}



Nepomuk2::Application::Application( const Application& res )
  : Software( res )
{
}


Nepomuk2::Application::Application( const Nepomuk2::Resource& res )
  : Software( res )
{
}


Nepomuk2::Application::Application( const QString& uri )
  : Software( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Application") )
{
}

Nepomuk2::Application::Application( const QUrl& uri )
  : Software( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Application") )
{
}

Nepomuk2::Application::Application( const QString& uri, const QUrl& type )
  : Software( uri, type )
{
}


Nepomuk2::Application::Application( const QUrl& uri, const QUrl& type )
  : Software( uri, type )
{
}

Nepomuk2::Application::~Application()
{
}

Nepomuk2::Application& Nepomuk2::Application::operator=( const Application& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::Application::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Application");
}


