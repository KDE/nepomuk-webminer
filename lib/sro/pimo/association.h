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

#ifndef _PIMO_ASSOCIATION_H_
#define _PIMO_ASSOCIATION_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

#include "pimo/classorthingorpropertyorassociation.h"

namespace Nepomuk2 {
namespace PIMO {
/**
 * An association between two or more pimo-things. This is used 
 * to model n-ary relations and metadata about relations. For 
 * example, the asociation of a person being organizational member 
 * is only effectual within a period of time (after the person joined 
 * the organization and before the person left the organization). 
 * There can be multiple periods of time when associations are 
 * valid. 
 */
class Association : public virtual PIMO::ClassOrThingOrPropertyOrAssociation
{
public:
    Association(const QUrl& uri = QUrl())
      : SimpleResource(uri), PIMO::ClassOrThingOrPropertyOrAssociation(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Association", QUrl::StrictMode)) {
    }

    Association(const SimpleResource& res)
      : SimpleResource(res), PIMO::ClassOrThingOrPropertyOrAssociation(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Association", QUrl::StrictMode)) {
    }

    Association& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#Association", QUrl::StrictMode));
        return *this;
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationMember. 
     * An super-property of all roles that an entity can have in an association. 
     * Member is the generic role of a thing in an association. Association 
     * subclasses should define sub-properties of this property. 
     * Associations can have Things as 
     */
    QList<QUrl> associationMembers() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationMember", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationMember. 
     * An super-property of all roles that an entity can have in an association. 
     * Member is the generic role of a thing in an association. Association 
     * subclasses should define sub-properties of this property. 
     * Associations can have Things as 
     */
    void setAssociationMembers(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationMember", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationMember. 
     * An super-property of all roles that an entity can have in an association. 
     * Member is the generic role of a thing in an association. Association 
     * subclasses should define sub-properties of this property. 
     * Associations can have Things as 
     */
    void addAssociationMember(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationMember", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationEffectual. 
     * During which time is this association effective? If omitted, 
     * the association is always effective. Start time and end-time 
     * may be left open, an open start time indicates that the fact is 
     * unknown, an open end-time indicates that the end-date is either 
     * unknown or the association has not ended. There can be multiple 
     * effectual periods. 
     */
    QList<QUrl> associationEffectuals() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationEffectual", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationEffectual. 
     * During which time is this association effective? If omitted, 
     * the association is always effective. Start time and end-time 
     * may be left open, an open start time indicates that the fact is 
     * unknown, an open end-time indicates that the end-date is either 
     * unknown or the association has not ended. There can be multiple 
     * effectual periods. 
     */
    void setAssociationEffectuals(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationEffectual", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationEffectual. 
     * During which time is this association effective? If omitted, 
     * the association is always effective. Start time and end-time 
     * may be left open, an open start time indicates that the fact is 
     * unknown, an open end-time indicates that the end-date is either 
     * unknown or the association has not ended. There can be multiple 
     * effectual periods. 
     */
    void addAssociationEffectual(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#associationEffectual", QUrl::StrictMode), value);
    }

protected:
    Association(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), PIMO::ClassOrThingOrPropertyOrAssociation(uri, type) {
    }
    Association(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), PIMO::ClassOrThingOrPropertyOrAssociation(res, type) {
    }
};
}
}

#endif
