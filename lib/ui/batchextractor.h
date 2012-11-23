/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#ifndef BATCHEXTRACTOR_H
#define BATCHEXTRACTOR_H

#include <QtCore/QObject>

#include <QtCore/QUrl>
#include <QtCore/QVariantList>

#include "nepomukwebminer_export.h"

namespace NepomukWebMiner
{
namespace UI
{
class BatchExtractorPrivate;

/**
 * @brief The BatchExtractor will extract metadata and pipe it to nepomuk in the background
 *
 * Unlike the AutomaticFetcher this one operates on item detail urls only and does not search for
 * the right website.
 *
 * This one can be used in external programs after the initial search for an item has been done.
 * The details url with some options are handed to the BatchExtractor and will be downloaded and piped
 * to nepomuk one by one in the background without interrupting the currentt UI workflow.
 *
 * The current status of the extraction job will be emitted via some signals to give the user some
 * visual feedback whats happening in the background. Also the user can decide to cancel the job if
 * necessary.
 */
class NEPOMUKWEBMINER_EXPORT BatchExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Holds the information for one extraction job object
     */
    struct ExtractionJob {
        QUrl detailsUrl;        /**< the weburl where the metadata is extracted from */
        QVariantMap options;    /**< some additional options for the extraction plugin */
        QString name;           /**< a name that describes this job */
        QString resourceUri;    /**< optional nepomuk resource uri if the data should be added to an existing resource (file for example) */
    };

    explicit BatchExtractor(QObject *parent = 0);

    /**
     * @brief Returns the name of the current running extraction job
     *
     * @return name of the current extraction job
     */
    QString currentExtractionInfo() const;

public Q_SLOTS:
    /**
     * @brief Adds a new extraction job to the queue
     *
     * @param detailsUrl the weburl where the metadata is extracted from
     * @param options some additional options for the extraction plugin
     * @param name a name that describes this job
     * @param resourceUri optional nepomuk resource uri if the data should be added to an existing resource (file for example)
     */
    void addJob(const QUrl &detailsUrl, const QVariantMap &options,
                const QString &name, const QString &resourceUri = QString());

Q_SIGNALS:
    /**
     * @brief Emitted when the extarction is in progress and every time a new item is added to the queue
     *
     * Can be used to indicate the active extarction with the current amout of items in the queue
     *
     * @param queueSize size of all items that have to be processed
     */
    void extractionStarted(int queueSize);

    /**
     * @brief Emitted when the queue is empty and all items have been fetched
     */
    void extractionFinished();

private:
    /**
     * @brief takes first entry in the queue and starts the extarction
     */
    void extractNext();

    /**
     * @brief helper function if metadata is added to an existing nepomuk resource
     *
     * Ensures the nepomuk uri is in the right part of the metadata variantmap
     * @param resourceType resource type of the data
     * @param uri the nepomuk resource uri
     * @param metadata the metadata map
     */
    void addResourceUriToMetaData(const QString &resourceType, const QString &uri, QVariantMap &metadata);

private Q_SLOTS:
    /**
     * @brief called from the Webextractor
     * @param msg log message
     */
    void log(const QString &msg);

    /**
     * @brief called from the Webextractor
     * @param msg error message
     */
    void error(const QString &msg);

    /**
     * @brief called from the Webextractor and returns all metadata
     * @param resourceType resource type according to the plugin
     * @param entry metadata entry from the plugin
     */
    void itemResults(const QString &resourceType, const QVariantMap &entry);

    /**
     * @brief Called by the QFutureWatcher when the asycron pipe action has finished
     */
    void pipeFinished();

private:
    Q_DECLARE_PRIVATE(BatchExtractor)
    BatchExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};

}
}
#endif // BATCHEXTRACTOR_H
