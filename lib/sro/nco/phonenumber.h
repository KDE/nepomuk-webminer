/*
 * This file has been generated by the onto2vocabularyclass tool
 * copyright (C) 2007-2010 Sebastian Trueg <trueg@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _NCO_PHONENUMBER_H_
#define _NCO_PHONENUMBER_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

#include "nco/contactmedium.h"

namespace Nepomuk2 {
namespace NCO {
/**
 * A telephone number. 
 */
class PhoneNumber : public virtual NCO::ContactMedium
{
public:
    PhoneNumber(const QUrl& uri = QUrl())
      : SimpleResource(uri), NCO::ContactMedium(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PhoneNumber", QUrl::StrictMode)) {
    }

    PhoneNumber(const SimpleResource& res)
      : SimpleResource(res), NCO::ContactMedium(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PhoneNumber", QUrl::StrictMode)) {
    }

    PhoneNumber& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PhoneNumber", QUrl::StrictMode));
        return *this;
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber. 
     */
    QString phoneNumber() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber. 
     */
    void setPhoneNumber(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber. 
     */
    void addPhoneNumber(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#phoneNumber", QUrl::StrictMode), value);
    }

protected:
    PhoneNumber(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NCO::ContactMedium(uri, type) {
    }
    PhoneNumber(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NCO::ContactMedium(res, type) {
    }
};
}
}

#endif
