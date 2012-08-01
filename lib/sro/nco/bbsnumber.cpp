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
#include "bbsnumber.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::BbsNumber::BbsNumber()
  : ModemNumber( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#BbsNumber") )
{
}



Nepomuk2::BbsNumber::BbsNumber( const BbsNumber& res )
  : ModemNumber( res )
{
}


Nepomuk2::BbsNumber::BbsNumber( const Nepomuk2::Resource& res )
  : ModemNumber( res )
{
}


Nepomuk2::BbsNumber::BbsNumber( const QString& uri )
  : ModemNumber( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#BbsNumber") )
{
}

Nepomuk2::BbsNumber::BbsNumber( const QUrl& uri )
  : ModemNumber( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#BbsNumber") )
{
}

Nepomuk2::BbsNumber::BbsNumber( const QString& uri, const QUrl& type )
  : ModemNumber( uri, type )
{
}


Nepomuk2::BbsNumber::BbsNumber( const QUrl& uri, const QUrl& type )
  : ModemNumber( uri, type )
{
}

Nepomuk2::BbsNumber::~BbsNumber()
{
}

Nepomuk2::BbsNumber& Nepomuk2::BbsNumber::operator=( const BbsNumber& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::BbsNumber::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#BbsNumber");
}


