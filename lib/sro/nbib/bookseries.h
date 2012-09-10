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

#ifndef _NBIB_BOOKSERIES_H_
#define _NBIB_BOOKSERIES_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

#include "nbib/series.h"

namespace Nepomuk2 {
namespace NBIB {
/**
 * The series of a book. 
 */
class BookSeries : public virtual NBIB::Series
{
public:
    BookSeries(const QUrl& uri = QUrl())
      : SimpleResource(uri), NIE::InformationElement(uri, QUrl::fromEncoded("http://www.example.com/nbib#BookSeries", QUrl::StrictMode)), NBIB::Series(uri, QUrl::fromEncoded("http://www.example.com/nbib#BookSeries", QUrl::StrictMode)) {
    }

    BookSeries(const SimpleResource& res)
      : SimpleResource(res), NIE::InformationElement(res, QUrl::fromEncoded("http://www.example.com/nbib#BookSeries", QUrl::StrictMode)), NBIB::Series(res, QUrl::fromEncoded("http://www.example.com/nbib#BookSeries", QUrl::StrictMode)) {
    }

    BookSeries& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.example.com/nbib#BookSeries", QUrl::StrictMode));
        return *this;
    }

protected:
    BookSeries(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NIE::InformationElement(uri, type), NBIB::Series(uri, type) {
    }
    BookSeries(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NIE::InformationElement(res, type), NBIB::Series(res, type) {
    }
};
}
}

#endif
