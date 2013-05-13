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

#include <KDE/KApplication>
#include <KDE/KCmdLineArgs>
#include <KDE/KAboutData>
#include <KDE/KUrl>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QFileInfo>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>

#include "ui/fetcher.h"
#include "ui/fetcherdialog.h"
#include "ui/automaticfetcher.h"

#include <KDE/KDebug>

int main(int argc, char *argv[])
{
    KAboutData aboutData("nepomuk-webminer",
                         "nepomuk-webminer",
                         ki18n("Nepomuk-WebMiner"),
                         "0.6",
                         ki18n("Nepomuk-WebMiner - Harvest and add metadata for files and resources on the internet"),
                         KAboutData::License_GPL,
                         ki18n("(c) 2012, Jörg Ehrichs"),
                         KLocalizedString(),
                         "http://kde.org");
    aboutData.addAuthor(ki18n("Jörg Ehrichs"), ki18n("Maintainer"), "joerg.ehrichs@gmx.de");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("f").add("force", ki18n("Force meta data fetching even for files that already have them"));
    options.add("a").add("auto", ki18n("Fetch data automatically without user interaction"));
    options.add("u").add("url", ki18n("Fetch website and save meta data without file connection"));
    options.add("r").add("resource", ki18n("Fetch data for a Nepomuk Resource information"));

    options.add("", ki18n("If no --url or --resource is used, the given url argument will be treated as local file/folder"));

    options.add("s").add("standalone", ki18n("When set the automatic fetcher does not use the nepomuk service for execution"));

    options.add(":", ki18n("Extra options to help filename parsing:"));
    options.add("t").add("tvshow", ki18n("Defines that the folder we are working on contains only one or more tvshow."));
    options.add("m").add("movie", ki18n("Defines that the folder we are working on contains only one or more movies."));

    options.add("+url", ki18n("The input url to the file/folder, website or nepomuk resource."));

    options.add("", ki18n("Some Examples:\n"
                          "\tnepomuk-webminer ~/Documents\n"
                          "\tnepomuk-webminer -a -u http://www.imdb.com/title/tt1254207/\n"
                          "\tnepomuk-webminer -a -t ~/Videos\n"
                          "\tnepomuk-webminer -r nepomuk:/res/eb9bc9e1-707d-47af-952c-d7381480fbcc"));

    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
    KApplication app;

    if (!args->count() || !args->url(0).isValid()) {
        KCmdLineArgs::usageError(i18n("No url specified.\nPlease start it with nepomuk-webminer &lt;url&gt;.\n"
                                      "For example: nepomuk-webminer ~/Documents\n"
                                      "Or: nepomuk-webminer -tvshow -auto ~/Videos\n"));
        return 0;
    }


    if (args->isSet("auto")) {

        if( !args->isSet("standalone")) {
            if ( QDBusConnection::sessionBus().interface()->isServiceRegistered( "org.kde.nepomuk.services.nepomuk-webminerservice" ) ) {

                // connect to the dbus interface
                QDBusInterface dBusWebMiner(QLatin1String("org.kde.nepomuk.services.nepomuk-webminerservice"),
                                            QLatin1String("/WebMiner"),
                                            QLatin1String("org.kde.nepomuk.WebMiner"),
                                            QDBusConnection::sessionBus());

                dBusWebMiner.call( "indexManually", args->url(0).toLocalFile());

                // stop execution here if we use the service for execution
                return 0;
            }
            else {
                kDebug() << "service not available, fallback to no service execution";
            }
        }

        // normal execution if no service is used for automatic extraction
        // or the service was not available

        NepomukWebMiner::UI::AutomaticFetcher af;

        QObject::connect(&af, SIGNAL(finished()), &app, SLOT(quit()));

        if (args->isSet("url")) {
            af.addFetcherUrl(args->url(0));
            QTimer::singleShot(0, &af, SLOT(startUrlFetcher()));
        }
        else if (args->isSet("resource")) {
            af.addFetcherResource(args->url(0));
            QTimer::singleShot(0, &af, SLOT(startFetcher()));
        }
        else {
            af.setForceUpdate(args->isSet("force"));
            af.setTvShowMode(args->isSet("tvshow"));
            af.setMovieMode(args->isSet("movie"));

            af.addFetcherPath(args->url(0));
            QTimer::singleShot(0, &af, SLOT(startFetcher()));
        }

        return app.exec();
    } else {
        NepomukWebMiner::UI::FetcherDialog fd;
        fd.setForceUpdate(args->isSet("f"));
        fd.setTvShowMode(args->isSet("tvshow"));
        fd.setMovieMode(args->isSet("movie"));

        if (args->isSet("url")) {
        }
        else if (args->isSet("resource")) {
            fd.addFetcherResource(Nepomuk2::Resource::fromResourceUri(args->url(0)));
        }
        else {
            QFileInfo fi(args->url(0).prettyUrl());
            if(fi.isFile()) {
                fd.addFetcherPath(args->url(0));
            }
            else {
                // show a dialog with the status of the current file fetching
                // this can be canceled and closes the dialog again
                if (!fd.startFetching(args->url(0)))
                    return 0;
            }
        }

        QObject::connect(&fd, SIGNAL(finished(int)), &app, SLOT(quit()));
        fd.show();

        return app.exec();
    }
}
