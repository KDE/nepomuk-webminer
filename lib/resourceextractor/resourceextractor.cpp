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

#include <KDE/Nepomuk/File>
#include <Nepomuk/Vocabulary/NMM>
#include "sro/nbib.h"

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
    KUrl baseCallUrl;
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

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::lookupResource(const Nepomuk::Resource &resource)
{

}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::lookupResource(const QList<Nepomuk::Resource> &resources)
{

}

QList<NepomukMetaDataExtractor::Extractor::MetaDataParameters *> NepomukMetaDataExtractor::Extractor::ResourceExtractor::resourcesList()
{
    Q_D( ResourceExtractor );

    return d->resourcesToLookup;
}

void NepomukMetaDataExtractor::Extractor::ResourceExtractor::addFilesToList(const KUrl &fileUrl)
{
    Q_D( ResourceExtractor );
    Nepomuk::File fileResource(fileUrl);

    if( !d->forceUpdate && (
        fileResource.hasProperty(Nepomuk::Vocabulary::NBIB::publishedAs()) ||
        fileResource.hasType(Nepomuk::Vocabulary::NMM::TVShow()) ||
        fileResource.hasType(Nepomuk::Vocabulary::NMM::Movie()))) {
        kDebug() << "skip file " << fileUrl;
        return;
    }

    KSharedPtr<KMimeType> kmt = KMimeType::findByUrl( fileUrl );

    MetaDataParameters *metaDataParameters = new MetaDataParameters;
    metaDataParameters->metaDataSaved = false;

    if(kmt.data()->name().contains(QLatin1String("application/vnd.oasis.opendocument.text"))) {
        OdfExtractor odfExtractor;
        odfExtractor.parseUrl( metaDataParameters, fileUrl );
    }
    else if(kmt.data()->name().contains(QLatin1String("application/pdf"))) {
        PopplerExtractor pdfExtractor;
        pdfExtractor.parseUrl( metaDataParameters, fileUrl );
    }

    else if(kmt.data()->name().contains(QLatin1String("video/"))) {
        VideoExtractor videoExtractor;
        videoExtractor.parseUrl( metaDataParameters, fileUrl, d->baseCallUrl );
    }
    else {
        kDebug() << "unsupportet mimetype" << kmt.data()->name();
        delete metaDataParameters;
        return;
    }

    d->resourcesToLookup.append( metaDataParameters );
}
