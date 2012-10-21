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

#include "resourceextractor.h"

#include "metadataparameters.h"

#include "popplerextractor.h"
#include "odfextractor.h"
#include "videoextractor.h"
#include "audioextractor.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>
#include <Nepomuk2/File>
#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NIE>
#include "sro/nbib.h"

#include "sro/nbib/publication.h"
#include "sro/nbib/article.h"
#include "sro/nco/personcontact.h"
#include "sro/nmm/movie.h"

#include <KDE/KMimeType>
#include <KDE/KDebug>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFileInfoList>

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class ResourceExtractorPrivate {
    public:
        bool forceUpdate;
        bool tvShowMode;
        bool useTvShowFolderNames;
        bool movieShowMode;
        KUrl baseCallUrl; //TODO: remove this parameter? helps in the video extractor in folder name detection
        QList<MetaDataParameters *> resourcesToLookup;
    };
}
}

NepomukMetaDataExtractor::Extractor::ResourceExtractor::ResourceExtractor(QObject *parent)
    : QObject(parent)
    , d_ptr( new NepomukMetaDataExtractor::Extractor::ResourceExtractorPrivate )
{
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::setForceUpdate(bool update)
{
    Q_D( ResourceExtractor );
    d->forceUpdate = update;
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::setTvShowMode(bool tvshowmode)
{
    Q_D( ResourceExtractor );
    d->tvShowMode = tvshowmode;
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::setTvShowNamesInFolders(bool useFolderNames)
{
    Q_D( ResourceExtractor );
    d->useTvShowFolderNames = useFolderNames;
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::setMovieMode(bool moviemode)
{
    Q_D( ResourceExtractor );
    d->movieShowMode = moviemode;
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::lookupFiles(const KUrl &fileOrFolder)
{
    Q_D( ResourceExtractor );
    if(!d->baseCallUrl.isValid()) {
        d->baseCallUrl = fileOrFolder;
    }

    emit progressStatus( i18n("Check available files") );
    QDir dir(fileOrFolder.toLocalFile());

    if(dir.exists()) {
        QFileInfoList list = dir.entryInfoList();
        foreach( const QFileInfo &fileInfo, list) {
            if( fileInfo.fileName() == QString(".") || fileInfo.fileName() == QString("..")) {
                continue;
            }
            KUrl url( fileInfo.absoluteFilePath() );
            if(fileInfo.isDir()) {
                lookupFiles( url );
            }
            else {
                addFilesToList(url);
            }
        }
    }
    else {
        addFilesToList(fileOrFolder.toLocalFile() );
    }

    int resourceSize = d->resourcesToLookup.size();

    if( resourceSize != 0) {
        emit progressStatus( i18n("Found %1 files for the metadata fetching", resourceSize) );
    }
    else {
        emit progressStatus( i18n("No files for meta data fetching found") );
    }

    emit resourceExtarctionDone();
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::lookupResource(const Nepomuk2::Resource &resource)
{
    Q_D( ResourceExtractor );

    // Step1 get the file resource of this nepomuk resource
    Nepomuk2::File fileResource;

    // BUG: the hasType() is bugged without the call to types() first. Fixed in 4.9.1
    resource.types();

    if( resource.hasType(Nepomuk2::Vocabulary::NBIB::Publication()) ) {
        // in this case the file is not double typed
        fileResource = Nepomuk2::File(fileResource.property( Nepomuk2::Vocabulary::NBIB::isPublicationOf() ).toResource());
    }
    else {
        // otherwise assume the resource is double typed
        fileResource = Nepomuk2::File(resource);
    }

    Extractor::MetaDataParameters *metaDataParameters = new Extractor::MetaDataParameters;
    metaDataParameters->resourceType = QLatin1String("publication"); // default to publication just in case we can't find anything else
    metaDataParameters->metaDataSaved = true;

    if( fileResource.url().isLocalFile() ) {
        fileChecker(metaDataParameters, fileResource.url());
    }

    resourceChecker(metaDataParameters, resource);

    d->resourcesToLookup.append( metaDataParameters );

    emit resourceExtarctionDone();
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::lookupResource(const QList<Nepomuk2::Resource> &resources)
{
    foreach(const Nepomuk2::Resource & nr, resources) {
        lookupResource(nr);
    }

    emit resourceExtarctionDone();
}

QList<NepomukMetaDataExtractor::Extractor::MetaDataParameters *> NepomukMetaDataExtractor::Extractor::ResourceExtractor::resourcesList()
{
    Q_D( ResourceExtractor );

    return d->resourcesToLookup;
}

NepomukMetaDataExtractor::Extractor::MetaDataParameters *NepomukMetaDataExtractor::Extractor::ResourceExtractor::takeNext()
{
    Q_D( ResourceExtractor );

    if(d->resourcesToLookup.isEmpty())
        return 0;
    else
        return d->resourcesToLookup.takeFirst();
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::addFilesToList(const KUrl &fileUrl)
{
    Q_D( ResourceExtractor );
    Nepomuk2::File fileResource(fileUrl);

    if( !d->forceUpdate && (
        fileResource.hasProperty(Nepomuk2::Vocabulary::NBIB::publishedAs()) ||
        fileResource.hasType(Nepomuk2::Vocabulary::NMM::TVShow()) ||
        fileResource.hasType(Nepomuk2::Vocabulary::NMM::Movie()) ||
        fileResource.hasType(Nepomuk2::Vocabulary::NMM::MusicPiece()) )) {
        kDebug() << "skip file " << fileUrl << "because it already has some meta data that would be overwritten use force update to fetch meta data anyway";
        return;
    }

    Extractor::MetaDataParameters *metaDataParameters = new Extractor::MetaDataParameters;
    metaDataParameters->metaDataSaved = true;

    bool fileSupported = fileChecker(metaDataParameters, fileUrl);

    // we skip files that are not supported (not pdf, opendocument, music or video files)
    if( !fileSupported) {
        delete metaDataParameters;
        return;
    }

    resourceChecker(metaDataParameters, fileResource);

    d->resourcesToLookup.append( metaDataParameters );
}

bool NepomukMetaDataExtractor::Extractor::ResourceExtractor::fileChecker(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl)
{
    Q_D( ResourceExtractor );
    KSharedPtr<KMimeType> kmt = KMimeType::findByUrl( fileUrl );

    if(kmt.data()->name().contains(QLatin1String("application/vnd.oasis.opendocument.text"))) {
        Extractor::OdfExtractor odfExtractor;
        odfExtractor.parseUrl( mdp, fileUrl );
    }
    else if(kmt.data()->name().contains(QLatin1String("application/pdf"))) {
        Extractor::PopplerExtractor pdfExtractor;
        pdfExtractor.parseUrl( mdp, fileUrl );
    }
    else if(kmt.data()->name().contains(QLatin1String("video/"))) {
        Extractor::VideoExtractor videoExtractor;
        videoExtractor.parseUrl( mdp, fileUrl, d->baseCallUrl );
    }
    else if(kmt.data()->name().contains(QLatin1String("audio/"))) {
        Extractor::AudioExtractor audioExtractor;
        audioExtractor.parseUrl( mdp, fileUrl );
    }
    else {
        kDebug() << "unsupportet mimetype" << kmt.data()->name();
        return false;
    }

    return true;
}

bool NepomukMetaDataExtractor::Extractor::ResourceExtractor::resourceChecker(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const Nepomuk2::Resource &resource)
{
    Nepomuk2::Resource queryResource;

    // first check if the real nepomuk resource can be reached via "publishedAs"
    queryResource = resource.property( Nepomuk2::Vocabulary::NBIB::publishedAs() ).toResource();

    if( !queryResource.exists() ) {
        // if this is not the case we do not have such a Publication resource. All other resources (TvShow/Movie etc are double typed with the file resource)
        queryResource = resource;
    }

    // BUG: the hasType() is bugged without the call to types() first. Fixed in 4.9.1
    queryResource.types();

    // Now get some values from the resource for the search parameters

    if( queryResource.hasType(Nepomuk2::Vocabulary::NBIB::Publication()) ) {
        mdp->resourceType = QLatin1String("publication");
        //Nepomuk2::NBIB::Publication publication( fileResource.uri() ); seems this only works for new resources

        QString title = queryResource.property( Nepomuk2::Vocabulary::NIE::title()).toString();
        if( !title.isEmpty()) {
            mdp->searchTitle = title;
        }
        if( !queryResource.property( Nepomuk2::Vocabulary::NCO::creator() ).toResourceList().isEmpty()) {
            Nepomuk2::Resource author = queryResource.property( Nepomuk2::Vocabulary::NCO::creator() ).toResourceList().first();
            mdp->searchPerson = author.genericLabel();
        }

        QString releaseDateString = queryResource.property( Nepomuk2::Vocabulary::NBIB::publicationDate()).toString();
        QDateTime releaseDate = QDateTime::fromString(releaseDateString);
        if( releaseDate.isValid() ) {
            mdp->searchYearMax = releaseDate.toString(QLatin1String("yyyy"));
            mdp->searchYearMin = releaseDate.toString(QLatin1String("yyyy"));
        }
    }
    else if( resource.hasType(Nepomuk2::Vocabulary::NMM::TVShow()) ) {
        mdp->resourceType = QLatin1String("tvshow");

        QString title = queryResource.property( Nepomuk2::Vocabulary::NIE::title()).toString();
        if(!title.isEmpty()) {
            mdp->searchTitle =title;
        }

        QString episodeNumber = queryResource.property( Nepomuk2::Vocabulary::NMM::episodeNumber()).toString();
        if(!episodeNumber.isEmpty()) {
            mdp->searchEpisode = episodeNumber;
        }

        Nepomuk2::Resource seasonResource = queryResource.property( Nepomuk2::Vocabulary::NMM::isPartOfSeason()).toResource();
        QString seasonNumber = seasonResource.property( Nepomuk2::Vocabulary::NMM::seasonNumber()).toString();
        if(!seasonNumber.isEmpty()) {
            mdp->searchSeason = seasonNumber;
        }

        Nepomuk2::Resource seriesResource = queryResource.property( Nepomuk2::Vocabulary::NMM::series()).toResource();
        QString seriesTitle = seriesResource.property( Nepomuk2::Vocabulary::NIE::title()).toString();
        if(!seriesTitle.isEmpty()) {
            mdp->searchShowTitle =seriesTitle;
        }

    }
    else if( resource.hasType(Nepomuk2::Vocabulary::NMM::Movie()) ) {
        mdp->resourceType = QLatin1String("movie");
        //Nepomuk2::NMM::Movie movie(queryResource.uri()); seems this only works for new resources

        kDebug() << "title" << queryResource.property( Nepomuk2::Vocabulary::NIE::title());
        QString title = queryResource.property( Nepomuk2::Vocabulary::NIE::title()).toString();
        if(!title.isEmpty()) {
            mdp->searchTitle =title;
        }

        QString releaseDateString = queryResource.property( Nepomuk2::Vocabulary::NMM::releaseDate()).toString();
        QDateTime releaseDate = QDateTime::fromString(releaseDateString);
        if( releaseDate.isValid() ) {
            mdp->searchYearMax = releaseDate.toString(QLatin1String("yyyy"));
            mdp->searchYearMin = releaseDate.toString(QLatin1String("yyyy"));
        }
    }
    else if( resource.hasType(Nepomuk2::Vocabulary::NMM::MusicPiece()) ) {
        mdp->resourceType = QLatin1String("music");

        QString title = queryResource.property( Nepomuk2::Vocabulary::NIE::title()).toString();
        if(!title.isEmpty()) {
            mdp->searchTitle =title;
        }

        QString track = queryResource.property( Nepomuk2::Vocabulary::NMM::trackNumber()).toString();
        if(!track.isEmpty()) {
            mdp->searchTrack =track;
        }

        QString performer = queryResource.property( Nepomuk2::Vocabulary::NMM::performer()).toResource().genericLabel();
        if(!performer.isEmpty()) {
            mdp->searchPerson =performer;
        }

        Nepomuk2::Resource album = queryResource.property( Nepomuk2::Vocabulary::NMM::musicAlbum()).toResource();
        QString albumName = album.property( Nepomuk2::Vocabulary::NIE::title()).toString();
        if(!albumName.isEmpty()) {
            mdp->searchAlbum =albumName;
        }
    }
    else {
        // try to get some general info
        if( mdp->searchTitle.isEmpty() && !queryResource.genericLabel().isEmpty()) {
            mdp->searchTitle = queryResource.genericLabel();
        }
    }

    return true;
}
