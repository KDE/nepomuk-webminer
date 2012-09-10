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

#ifndef _NBIB_ARTICLE_H_
#define _NBIB_ARTICLE_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

#include "publication.h"

namespace Nepomuk2 {
namespace NBIB {
/**
 * An article from a journal or magazine. 
 */
class Article : public virtual NBIB::Publication
{
public:
    Article(const QUrl& uri = QUrl())
      : SimpleResource(uri), NIE::InformationElement(uri, QUrl::fromEncoded("http://www.example.com/nbib#Article", QUrl::StrictMode)), NBIB::Publication(uri, QUrl::fromEncoded("http://www.example.com/nbib#Article", QUrl::StrictMode)) {
    }

    Article(const SimpleResource& res)
      : SimpleResource(res), NIE::InformationElement(res, QUrl::fromEncoded("http://www.example.com/nbib#Article", QUrl::StrictMode)), NBIB::Publication(res, QUrl::fromEncoded("http://www.example.com/nbib#Article", QUrl::StrictMode)) {
    }

    Article& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.example.com/nbib#Article", QUrl::StrictMode));
        return *this;
    }

    /**
     * Get property http://www.example.com/nbib#collection. 
     * The collection this article was published in (for example a 
     * Journalissue, Newspaper or Proceedings 
     */
    QUrl collection() const {
        QUrl value;
        if(contains(QUrl::fromEncoded("http://www.example.com/nbib#collection", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.example.com/nbib#collection", QUrl::StrictMode)).first().value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.example.com/nbib#collection. 
     * The collection this article was published in (for example a 
     * Journalissue, Newspaper or Proceedings 
     */
    void setCollection(const QUrl& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.example.com/nbib#collection", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.example.com/nbib#collection. 
     * The collection this article was published in (for example a 
     * Journalissue, Newspaper or Proceedings 
     */
    void addCollection(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.example.com/nbib#collection", QUrl::StrictMode), value);
    }

protected:
    Article(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NIE::InformationElement(uri, type), NBIB::Publication(uri, type) {
    }
    Article(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NIE::InformationElement(res, type), NBIB::Publication(res, type) {
    }
};
}
}

#endif
