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

#ifndef _NAO_FREEDESKTOPICON_H_
#define _NAO_FREEDESKTOPICON_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

#include "nao/symbol.h"

namespace Nepomuk2 {
namespace NAO {
/**
 * Represents a desktop icon as defined in the FreeDesktop Icon 
 * Naming Standard (http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html). 
 */
class FreeDesktopIcon : public virtual NAO::Symbol
{
public:
    FreeDesktopIcon(const QUrl& uri = QUrl())
      : SimpleResource(uri), NAO::Symbol(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#FreeDesktopIcon", QUrl::StrictMode)) {
    }

    FreeDesktopIcon(const SimpleResource& res)
      : SimpleResource(res), NAO::Symbol(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#FreeDesktopIcon", QUrl::StrictMode)) {
    }

    FreeDesktopIcon& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#FreeDesktopIcon", QUrl::StrictMode));
        return *this;
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#iconName. 
     * Defines a name for a FreeDesktop Icon as defined in the FreeDesktop 
     * Icon Naming Standard 
     */
    QStringList iconNames() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#iconName", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#iconName. 
     * Defines a name for a FreeDesktop Icon as defined in the FreeDesktop 
     * Icon Naming Standard 
     */
    void setIconNames(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#iconName", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#iconName. 
     * Defines a name for a FreeDesktop Icon as defined in the FreeDesktop 
     * Icon Naming Standard 
     */
    void addIconName(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#iconName", QUrl::StrictMode), value);
    }

protected:
    FreeDesktopIcon(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NAO::Symbol(uri, type) {
    }
    FreeDesktopIcon(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NAO::Symbol(res, type) {
    }
};
}
}

#endif
