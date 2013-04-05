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

#include "resourceextractor.h"

#include "metadataparameters.h"
#include "filenameanalyzer.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>
#include <Nepomuk2/File>
#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NIE>
#include "ontology/nbib.h"

#include "ontology/nbib/publication.h"
#include "ontology/nbib/article.h"
#include "ontology/nco/personcontact.h"
#include "ontology/nmm/movie.h"

#include <KDE/KMimeType>
#include <KDE/KDebug>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFileInfoList>

namespace NepomukWebMiner
{
namespace Extractor
{
class ResourceExtractorPrivate
{
public:
    NepomukWebMiner::Extractor::FilenameAnalyzer *filenameAnalyzer;
    bool forceUpdate;
    bool cancel;
    QList<MetaDataParameters *> resourcesToLookup;
};
}
}

NepomukWebMiner::Extractor::ResourceExtractor::ResourceExtractor(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukWebMiner::Extractor::ResourceExtractorPrivate)
{
    Q_D(ResourceExtractor);
    d->cancel = false;
    d->filenameAnalyzer = new NepomukWebMiner::Extractor::FilenameAnalyzer;
}

void NepomukWebMiner::Extractor::ResourceExtractor::cancel()
{
    Q_D(ResourceExtractor);
    d->cancel = true;
}

void NepomukWebMiner::Extractor::ResourceExtractor::setForceUpdate(bool update)
{
    Q_D(ResourceExtractor);
    d->forceUpdate = update;
}

void NepomukWebMiner::Extractor::ResourceExtractor::setTvShowMode(bool tvshowmode)
{
    Q_D(ResourceExtractor);
    d->filenameAnalyzer->setTvShowMode(tvshowmode);
}

void NepomukWebMiner::Extractor::ResourceExtractor::setMovieMode(bool moviemode)
{
    Q_D(ResourceExtractor);
    d->filenameAnalyzer->setMovieMode(moviemode);
}

void NepomukWebMiner::Extractor::ResourceExtractor::lookupFiles(const KUrl &fileOrFolder, bool nested)
{
    Q_D(ResourceExtractor);

    emit progressStatus(i18n("Check available files"));
    QDir dir(fileOrFolder.toLocalFile());

    if (dir.exists()) {
        QFileInfoList list = dir.entryInfoList();
        foreach (const QFileInfo & fileInfo, list) {
            if(d->cancel) {
                break;
            }
            if (fileInfo.fileName() == QString(".") || fileInfo.fileName() == QString("..")) {
                continue;
            }
            KUrl url(fileInfo.absoluteFilePath());
            if (fileInfo.isDir()) {
                emit progressStatus(i18n("Check folder %1", url.prettyUrl()));
                lookupFiles(url, true);
            } else {
                emit progressStatus(i18n("Check file %1", url.prettyUrl()));
                addFilesToList(url);
            }
        }
    } else {
        addFilesToList(fileOrFolder.toLocalFile());
    }

    if(!nested) {
        int resourceSize = d->resourcesToLookup.size();

        if (resourceSize != 0) {
            emit progressStatus(i18n("Found %1 files for the metadata fetching", resourceSize));
        } else {
            emit progressStatus(i18n("No files for meta data fetching found"));
        }

        emit resourceExtarctionDone();
    }
}

void NepomukWebMiner::Extractor::ResourceExtractor::lookupResource(const Nepomuk2::Resource &resource)
{
    Q_D(ResourceExtractor);

    // Step1 get the file resource of this nepomuk resource
    Nepomuk2::File fileResource;

    if (resource.hasType(Nepomuk2::Vocabulary::NBIB::Publication())) {
        // in this case the file is not double typed
        fileResource = Nepomuk2::File(fileResource.property(Nepomuk2::Vocabulary::NBIB::isPublicationOf()).toResource());
    } else {
        // otherwise assume the resource is double typed
        fileResource = Nepomuk2::File(resource);
    }

    Extractor::MetaDataParameters *metaDataParameters = new Extractor::MetaDataParameters;
    metaDataParameters->setResourceType(QLatin1String("publication")); // default to publication just in case we can't find anything else
    metaDataParameters->setMetaDataSaved(true);
    metaDataParameters->setResourceUri(resource.uri());

    resourceChecker(metaDataParameters, resource);

    if (fileResource.url().isLocalFile()) {
        filenameAnalyzer(metaDataParameters, fileResource.url());
    }

    d->resourcesToLookup.append(metaDataParameters);

    emit resourceExtarctionDone();
}

void NepomukWebMiner::Extractor::ResourceExtractor::lookupResource(const QList<Nepomuk2::Resource> &resources)
{
    foreach (const Nepomuk2::Resource & nr, resources) {
        lookupResource(nr);
    }

    emit resourceExtarctionDone();
}

