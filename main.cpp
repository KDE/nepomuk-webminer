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

#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KUrl>

#include <QTimer>

#include "metadatafetcher.h"

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
    options.add("+uri", ki18n("The URL to the publication file or folder to annotate."));
    KCmdLineArgs::addCmdLineOptions( options );
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

    KApplication app;

    MetaDataFetcher mdf;
    QObject::connect(&mdf, SIGNAL(fetchingDone()), &app, SLOT(quit()));


    KUrl debug("/home/joerg/Dokumente/meta-data-extractor/fetchtest/");
    //    KUrl debug("/home/joerg/Dokumente/metdaData-test.odt");
    mdf.lookupFiles( debug );

    QTimer::singleShot(0, &mdf, SLOT(run()));

    return app.exec();

//    if ( args->count() ) {
//        mdf.lookupFiles( args->url( 0 ) );
//        QTimer::singleShot(0, &mdf, SLOT(run()));

//        return app.exec();
//    }
//    else {
//        return 1;
//    }
}
