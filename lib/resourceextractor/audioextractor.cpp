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

#include "audioextractor.h"

#include "../metadataparameters.h"

#include <taglib/taglib.h>
#include <taglib/tstring.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include <QtCore/QString>

#include <KDE/KDebug>

namespace NepomukMetaDataExtractor
{
namespace Extractor
{
class AudioExtractorPrivate
{
    // nothing yet, but might be needed in the future
};
}
}

NepomukMetaDataExtractor::Extractor::AudioExtractor::AudioExtractor(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukMetaDataExtractor::Extractor::AudioExtractorPrivate)
{
}

void NepomukMetaDataExtractor::Extractor::AudioExtractor::parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl)
{
    mdp->setResourceUri(fileUrl);
    mdp->setResourceType(QLatin1String("music"));

    if (!findByTag(mdp)) {
        findByFileName(mdp);
    }
}

bool NepomukMetaDataExtractor::Extractor::AudioExtractor::findByTag(MetaDataParameters *mdp)
{
    TagLib::FileRef f(mdp->resourceUri().toLocalFile().toUtf8().data(), false);

    if (f.isNull()) {
        kDebug() << "TagLib couldn't create FileRef resource for" << mdp->resourceUri().toLocalFile();
        return false;
    }

    // fill the search parameters with the found data
    TagLib::String title = f.tag()->title();
    mdp->setSearchTitle(QString::fromUtf8(title.toCString(true)));

    TagLib::String artist = f.tag()->artist();
    mdp->setSearchPerson(QString::fromUtf8(artist.toCString(true)));

    TagLib::String album = f.tag()->album();
    mdp->setSearchAlbum(QString::fromUtf8(album.toCString(true)));

    //TagLib::String genre = g.tag()->genre();
    uint year = f.tag()->year();
    mdp->setSearchYearMax(QString("%1").arg(year));
    mdp->setSearchYearMin(QString("%1").arg(year));

    uint track = f.tag()->track();
    mdp->setSearchTrack(QString("%1").arg(track));
    if (mdp->searchTrack() == QString("0")) {
        mdp->setSearchTrack(QString());
    }

    if (mdp->searchTitle().isEmpty() && mdp->searchPerson().isEmpty() && mdp->searchAlbum().isEmpty() && mdp->searchTrack().isEmpty()) {
        kDebug() << "TagLib couldn't find any information on" << mdp->resourceUri().toLocalFile();
        return false;
    } else {
        return true;
    }
}

bool NepomukMetaDataExtractor::Extractor::AudioExtractor::findByFileName(MetaDataParameters *mdp)
{
    return false;
}
