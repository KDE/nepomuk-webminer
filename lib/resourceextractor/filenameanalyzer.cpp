/*
 * Copyright 2013 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#include "filenameanalyzer.h"

#include "metadataparameters.h"
#include "mdesettings.h"

#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <KDE/KMimeType>

#include <KDE/KDebug>

namespace NepomukWebMiner
{
namespace Extractor
{
class FilenameAnalyzerPrivate
{
public:
    bool tvShowMode;
    bool movieShowMode;

    QList<QRegExp> stripNames;
    QList<RegExpData> regexpTvShows;
    QList<RegExpData> regexpMovies;
    QList<RegExpData> regexpDocuments;
    QList<RegExpData> regexpMusic;
};
}
}

NepomukWebMiner::Extractor::FilenameAnalyzer::FilenameAnalyzer(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukWebMiner::Extractor::FilenameAnalyzerPrivate)
{
    reloadRegExp();
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::setTvShowMode(bool tvshowmode)
{
    Q_D(FilenameAnalyzer);
    d->tvShowMode = tvshowmode;
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::setMovieMode(bool moviemode)
{
    Q_D(FilenameAnalyzer);
    d->movieShowMode = moviemode;
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::analyze(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const KUrl &fileUrl)
{
    Q_D(FilenameAnalyzer);
    QString filenName = fileUrl.fileName();

    stripFileName(filenName);
    cleanFileName(filenName);
    filenName = filenName.simplified();

    KSharedPtr<KMimeType> kmt = KMimeType::findByUrl(fileUrl);
    QString mimetype = kmt.data()->name();

    bool extractionWorked = false;
    // select the right set of regexp for a specific mimetype
    if ( mimetype.contains(QLatin1String("application/vnd.oasis.opendocument.text")) ) {
        extractionWorked = analyzeFileName(d->regexpDocuments, mdp, filenName, fileUrl );
    }
    else if( mimetype.contains(QLatin1String("application/pdf")) ) {
        extractionWorked = analyzeFileName(d->regexpDocuments, mdp, filenName, fileUrl );
    }
    else if ( mimetype.contains(QLatin1String("video/")) ) {
        if(d->movieShowMode) {
            extractionWorked = analyzeFileName(d->regexpMovies, mdp, filenName, fileUrl );
        }
        else if(d->tvShowMode) {
            extractionWorked = analyzeFileName(d->regexpTvShows, mdp, filenName, fileUrl );
        }
        else {
            // we did not specify directly if thsi was a tvshow or movie
            // so try tvshow first, it it fails try movie afterwards
            extractionWorked = analyzeFileName(d->regexpTvShows, mdp, filenName, fileUrl );

            if(!extractionWorked) {
                extractionWorked = analyzeFileName(d->regexpMovies, mdp, filenName, fileUrl );

                if(extractionWorked) {
                    mdp->setResourceType(QLatin1String("movie"));
                }
            }
            else {
                mdp->setResourceType(QLatin1String("tvshow"));
            }
        }
    }
    else if( mimetype.contains(QLatin1String("audio/")) ) {
        extractionWorked = analyzeFileName(d->regexpMusic, mdp, filenName, fileUrl );
    }


    // if by now no analyzer found something, we use the stripped filename as searchtitle
    // if nothing mat
    if(! extractionWorked ) {
        mdp->setSearchTitle( filenName );
    }
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::stripFileName(QString &fileName)
{
    Q_D(FilenameAnalyzer);

    //first remove filetype (.avi, .pdf and so on)
    int p = fileName.lastIndexOf('.');
    if(p != -1) {
        fileName.remove(p, fileName.size());
    }

    // now remove anything the user has specified in the KConfig
    foreach(const QRegExp &regExp, d->stripNames) {
        fileName.replace(regExp, QLatin1String(""));
    }
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::cleanFileName(QString &fileName)
{
    fileName.replace('.', ' ');
    fileName.replace('_', ' ');
}

bool NepomukWebMiner::Extractor::FilenameAnalyzer::analyzeFileName(QList<Extractor::RegExpData> regExpList, NepomukWebMiner::Extractor::MetaDataParameters *mdp, const QString &filename, const KUrl &fileUrl)
{
    bool foundMatch = false;

    foreach(const RegExpData &red, regExpList) {
        QString searchString;
        if(red.useFolder) {
            searchString = fileUrl.prettyUrl();
        }
        else {
            searchString = filename;
        }

        if(red.regExp.exactMatch(searchString)) {
            kDebug() << "found match for: " << searchString << "via: " << red.regExp.pattern();
            foundMatch = true;

            //now get the matched group strings.
            QStringList matches = red.regExp.capturedTexts();

            if(matches.size() > 1) {
                //ignore first entry, this has the complete string again
                for(int i=1; i<matches.size(); i++) {
                    saveResult(mdp, matches.at(i), red.matchList.at(i-1));
                }
            }

            break;
        }
        else {
            kDebug() << "no match for: " << searchString << "via: " << red.regExp.pattern();
        }
    }

    return foundMatch;
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::saveResult(NepomukWebMiner::Extractor::MetaDataParameters *mdp, const QString &text, MatchTypes type)
{
    switch(type) {
    case Extractor::MATCH_TITLE:
        mdp->setSearchTitle(text);
        kDebug() << "set title name to: " << text;
        break;
    case Extractor::MATCH_SHOW:
        mdp->setSearchShowTitle(text);
        kDebug() << "set tvshow to: " << text;
        break;
    case Extractor::MATCH_SEASON:
        mdp->setSearchSeason(text);
        kDebug() << "set season to: " << text;
        break;
    case Extractor::MATCH_EPISODE:
        mdp->setSearchEpisode(text);
        kDebug() << "set episode to: " << text;
        break;
    case Extractor::MATCH_PERSON:
        mdp->setSearchPerson(text);
        kDebug() << "set person to: " << text;
        break;
    case Extractor::MATCH_ALBUM:
        mdp->setSearchAlbum(text);
        kDebug() << "set album to: " << text;
        break;
    case Extractor::MATCH_YEAR:
        mdp->setSearchYearMax(text);
        mdp->setSearchYearMin(text);
        kDebug() << "set year to: " << text;
        break;
    case Extractor::MATCH_TRACK:
        mdp->setSearchTrack(text);
        kDebug() << "set track to: " << text;
        break;
    }
}

void NepomukWebMiner::Extractor::FilenameAnalyzer::reloadRegExp()
{
    //FIXME: find a way to reload regexp cache whenever the KConfig/user changes the file
    kDebug() << "reload filename regexp cache";

    Q_D(FilenameAnalyzer);

    foreach(const QString &s, MDESettings::stripNames()) {
        d->stripNames.append( QRegExp(s, Qt::CaseInsensitive) );
    }

    d->regexpTvShows << parseRegExpConfig( MDESettings::regExpTvShow() );
    d->regexpMovies << parseRegExpConfig( MDESettings::regExpMovie() );
    d->regexpMusic << parseRegExpConfig( MDESettings::regExpMusic() );
    d->regexpDocuments << parseRegExpConfig( MDESettings::regExpDocument() );
}

QList<NepomukWebMiner::Extractor::RegExpData> NepomukWebMiner::Extractor::FilenameAnalyzer::parseRegExpConfig(const QString &config)
{
    QList<Extractor::RegExpData> regexpList;
    // in the KConfig each single entry is encapsluated by #|# and the single parts are encapsulated by #,#
    // its done this way, because ;,| and so on could be part of the regular expression and thus cause problems
    foreach(const QString &entry, config.split(QLatin1String("#|#"))) {
        QStringList settings = entry.split(QLatin1String("#,#"));

        if(settings.size() == 4) {
            Extractor::RegExpData red;
            red.regExp = QRegExp(settings.at(0), Qt::CaseInsensitive, QRegExp::RegExp2);

            if(settings.at(1).toLower() == "true") {
                red.useFolder = true;
            }
            else {
                red.useFolder = false;
            }

            foreach(const QString &match, settings.at(2).split(',')) {
                red.matchList << (Extractor::MatchTypes)match.toInt();
            }

            red.example = settings.at(3);

            regexpList << red;
        }
        else {
            kDebug() << "could not parse the KConfig entry " << entry;
        }
    }

    return regexpList;
}

QString NepomukWebMiner::Extractor::FilenameAnalyzer::writeRegExpConfig(const QList<NepomukWebMiner::Extractor::RegExpData> &data)
{
    QString configString;

    // Each entry is looks like this
    // #|# splits the RegExpData
    // #,# splits the data within RegExpData
    // regexp#,#useFolder#,#1,2,3#|#regexp#,#useFolder#,#1,2,3

    foreach(const Extractor::RegExpData &red, data) {
        configString.append( red.regExp.pattern() );
        configString.append(QLatin1String("#,#"));

        if( red.useFolder ) {
            configString.append(QLatin1String("true"));
        }
        else {
            configString.append(QLatin1String("false"));
        }
        configString.append(QLatin1String("#,#"));

        foreach(Extractor::MatchTypes mt, red.matchList) {
            configString.append( QString::number((int)mt) );
            configString.append(QLatin1String(","));
        }
        configString.chop(1); // remove last ","

        configString.append(QLatin1String("#,#"));
        configString.append(red.example);
        configString.append(QLatin1String("#|#"));
    }
    configString.chop(3); // remove last "#|#"

    return configString;
}