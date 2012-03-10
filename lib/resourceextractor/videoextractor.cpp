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

#include "videoextractor.h"

#include "../metadataparameters.h"

#include <QtCore/QStringList>

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class VideoExtractorPrivate {
        public:
            QList<QRegExp> filenameRegExps;
        };
    }
}

NepomukMetaDataExtractor::Extractor::VideoExtractor::VideoExtractor(QObject *parent)
    : QObject(parent)
    , d_ptr( new NepomukMetaDataExtractor::Extractor::VideoExtractorPrivate )
{
    Q_D( VideoExtractor );

    // Regular expressions to parse file based on the ones from tvnamer.py
    // Copyright dbr/Ben
    // Regex's to parse filenames with. Must have 3 groups, seriesname, season number
    // and episode number. Use (?: optional) non-capturing groups if you need others.

    // foo_[s01]_[e01]
    d->filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]\\[s([0-9]+)\\]_\\[e([0-9]+)\\]?[^\\\\/]*" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.1x09*
    d->filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]\\[?([0-9]+)x([0-9]+)[^\\d]?[^\\\\/]*" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.s01.e01, foo.s01_e01
    d->filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]s([0-9]+)[\\._\\- ]?e([0-9]+)[^\\\\/]*" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    /* ignored as they clash with movie detection. for example The.Matrix.1999
    // foo.103* (the strange part at the end is used to 1. prevent another digit and 2. allow the name to end)
    d->filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]([0-9]{1})([0-9]{2})(?:[^\\d][^\\\\/]*)?" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.0103* (the strange part at the end is used to 1. prevent another digit and 2. allow the name to end)
    d->filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]([0-9]{2})([0-9]{2,3})(?:[^\\d][^\\\\/]*)?" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );
    */
}

void NepomukMetaDataExtractor::Extractor::VideoExtractor::parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl)
{
    Q_D( VideoExtractor );

    mdp->resourceUri = fileUrl;
    mdp->resourceType = QLatin1String("publication");

    // 1. extract base name of the file
    // TODO: analyze the path, too. In case we have something like: "foobar - Season 2/02x12 - blabla.avi"
    QString name = fileUrl.fileName();

    // 2. run the base name through all regular expressions we have
    bool foundSeries = false;
    for ( int i = 0; i < d->filenameRegExps.count(); ++i ) {
        QRegExp& exp = d->filenameRegExps[i];
        if ( exp.exactMatch( name ) ) {

            foundSeries = true;

            mdp->searchShowTitle = exp.cap( 1 ).simplified();
            mdp->searchSeason = exp.cap( 2 );
            mdp->searchEpisode = exp.cap( 3 );

            // 3. clean up tv show name
            mdp->searchShowTitle.replace( '.', ' ' );
            mdp->searchShowTitle.replace( '_', ' ' );
            if ( mdp->searchShowTitle.endsWith( '-' ) )
                mdp->searchShowTitle.truncate( mdp->searchShowTitle.length()-1 );
            mdp->searchShowTitle = mdp->searchShowTitle.simplified();
        }
    }

    if(foundSeries) {
        mdp->resourceType = QLatin1String("tvshow");
    }
    else {
        mdp->resourceType = QLatin1String("movie");

        QStringList nameSplit = name.split(QLatin1String("."));
        //name.replace('.', ' ');
        //name.replace('-', ' ');

        //TODO: Remove the extension
        //TODO: Remove special characters like [] ()
        //TODO: Remove DVDRip
        //TODO: Remove the year

        mdp->searchTitle = nameSplit.first();
    }
}
