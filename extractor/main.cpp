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

#include <KDE/KApplication>
#include <KDE/KCmdLineArgs>
#include <KDE/KAboutData>
#include <KDE/KUrl>

#include "ui/fetcherdialog.h"
#include "ui/automaticfetcher.h"

int main( int argc, char *argv[] )
{
    KAboutData aboutData( "metadataextractor",
                          "metadataextractor",
                          ki18n("Metadata Extractor"),
                          "0.2",
                          ki18n("Metadata Extractor - Find and add metadata for files and resources on the internet"),
                          KAboutData::License_GPL,
                          ki18n("(c) 2012, Jörg Ehrichs"),
                          KLocalizedString(),
                          "http://kde.org" );
    aboutData.addAuthor(ki18n("Jörg Ehrichs"), ki18n("Maintainer"), "joerg.ehrichs@gmx.de");

    KCmdLineArgs::init( argc, argv, &aboutData );

    KCmdLineOptions options;
    options.add("f").add("force", ki18n("Force meta data fetching even for files that already have them"));
    options.add("a").add("auto", ki18n("Fetch data automatically without user interaction"));

    options.add("t").add("tvshow", ki18n("Optional Helper parameter to find better searchparameter. Defines that the folder we are working on contains only one or more tvshow. Will fetch metadata for all episodes"));
    options.add("u").add("usefoldernames", ki18n("Defines that for tvshows the folder names have necessary name info (like Series/Episode1.avi or Series/Season/Episode1.avi) otherwise only SeriesS01E01.avi etc will work."));

    options.add("m").add("movie", ki18n("Helper parameter to find better searchparameter. Defines that the folder we are working on contains only one or more movies."));

    options.add("+url", ki18n("The file or folder url used for the input"));

    KCmdLineArgs::addCmdLineOptions( options );
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
    KApplication app;

    if ( !args->count() || !args->url(0).isValid() ) {
        KCmdLineArgs::usageError(i18n("No file or folder specified.\nPlease start it with metadataextractor <url>.\n"
                                      "For example: metadataextractor ~/Documents\n"
                                      "Or: metadataextractor -tvshow -auto ~/Videos\n"));
        return 0;
    }


    if(args->isSet("auto")) {
        NepomukMetaDataExtractor::UI::AutomaticFetcher af;
        af.setForceUpdate( args->isSet("force") );
        af.setTvShowMode( args->isSet("tvshow") );
        af.setTvShowNamesInFolders( args->isSet("usefoldernames") );
        af.setMovieMode( args->isSet("movie") );

        af.setInitialPathOrFile( args->url( 0 ) );

        af.startFetcher();
        return app.exec();
    }
    else {
        NepomukMetaDataExtractor::UI::FetcherDialog fd;
        fd.setForceUpdate( args->isSet("f") );
        fd.setTvShowMode( args->isSet("tvshow") );

        fd.setInitialPathOrFile( args->url( 0 ) );
        fd.show();

        return app.exec();
    }
}