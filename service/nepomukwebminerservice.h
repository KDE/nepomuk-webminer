/*
 * Copyright (C) 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NEPOMUKWEBMINERSERVICE_H
#define NEPOMUKWEBMINERSERVICE_H

#include <Nepomuk2/Service>
#include <Nepomuk2/Resource>

#include <QtCore/QUrl>
#include <QtCore/QVariantList>

class NepomukWebMinerServicePrivate;

/**
 * @brief This service calls the Nepomuk-WebMiner for all audio/video/pdf resource with kext:indexingLevel == 2
 *
 * Extracts the metadata from the web based on the AutomaticFetcher.
 */
class NepomukWebMinerService : public Nepomuk2::Service
{
    Q_OBJECT

public:
    explicit NepomukWebMinerService(QObject *parent = 0, const QVariantList& args = QVariantList());
    ~NepomukWebMinerService();

    bool isSuspended() const;
    bool isIndexing() const;

    /**
     * @brief A user readable description of the scheduler's status
     */
    QString userStatusString() const;

    /**
     * @brief The current uri being indexed. It is empty if no file is being indexed.
     *
     * The url being empty does not indicate that the indexer isn't running,
     * just that it hasn't found a file to index.
     *
     * @sa indexingStarted
     * @sa indexingStopped
     */
    QUrl currentUrl() const;

private:
    Q_DECLARE_PRIVATE(NepomukWebMinerService)
    NepomukWebMinerServicePrivate *const d_ptr; /**< d-pointer for this class */
};

#endif // NEPOMUKWEBMINERSERVICE_H
