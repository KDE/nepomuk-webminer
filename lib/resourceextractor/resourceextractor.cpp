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

ResourceExtractor::ResourceExtractor(QObject *parent)
    : QObject(parent)
{
}

void ResourceExtractor::setForceUpdate(bool update)
{
    m_forceUpdate = update;
}


void ResourceExtractor::lookupFiles(const KUrl &fileOrFolder)
{
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
        addFilesToList(fileOrFolder.toLocalFile());
    }

    emit progressStatus( i18n("Found:") );

    int pubSize = m_resourcesToLookup.value(QLatin1String("publication")).size();
    if( pubSize != 0)
        emit progressStatus( i18n("%1 files that miss publication data", pubSize) );

    int tvshowSize = m_resourcesToLookup.value(QLatin1String("tvshow")).size();
    if( tvshowSize != 0)
        emit progressStatus( i18n("%1 files that miss tvshow data", tvshowSize) );

    int movieSize = m_resourcesToLookup.value(QLatin1String("movie")).size();
    if( movieSize != 0)
        emit progressStatus( i18n("%1 files that miss movie data", movieSize) );

    if( pubSize == 0 && tvshowSize == 0 && movieSize == 0) {
        emit progressStatus( i18n("No files for meta data fetching found") );
    }
    else {
        emit resourceExtarctionDone();
    }
}

void ResourceExtractor::lookupResource(const Nepomuk::Resource &resource)
{

}

void ResourceExtractor::lookupResource(const QList<Nepomuk::Resource> &resources)
{

}

QStringList ResourceExtractor::availableResourceTypes()
{
    QStringList availableTypes;
    foreach(const QString &k, m_resourcesToLookup.keys() ) {
        availableTypes << k;
    }

    return availableTypes;
}

QList<MetaDataParameters *> ResourceExtractor::resourcesToFetch(const QString &type)
{
    QList<MetaDataParameters *> mdpList = m_resourcesToLookup.value(type);

    return mdpList;
}

void ResourceExtractor::addFilesToList(const KUrl &fileUrl)
{
    Nepomuk::File fileResource(fileUrl);

    if( !m_forceUpdate && (
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
        videoExtractor.parseUrl( metaDataParameters, fileUrl );
    }
    else {
        kDebug() << "unsupportet mimetype" << kmt.data()->name();
    }

    QList<MetaDataParameters*> currentFileList = m_resourcesToLookup.value( metaDataParameters->resourceType );
    currentFileList.append( metaDataParameters );
    m_resourcesToLookup.insert( metaDataParameters->resourceType, currentFileList);
}
