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

#include "videoextractor.h"

#include "../metadataparameters.h"

#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <KDE/KDebug>

namespace NepomukMetaDataExtractor
{
namespace Extractor
{
class VideoExtractorPrivate
{
public:
    QList<QRegExp> tvshowFolderRegExpsCompiled;
    QList<QRegExp> tvshowFilenameRegExps;
    QList<QRegExp> movieFilenameRegExps;
    bool tvshowOnly;
    bool useFolderNames;
    bool movieOnly;
};
}
}

NepomukMetaDataExtractor::Extractor::VideoExtractor::VideoExtractor(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukMetaDataExtractor::Extractor::VideoExtractorPrivate)
{
    Q_D(VideoExtractor);
    d->tvshowOnly = false;
    d->useFolderNames = false;
    d->movieOnly = false;

    // -----------------------------------------------------------------------------------
    // Regular expressions for the folder structure

    // this part defines the season element. will not match /Title/1/filename this one would
    // clash with detection on the anime /5 centimeteres per second/filename and others
    //TODO add more special cases
    QLatin1String seasonRe(".*(Season|Staffel|S|Series)?.*");

    // Title/Season 1/filename or xyz/Title/1 Season/filename or xyz/Title/S1/filename
    // include specials like the anime Blood+
    d->tvshowFolderRegExpsCompiled.append(
        QRegExp(QLatin1String("[\\\\/](.*)[\\\\/]") + seasonRe + QLatin1String("([0-9]{1,2})") + seasonRe + QLatin1String("[\\\\/]$"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // Title/filename
    d->tvshowFolderRegExpsCompiled.append(
        QRegExp(QLatin1String("^[\\\\/]?([^\\\\/]*)[\\\\/]$"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // Regular expressions to parse file based on the ones from tvnamer.py
    // Copyright dbr/Ben
    // Regex's to parse filenames with. Must have 3 groups, seriesname, season number
    // and episode number. Use (?: optional) non-capturing groups if you need others.

    // foo_[s01]_[e01]
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]\\[s([0-9]+)\\]_\\[e([0-9]+)\\]?[^\\\\/]*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // foo.1x09*
    // we match it such that season number is at most 2 digits to avoid conflicting with resolution tags, eg "[640x360]"
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]\\[?([0-9]{1,2})x([0-9]+)[^\\d]?[^\\\\/]*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // foo.s01.e01, foo.s01_e01
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]s([0-9]+)[\\._\\- ]?e([0-9]+)[^\\\\/]*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // foo - Episode 01 or foo - e01
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.*)\\s?[\\.|\\_|\\-]+\\s?(?:Episode|e)+[\\s|\\.|\\_|\\-]?([0-9]{1,2})\\s?\\-?\\s?.*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // would also match movies like Matrix.1999
    //if(d->tvshowOnly) {
    // this should match most (hopefully all) anime fansub file formats
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]s([0-9]{1,2})[ \\._\\-]+([0-9]+)[^\\\\/]*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // this would match fansub files where season numbers are ommitted, which is common when it's season 1
    // we assume episode numbers don't go into the hundreds: else it'll overlap with the next regex,
    // and we have no way of deciding which regex is right..
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]([0-9]{2})(?:[^\\d][^\\\\/]*)?"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // foo.103* (the strange part at the end is used to 1. prevent another digit and 2. allow the name to end)
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]([0-9]{1})([0-9]{2})(?:[^\\d][^\\\\/]*)?"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // foo.0103* (the strange part at the end is used to 1. prevent another digit and 2. allow the name to end)
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String("(.+)[ \\._\\-]([0-9]{2})([0-9]{2,3})(?:[^\\d][^\\\\/]*)?"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // Episode 01
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String(".*(?:Episode)+[\\s|\\.|\\_|\\-]?([0-9]{1,2})\\s?\\-?\\s?.*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));

    // e01 or e01 - episode title
    d->tvshowFilenameRegExps.append(
        QRegExp(QLatin1String(".*e+([0-9]{1,2})[\\s|\\-|\\s]+.*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));
    //}

    // The following try to extract the movie name and year
    // match The.Title.2012 or The.Title.(2012) or The.Title.[2012]
    d->movieFilenameRegExps.append(
        QRegExp(QLatin1String("(.*)(?:\\s|\\.|\\-|\\_|\\(|\\[)+([0-9]{4}).*"),
                Qt::CaseInsensitive, QRegExp::RegExp2));
}

void NepomukMetaDataExtractor::Extractor::VideoExtractor::setTvShowMode(bool tvshowmode)
{
    Q_D(VideoExtractor);
    d->tvshowOnly = tvshowmode;

    if (d->movieOnly && d->tvshowOnly) {
        d->tvshowOnly = false;
        d->movieOnly = false;
    }
}

void NepomukMetaDataExtractor::Extractor::VideoExtractor::setTvShowNamesInFolders(bool useFolderNames)
{
    Q_D(VideoExtractor);
    d->useFolderNames = useFolderNames;
}

void NepomukMetaDataExtractor::Extractor::VideoExtractor::setMovieMode(bool moviemode)
{
    Q_D(VideoExtractor);
    d->movieOnly = moviemode;

    if (d->movieOnly && d->tvshowOnly) {
        d->tvshowOnly = false;
        d->movieOnly = false;
    }
}

void NepomukMetaDataExtractor::Extractor::VideoExtractor::parseUrl(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl)
{
    Q_D(VideoExtractor);
    mdp->setResourceUri(fileUrl);

    // All we need to check is if we can parse a movie name and year from the file name
    if (d->movieOnly) {
        mdp->setResourceType(QLatin1String("movie"));

        // in case the detection failed, we use the filename as search title for the web search
        if (!parseMovieFileName(mdp, fileUrl.fileName())) {
            mdp->setSearchTitle(fileUrl.fileName());
        }
    }

    // in this case the url is definitly a tv show
    if (d->tvshowOnly) {
        mdp->setResourceType(QLatin1String("tvshow"));

        // first check the folder name scheme
        if (d->useFolderNames) {
            if (!parseTvShowFolder(mdp, fileUrl, baseUrl)) {
                // should this fail we fall back to normal file name detection
                if (!parseTvShowFileName(mdp, fileUrl.fileName())) {
                    // fallback to filename if even this fails
                    mdp->setSearchTitle(fileUrl.fileName());
                }
            }
        } else {
            // if we did not specify to use folder names, use filenames only
            if (!parseTvShowFileName(mdp, fileUrl.fileName())) {
                // falback to filename if even this fails
                mdp->setSearchTitle(fileUrl.fileName());
            }
        }
    }

    // if we did not specify movie or tvshow explicit we must try out our luck
    if (!d->tvshowOnly && !d->movieOnly) {
        // first check tvshow file name
        if (parseTvShowFileName(mdp, fileUrl.fileName())) {
            mdp->setResourceType(QLatin1String("tvshow"));
            //yay works, stop here
            return;
        }

        //if this did not work, check movie file name
        if (parseMovieFileName(mdp, fileUrl.fileName())) {
            mdp->setResourceType(QLatin1String("movie"));
            //yay works, stop here
            return;
        }
    }

    // still no luck? :(
    mdp->setResourceType(QLatin1String("movie"));
    mdp->setSearchTitle(fileUrl.fileName().split('.').first());
}

bool NepomukMetaDataExtractor::Extractor::VideoExtractor::parseTvShowFolder(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl, const KUrl &baseUrl)
{
    Q_D(VideoExtractor);

    QString strippedFolderUpOne = fileUrl.directory(KUrl::AppendTrailingSlash).remove(baseUrl.upUrl().toLocalFile());

    foreach (const QRegExp & re, d->tvshowFolderRegExpsCompiled) {

        if (re.exactMatch(strippedFolderUpOne)) {

            if (!re.cap(1).simplified().isEmpty())
                mdp->setSearchShowTitle(re.cap(1).simplified());
            if (re.captureCount() == 4) {
                if (!re.cap(3).isEmpty())
                    mdp->setSearchSeason(re.cap(3));
            } else {
                if (!re.cap(2).isEmpty())
                    mdp->setSearchSeason(re.cap(2));
            }
            return true;
        }
    }

    return false;
}

bool NepomukMetaDataExtractor::Extractor::VideoExtractor::parseTvShowFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName)
{
    Q_D(VideoExtractor);
    foreach (const QRegExp & re, d->tvshowFilenameRegExps) {
        if (re.exactMatch(fileName)) {
            kDebug() << "Regexp matched" << re;
            for (int ii = 0; ii <= re.captureCount(); ii++) {
                kDebug() << ii << " : " << re.cap(ii);
            }
            if (re.captureCount() > 2) {
                mdp->setSearchShowTitle(re.cap(1).simplified());
                mdp->setSearchSeason(re.cap(2));
                mdp->setSearchEpisode(re.cap(3));

            } else if (re.captureCount() == 2) {
                mdp->setSearchShowTitle(re.cap(1));
                mdp->setSearchEpisode(re.cap(2));
                // should be no harm presetting this, some filenames omit the season assuming it's 1
                // presetting lets anime fansub files be automatically fetchable
                mdp->setSearchSeason(QChar('1'));
            } else {
                mdp->setSearchEpisode(re.cap(1));
            }
            // 3. clean up tv show name
            // remove subber tag from the show name if it exists
            QString curShowTitle = mdp->searchShowTitle();
            QRegExp subberCheck("\\[(.+)\\](.+)");
            if (subberCheck.exactMatch(curShowTitle)) {
                curShowTitle = subberCheck.cap(2).simplified();
            }
            curShowTitle.replace('.', ' ');
            curShowTitle.replace('_', ' ');
            if (curShowTitle.endsWith('-')) {
                curShowTitle.truncate(curShowTitle.length() - 1);
            }

            mdp->setSearchShowTitle(curShowTitle.simplified());

            return true;
        }
    }

    return false;
}

bool NepomukMetaDataExtractor::Extractor::VideoExtractor::parseMovieFileName(NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp, const QString &fileName)
{
    Q_D(VideoExtractor);
    foreach (const QRegExp & re, d->movieFilenameRegExps) {
        if (re.exactMatch(fileName)) {

            mdp->setSearchTitle(re.cap(1).simplified());
            return true;
        }
    }

    return false;
}
