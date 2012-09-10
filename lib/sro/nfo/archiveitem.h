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

#ifndef _NFO_ARCHIVEITEM_H_
#define _NFO_ARCHIVEITEM_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

#include "nfo/embeddedfiledataobject.h"

namespace Nepomuk2 {
namespace NFO {
/**
 * A file entity inside an archive. 
 */
class ArchiveItem : public virtual NFO::EmbeddedFileDataObject
{
public:
    ArchiveItem(const QUrl& uri = QUrl())
      : SimpleResource(uri), NIE::DataObject(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode)), NFO::FileDataObject(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode)), NFO::EmbeddedFileDataObject(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode)) {
    }

    ArchiveItem(const SimpleResource& res)
      : SimpleResource(res), NIE::DataObject(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode)), NFO::FileDataObject(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode)), NFO::EmbeddedFileDataObject(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode)) {
    }

    ArchiveItem& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#ArchiveItem", QUrl::StrictMode));
        return *this;
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected. 
     * States if a given resource is password-protected. 
     */
    bool isPasswordProtected() const {
        bool value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected", QUrl::StrictMode)).first().value<bool>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected. 
     * States if a given resource is password-protected. 
     */
    void setIsPasswordProtected(const bool& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected. 
     * States if a given resource is password-protected. 
     */
    void addIsPasswordProtected(const bool& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#isPasswordProtected", QUrl::StrictMode), value);
    }

protected:
    ArchiveItem(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NIE::DataObject(uri, type), NFO::FileDataObject(uri, type), NFO::EmbeddedFileDataObject(uri, type) {
    }
    ArchiveItem(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NIE::DataObject(res, type), NFO::FileDataObject(res, type), NFO::EmbeddedFileDataObject(res, type) {
    }
};
}
}

#endif
