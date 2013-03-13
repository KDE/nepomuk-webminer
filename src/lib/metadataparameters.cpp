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

#include "metadataparameters.h"


namespace NepomukWebMiner
{
namespace Extractor
{
class MetaDataParametersPrivate
{
public:
    QString searchTitle;          /**< title of the entry to search for */
    QString searchAltTitle;       /**< alternative title of the entry to search for in case the first one does not return anything */
    QString searchYearMin;
    QString searchYearMax;

    QString searchPerson;         /**< Publication author, movie director, album preformer */

    // tv show related
    QString searchShowTitle;
    QString searchEpisode;
    QString searchSeason;

    // music related
    QString searchAlbum;
    QString searchTrack;

    // publication related
    QString searchJournal;

    QString resourceType;         /**< used to select the right set of python modules that allow retrieving data for this resoruce */
    KUrl resourceUri;             /**< the nepomuk or file url where the retrieved data will be added to */
    QVariantMap metaData;         /**< here all fetched data is pushed into @todo define proper api how the data should be saved in her (key=value list) */
    bool metaDataSaved;
};
}
}

NepomukWebMiner::Extractor::MetaDataParameters::MetaDataParameters()
    : d_ptr(new NepomukWebMiner::Extractor::MetaDataParametersPrivate)
{
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchTitle(const QString &title)
{
    Q_D(MetaDataParameters);
    d->searchTitle = title;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchTitle() const
{
    return d_ptr->searchTitle;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchAltTitle(const QString &alttitle)
{
    Q_D(MetaDataParameters);
    d->searchAltTitle = alttitle;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchAltTitle() const
{
    return d_ptr->searchAltTitle;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchYearMin(const QString &minyear)
{
    Q_D(MetaDataParameters);
    d->searchYearMin = minyear;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchYearMin() const
{
    return d_ptr->searchYearMin;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchYearMax(const QString &maxyear)
{
    Q_D(MetaDataParameters);
    d->searchYearMax = maxyear;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchYearMax() const
{
    return d_ptr->searchYearMax;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchPerson(const QString &name)
{
    Q_D(MetaDataParameters);
    d->searchPerson = name;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchPerson() const
{
    return d_ptr->searchPerson;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchShowTitle(const QString &showtitle)
{
    Q_D(MetaDataParameters);
    d->searchShowTitle = showtitle;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchShowTitle() const
{
    return d_ptr->searchShowTitle;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchEpisode(const QString &episode)
{
    Q_D(MetaDataParameters);
    d->searchEpisode = episode;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchEpisode() const
{
    return d_ptr->searchEpisode;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchSeason(const QString &season)
{
    Q_D(MetaDataParameters);
    d->searchSeason = season;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchSeason() const
{
    return d_ptr->searchSeason;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchAlbum(const QString &album)
{
    Q_D(MetaDataParameters);
    d->searchAlbum = album;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchAlbum() const
{
    return d_ptr->searchAlbum;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchTrack(const QString &track)
{
    Q_D(MetaDataParameters);
    d->searchTrack = track;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchTrack() const
{
    return d_ptr->searchTrack;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setSearchJournal(const QString &journal)
{
    Q_D(MetaDataParameters);
    d->searchJournal = journal;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::searchJournal() const
{
    return d_ptr->searchJournal;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setResourceType(const QString &resourceType)
{
    Q_D(MetaDataParameters);
    d->resourceType = resourceType;
}

QString NepomukWebMiner::Extractor::MetaDataParameters::resourceType() const
{
    return d_ptr->resourceType;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setResourceUri(const KUrl &resourceUri)
{
    Q_D(MetaDataParameters);
    d->resourceUri = resourceUri;
}

KUrl NepomukWebMiner::Extractor::MetaDataParameters::resourceUri() const
{
    return d_ptr->resourceUri;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setMetaData(const QVariantMap &metaData)
{
    Q_D(MetaDataParameters);
    d->metaData = metaData;
}

QVariantMap NepomukWebMiner::Extractor::MetaDataParameters::metaData() const
{
    return d_ptr->metaData;
}

void NepomukWebMiner::Extractor::MetaDataParameters::setMetaDataSaved(bool metaDataSaved)
{
    Q_D(MetaDataParameters);
    d->metaDataSaved = metaDataSaved;
}

bool NepomukWebMiner::Extractor::MetaDataParameters::metaDataSaved() const
{
    return d_ptr->metaDataSaved;
}
