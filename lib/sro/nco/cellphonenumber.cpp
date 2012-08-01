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
#include "cellphonenumber.h"

#include "messagingnumber.h"
#include "voicephonenumber.h"

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>


Nepomuk2::CellPhoneNumber::CellPhoneNumber()
  : VoicePhoneNumber( QUrl(), QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CellPhoneNumber") )
{
}



Nepomuk2::CellPhoneNumber::CellPhoneNumber( const CellPhoneNumber& res )
  : VoicePhoneNumber( res )
{
}


Nepomuk2::CellPhoneNumber::CellPhoneNumber( const Nepomuk2::Resource& res )
  : VoicePhoneNumber( res )
{
}


Nepomuk2::CellPhoneNumber::CellPhoneNumber( const QString& uri )
  : VoicePhoneNumber( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CellPhoneNumber") )
{
}

Nepomuk2::CellPhoneNumber::CellPhoneNumber( const QUrl& uri )
  : VoicePhoneNumber( uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CellPhoneNumber") )
{
}

Nepomuk2::CellPhoneNumber::CellPhoneNumber( const QString& uri, const QUrl& type )
  : VoicePhoneNumber( uri, type )
{
}


Nepomuk2::CellPhoneNumber::CellPhoneNumber( const QUrl& uri, const QUrl& type )
  : VoicePhoneNumber( uri, type )
{
}

Nepomuk2::CellPhoneNumber::~CellPhoneNumber()
{
}

Nepomuk2::CellPhoneNumber& Nepomuk2::CellPhoneNumber::operator=( const CellPhoneNumber& res )
{
    Resource::operator=( res );
    return *this;
}

QString Nepomuk2::CellPhoneNumber::resourceTypeUri()
{
    return QLatin1String("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#CellPhoneNumber");
}

Nepomuk2::MessagingNumber Nepomuk2::CellPhoneNumber::toMessagingNumber() const
{
    return Nepomuk2::MessagingNumber( *this );
}

Nepomuk2::VoicePhoneNumber Nepomuk2::CellPhoneNumber::toVoicePhoneNumber() const
{
    return Nepomuk2::VoicePhoneNumber( *this );
}


