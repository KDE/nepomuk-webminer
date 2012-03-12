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

int main( int argc, char *argv[] )
{
    KAboutData aboutData( "metadataextractor",
                          "metadataextractor",
                          ki18n("Metadata Extractor"),
                          "0.1",
                          ki18n("Metadata Extractor - Find publication metadata"),
                          KAboutData::License_GPL,
                          ki18n("(c) 2012, Jörg Ehrichs"),
                          KLocalizedString(),
                          "http://kde.org" );
    aboutData.addAuthor(ki18n("Jörg Ehrichs"), ki18n("Maintainer"), "joerg.ehrichs@gmx.de");

    KCmdLineArgs::init( argc, argv, &aboutData );

    KCmdLineOptions options;
    options.add("f", ki18n("Force meta data fetching even for files that already have them"));
    options.add("tvshow", ki18n("Helper parameter. Defines that the folder we are working on contains a tvshow. Works with einer Series/Episode1.avi or Series/Season/Episode1.avi. Will fetch metadata for all episodes"));
    options.add("+url", ki18n("The file or folder url used for the input"));

    KCmdLineArgs::addCmdLineOptions( options );
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
    KApplication app;

    if ( args->count() && args->url(0).isValid() ) {

        NepomukMetaDataExtractor::Dialog::FetcherDialog fd;

        if(args->isSet("f")) {
            fd.setForceUpdate(true);
        }

        fd.setInitialPathOrFile( args->url( 0 ) );
        fd.show();

        return app.exec();
    }
    else {

        KCmdLineArgs::usageError(i18n("No file or folder specified.\nPlease start it with metadataextractor <url>.\nFor example: metadataextractor ~/Documents"));
        return 0;
    }
}
