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

#include "metadataparameters.h"
#include "resourceextractor/filenameanalyzer.h"

#include <QtTest>
#include <QtDebug>
#include <KDE/KDebug>

#include <qtest_kde.h>

/**
 * @file analysefilename.cpp
 *
 * @test UnitTest for the filename analysing
 *       checks: filename if some search parameters can be extarcted
 */
class AnalyseFileName: public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();
    void checkTvShows_data();
    void checkTvShows();

    void checkMovies_data();
    void checkMovies();

    void checkMusic_data();
    void checkMusic();

    void checkDocuments_data();
    void checkDocuments();

private:
    NepomukWebMiner::Extractor::FilenameAnalyzer *fna;
};

QTEST_KDEMAIN_CORE(AnalyseFileName)

void AnalyseFileName::initTestCase()
{
    fna = new NepomukWebMiner::Extractor::FilenameAnalyzer;
}

void AnalyseFileName::checkTvShows_data()
{
    QTest::addColumn<QString>("filename");

    QTest::addColumn<QString>("show");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("season");
    QTest::addColumn<QString>("episode");

    QTest::newRow("Test 1") << "foo [s02][e03].avi" << "foo" << "" << "2" << "3";
    QTest::newRow("Test 2") << "foo 2x03.avi" << "foo" << "" << "2" << "3";
    QTest::newRow("Test 3") << "foo s02e03.avi" << "foo" << "" << "2" << "3";
    QTest::newRow("Test 4") << "foo s2-3.avi" << "foo" << "" << "2" << "3";
    QTest::newRow("Test 5") << "foo - 03.avi" << "foo" << "" << "1" << "3";
    QTest::newRow("Test 6") << "foo - 203.avi" << "foo" << "" << "1" << "203";
    QTest::newRow("Test 7") << "foo - 0203.avi" << "foo" << "" << "2" << "3";

    QTest::newRow("Test 8") << "/Videos/foo/Episode 03.avi" << "foo" << "" << "1" << "3";
    QTest::newRow("Test 9") << "/Videos/foo/Season 02/e03 - bar.avi" << "foo" << "" << "2" << "3";
    QTest::newRow("Test 10") << "/Videos/foo/Episode 03 - bar.avi" << "foo" << "" << "1" << "3";
    QTest::newRow("Test 11") << "foo 203.avi" << "foo" << "" << "2" << "3";

    //QTest::newRow("Test 9") << "/Videos/foo/Season 02/e03 - bar.avi" << "foo" << "bar" << "2" << "3";
    //QTest::newRow("Test 10") << "/Videos/foo/Episode 03 - bar.avi" << "foo" << "bar" << "1" << "3";

    // anime filename patterns
    QTest::newRow("Test 12") << "[Fan_Subber] Foo Bar - 08 (1280x720 h264 AAC) [4169FF55].mkv" << "Foo Bar" << "" << "1" << "8";
    QTest::newRow("Test 13") << "[SubGroup] Show Name! - 157 [720p] [06BD4243].mkv" << "Show Name!" << "" << "1" << "157";
    QTest::newRow("Test 14") << "[sub-group]_foo_bar_-_02v2_[665EB12C].mkv" << "foo bar" << "" << "1" << "2";
    QTest::newRow("Test 15") << "[FoofooGroup].Foo!.02.h264.vorbis.mkv" << "Foo!" << "" << "1" << "2";
    QTest::newRow("Test 16") << "FOOBAR_24_WS_[H264][AQS][Sprocket][TWH][90635A6F].mkv.avi" << "FOOBAR" << "" << "1" << "24";
    QTest::newRow("Test 17") << "[Group]Foo_-_06_-_EpTitle_[AnGroup]_[FC7D9D6F].mkv" << "Foo" << "" << "1" << "6";

    fna->setTvShowMode(true);
    fna->setMovieMode(false);
}

void AnalyseFileName::checkTvShows()
{
    QFETCH(QString, filename);

    NepomukWebMiner::Extractor::MetaDataParameters mdp;

    fna->analyze(&mdp, KUrl(filename));

    QFETCH(QString, show);
    QFETCH(QString, title);
    QFETCH(QString, season);
    QFETCH(QString, episode);

    QCOMPARE(mdp.resourceType(), QLatin1String("tvshow"));
    QCOMPARE(mdp.searchShowTitle(), show);
    QCOMPARE(mdp.searchTitle(), title);
    QCOMPARE(mdp.searchSeason(), season);
    QCOMPARE(mdp.searchEpisode(), episode);
}

void AnalyseFileName::checkMovies_data()
{
    QTest::addColumn<QString>("filename");

    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("year");

    QTest::newRow("Test 1") << "Foo_bar_[2013].avi" << "Foo bar" << "2013";
    QTest::newRow("Test 2") << "Foo_bar_(2013).avi" << "Foo bar" << "2013";

    //QTest::newRow("Test 3") << "Foo_bar_-_2013.avi" << "Foo bar" << "2013";
    //QTest::newRow("Test 4") << "Foo_bar_-_(2013).avi" << "Foo bar" << "2013";

    fna->setTvShowMode(false);
    fna->setMovieMode(true);
}

void AnalyseFileName::checkMovies()
{
    QFETCH(QString, filename);

    NepomukWebMiner::Extractor::MetaDataParameters mdp;

    fna->analyze(&mdp, KUrl(filename));

    QFETCH(QString, title);
    QFETCH(QString, year);

    QCOMPARE(mdp.resourceType(), QLatin1String("movie"));
    QCOMPARE(mdp.searchTitle(), title);
    QCOMPARE(mdp.searchYearMax(), year);
    QCOMPARE(mdp.searchYearMin(), year);
}

void AnalyseFileName::checkMusic_data()
{
    QTest::addColumn<QString>("filename");

    QTest::addColumn<QString>("artist");
    QTest::addColumn<QString>("album");
    QTest::addColumn<QString>("track");
    QTest::addColumn<QString>("title");

    QTest::newRow("Test 1") << "Artist - 04 - Track.mp3" << "Artist" << "" << "4" << "Track";
    QTest::newRow("Test 2") << "/Music/Artist/Album/04 - Track.mp3" << "Artist" << "Album" << "4" << "Track";
    QTest::newRow("Test 2") << "/Music/Artist/Album/Track.mp3" << "Artist" << "Album" << "" << "Track";
    QTest::newRow("Test 4") << "Artist - Track.mp3" << "Artist" << "" << "" << "Track";

    fna->setTvShowMode(false);
    fna->setMovieMode(false);
}

void AnalyseFileName::checkMusic()
{
    QFETCH(QString, filename);

    NepomukWebMiner::Extractor::MetaDataParameters mdp;

    fna->analyze(&mdp, KUrl(filename));

    QFETCH(QString, artist);
    QFETCH(QString, album);
    QFETCH(QString, track);
    QFETCH(QString, title);

    QCOMPARE(mdp.resourceType(), QLatin1String("music"));
    QCOMPARE(mdp.searchPerson(), artist);
    QCOMPARE(mdp.searchAlbum(), album);
    QCOMPARE(mdp.searchTrack(), track);
    QCOMPARE(mdp.searchTitle(), title);
}

void AnalyseFileName::checkDocuments_data()
{
    //no default regular expressions given yet

    fna->setTvShowMode(false);
    fna->setMovieMode(false);
}

void AnalyseFileName::checkDocuments()
{
}

#include "analysefilename.moc"
