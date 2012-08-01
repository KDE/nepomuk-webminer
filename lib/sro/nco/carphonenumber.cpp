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
#include "carphonenumber.h"



#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::CarPhoneNumber::CarPhoneNumber()
  : VoicePhoneNumber( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CarPhoneNumber") )
{
}



Nepomuk2::CarPhoneNumber::CarPhoneNumber( const CarPhoneNumber& res )
  : VoicePhoneNumber( res )
{
}


Nepomuk2::CarPhoneNumber::CarPhoneNumber( const Nepomuk2::Resource& res )
  : VoicePhoneNumber( res )
{
}


Nepomuk2::CarPhoneNumber::CarPhoneNumber( const QString& uri )
  : VoicePhoneNumber( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CarPhoneNumber") )
{
}

Nepomuk2::CarPhoneNumber::CarPhoneNumber( const QUrl& uri )
  : VoicePhoneNumber( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CarPhoneNumber") )
{
}

Nepomuk2::CarPhoneNumber::CarPhoneNumber( const QString& uri, const QUrl& type )
  : VoicePhoneNumber( uri, type )
{
}


Nepomuk2::CarPhoneNumber::CarPhoneNumber( const QUrl& uri, const QUrl& type )
  : VoicePhoneNumber( uri, type )
{
}

Nepomuk2::CarPhoneNumber::~CarPhoneNumber()
{
}

Nepomuk2::CarPhoneNumber& Nepomuk2::CarPhoneNumber::operator=( const CarPhoneNumber& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::CarPhoneNumber::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CarPhoneNumber");
}