QList<NepomukWebMiner::Extractor::MetaDataParameters *> NepomukWebMiner::Extractor::ResourceExtractor::resourcesList()
{
    Q_D(ResourceExtractor);

    return d->resourcesToLookup;
}

NepomukWebMiner::Extractor::MetaDataParameters *NepomukWebMiner::Extractor::ResourceExtractor::takeNext()
{
    Q_D(ResourceExtractor);

    if (d->resourcesToLookup.isEmpty())
        return 0;
    else
        return d->resourcesToLookup.takeFirst();
}

void NepomukWebMiner::Extractor::ResourceExtractor::addFilesToList(const KUrl &fileUrl)
{
    Q_D(ResourceExtractor);

    // if it is a file we do not support, so skip (not pdf, opendocument, music or video files)
    if( !mimeTypeChecker(fileUrl) ) {
        return;
    }

    Nepomuk2::File fileResource(fileUrl);

    // skip file if we have already some information and do not use force update
    if (!d->forceUpdate && (
                fileResource.hasProperty(Nepomuk2::Vocabulary::NBIB::publishedAs()) ||
                fileResource.hasType(Nepomuk2::Vocabulary::NMM::TVShow()) ||
                fileResource.hasType(Nepomuk2::Vocabulary::NMM::Movie()) ||
                fileResource.hasType(Nepomuk2::Vocabulary::NMM::MusicPiece()))) {
        emit progressStatus(i18n("Skip file %1 because it already has some meta data that would be overwritten use force update to fetch meta data anyway", fileUrl.prettyUrl()));
        kDebug() << "skip file " << fileUrl << "because it already has some meta data that would be overwritten use force update to fetch meta data anyway";
        return;
    }

    Extractor::MetaDataParameters *metaDataParameters = new Extractor::MetaDataParameters;
    metaDataParameters->setMetaDataSaved(true);
    metaDataParameters->setResourceUri(fileUrl);

    resourceChecker(metaDataParameters, fileResource);

    filenameAnalyzer( metaDataParameters, fileUrl );

    if(metaDataParameters->resourceType().isEmpty()) {
        QString resourceType = selectPlugin(fileUrl);
        if(resourceType.isEmpty()) {
            kDebug() << "could not determine resource type for " << fileUrl;
            delete metaDataParameters;
            return;
        }
        else {
            metaDataParameters->setResourceType(resourceType);
        }
    }

    d->resourcesToLookup.append(metaDataParameters);
}

bool NepomukWebMiner::Extractor::ResourceExtractor::mimeTypeChecker(const KUrl &fileUrl)
{
    KSharedPtr<KMimeType> kmt = KMimeType::findByUrl(fileUrl);
    QString mimetype = kmt.data()->name();

    if ( !mimetype.contains(QLatin1String("application/vnd.oasis.opendocument.text")) &&
         !mimetype.contains(QLatin1String("application/pdf")) &&
         !mimetype.contains(QLatin1String("video/")) &&
         !mimetype.contains(QLatin1String("audio/")) ) {

        kDebug() << "unsupportet mimetype" << mimetype;
        return false;
    }

    return true;
}

QString NepomukWebMiner::Extractor::ResourceExtractor::selectPlugin(const KUrl fileUrl)
{
    KSharedPtr<KMimeType> kmt = KMimeType::findByUrl(fileUrl);
    QString mimetype = kmt.data()->name();

    if ( mimetype.contains(QLatin1String("application/vnd.oasis.opendocument.text")) ) {
        return QLatin1String("publication");
    }
    else if( mimetype.contains(QLatin1String("application/pdf")) ) {
        return QLatin1String("publication");
    }
    else if ( mimetype.contains(QLatin1String("video/")) ) {
        return QLatin1String("tvshow");
    }
    else if( mimetype.contains(QLatin1String("audio/")) ) {
        return QLatin1String("music");
    }

    return QString();
}

