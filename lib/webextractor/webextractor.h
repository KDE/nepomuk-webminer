/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WEBEXTRACTOR_H
#define WEBEXTRACTOR_H

#include "nepomukmetadataextractor_export.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

class NEPOMUKMETADATAEXTRACTOR_EXPORT WebExtractor : public QObject
{
    Q_OBJECT

public:
    struct Info {
        QString name;
        QString identifier;
        QString description;
        QString resource;
        QString urlregex;
        QString author;
        QString email;
        QString file;
        QString icon;
    };

    explicit WebExtractor(QObject *parent = 0);
    ~WebExtractor();

    virtual WebExtractor::Info info() const = 0;

public slots:
    virtual void search(const QVariantMap &parameters) = 0;
    virtual void extractItem(const QUrl &url) = 0;

signals:
    void error(const QString &errorMessage);
    void log(const QString &logMessage);
    void searchResults(const QVariantList &entries);
    void itemResults(const QVariantMap &entry);
};

#endif // WEBEXTRACTOR_H
