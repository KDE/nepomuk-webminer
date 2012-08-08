/*
   Copyright (C) 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef METADATAEXTRACTORSERVICE_H
#define METADATAEXTRACTORSERVICE_H

#include <Nepomuk2/Service>
#include <Nepomuk2/Resource>

#include <QtCore/QList>
#include <QtCore/QUrl>

namespace Nepomuk2 {
    class ResourceWatcher;
}

/**
 * @brief This service calls the metadata extarctor everytime a new @c nfo:Video or @c nfo::PaginatedTextDocument resource was created
 *
 * Extracts the metadata from the web based on the AutomaticFetcher.
 */
class MetaDataExtractorService : public Nepomuk2::Service
{
    Q_OBJECT

public:
    MetaDataExtractorService(QObject *parent = 0, const QVariantList& args = QVariantList());
    ~MetaDataExtractorService();

private Q_SLOTS:
    /**
     * @brief Fetch tv show or movie meta data if the resource is an existing file and does not already have TvShow or Movie meta data
     *
     * @param res the file resource that will be checked
     * @param types list of types for this resource
     */
    void slotVideoResourceCreated(const Nepomuk2::Resource& res, const QList<QUrl>& types);
    /**
     * @brief Fetch publication data if the resource is an existing file and does not already have a @c nbib:Publication connected
     *
     * @param res the file resource that will be checked
     * @param types list of types for this resource
     */
    void slotDocumentResourceCreated(const Nepomuk2::Resource& res, const QList<QUrl>& types);

private:
    Nepomuk2::ResourceWatcher* m_videoWatcher;
    Nepomuk2::ResourceWatcher* m_documentWatcher;
};

#endif // METADATAEXTRACTORSERVICE_H