bool NepomukWebMiner::Extractor::ResourceExtractor::resourceChecker(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const Nepomuk2::Resource &resource)
{
    bool foundData = false;

    Nepomuk2::Resource queryResource;

    // first check if the real nepomuk resource can be reached via "publishedAs"
    queryResource = resource.property(Nepomuk2::Vocabulary::NBIB::publishedAs()).toResource();

    if (!queryResource.exists()) {
        // if this is not the case we do not have such a Publication resource. All other resources (TvShow/Movie etc are double typed with the file resource)
        queryResource = resource;
    }

    // Now get some values from the resource for the search parameters

    if (queryResource.hasType(Nepomuk2::Vocabulary::NBIB::Publication())) {
        foundData = true;
        mdp->setResourceType(QLatin1String("publication"));

        QString title = queryResource.property(Nepomuk2::Vocabulary::NIE::title()).toString();
        if (!title.isEmpty()) {
            mdp->setSearchTitle(title);
        }
        if (!queryResource.property(Nepomuk2::Vocabulary::NCO::creator()).toResourceList().isEmpty()) {
            Nepomuk2::Resource author = queryResource.property(Nepomuk2::Vocabulary::NCO::creator()).toResourceList().first();
            mdp->setSearchPerson(author.genericLabel());
        }

        QString releaseDateString = queryResource.property(Nepomuk2::Vocabulary::NBIB::publicationDate()).toString();
        QDateTime releaseDate = QDateTime::fromString(releaseDateString);
        if (releaseDate.isValid()) {
            mdp->setSearchYearMax(releaseDate.toString(QLatin1String("yyyy")));
            mdp->setSearchYearMin(releaseDate.toString(QLatin1String("yyyy")));
        }
    }
    else if (resource.hasType(Nepomuk2::Vocabulary::NMM::TVShow())) {
        foundData = true;
        mdp->setResourceType(QLatin1String("tvshow"));

        QString title = queryResource.property(Nepomuk2::Vocabulary::NIE::title()).toString();
        if (!title.isEmpty()) {
            mdp->setSearchTitle(title);
        }

        QString episodeNumber = queryResource.property(Nepomuk2::Vocabulary::NMM::episodeNumber()).toString();
        if (!episodeNumber.isEmpty()) {
            mdp->setSearchEpisode(episodeNumber);
        }

        Nepomuk2::Resource seasonResource = queryResource.property(Nepomuk2::Vocabulary::NMM::isPartOfSeason()).toResource();
        QString seasonNumber = seasonResource.property(Nepomuk2::Vocabulary::NMM::seasonNumber()).toString();
        if (!seasonNumber.isEmpty()) {
            mdp->setSearchSeason(seasonNumber);
        }

        Nepomuk2::Resource seriesResource = queryResource.property(Nepomuk2::Vocabulary::NMM::series()).toResource();
        QString seriesTitle = seriesResource.property(Nepomuk2::Vocabulary::NIE::title()).toString();
        if (!seriesTitle.isEmpty()) {
            mdp->setSearchShowTitle(seriesTitle);
        }

    }
    else if (resource.hasType(Nepomuk2::Vocabulary::NMM::Movie())) {
        foundData = true;
        mdp->setResourceType(QLatin1String("movie"));

        QString title = queryResource.property(Nepomuk2::Vocabulary::NIE::title()).toString();
        if (!title.isEmpty()) {
            mdp->setSearchTitle(title);
        }

        QString releaseDateString = queryResource.property(Nepomuk2::Vocabulary::NMM::releaseDate()).toString();
        QDateTime releaseDate = QDateTime::fromString(releaseDateString);
        if (releaseDate.isValid()) {
            mdp->setSearchYearMax(releaseDate.toString(QLatin1String("yyyy")));
            mdp->setSearchYearMin(releaseDate.toString(QLatin1String("yyyy")));
        }
    }
    else if (resource.hasType(Nepomuk2::Vocabulary::NMM::MusicPiece())) {
        foundData = true;
        mdp->setResourceType(QLatin1String("music"));

        QString title = queryResource.property(Nepomuk2::Vocabulary::NIE::title()).toString();
        if (!title.isEmpty()) {
            mdp->setSearchTitle(title);
        }

        QString track = queryResource.property(Nepomuk2::Vocabulary::NMM::trackNumber()).toString();
        if (!track.isEmpty()) {
            mdp->setSearchTrack(track);
        }

        QString performer = queryResource.property(Nepomuk2::Vocabulary::NMM::performer()).toResource().genericLabel();
        if (!performer.isEmpty()) {
            mdp->setSearchPerson(performer);
        }

        Nepomuk2::Resource album = queryResource.property(Nepomuk2::Vocabulary::NMM::musicAlbum()).toResource();
        QString albumName = album.property(Nepomuk2::Vocabulary::NIE::title()).toString();
        if (!albumName.isEmpty()) {
            mdp->setSearchAlbum(albumName);
        }
    }
//    else {
//        // try to get some general info
//        if (mdp->searchTitle().isEmpty() && !queryResource.genericLabel().isEmpty()) {
//            foundData = true;
//            mdp->setSearchTitle(queryResource.genericLabel());
//        }
//    }

    return foundData;
}

void NepomukWebMiner::Extractor::ResourceExtractor::filenameAnalyzer(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl)
{
    Q_D(ResourceExtractor);
    d->filenameAnalyzer->analyze(mdp, fileUrl);
}
