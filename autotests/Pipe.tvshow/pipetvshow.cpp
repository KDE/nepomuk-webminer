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

#include "nepomukpipe/tvshowpipe.h"
#include <QtCore/QVariantMap>
#include <QtCore/QVariant>

#include <Nepomuk2/ResourceManager>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>
#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NIE>
#include <Soprano/Vocabulary/RDFS>

#include <Nepomuk2/DataManagement>
#include <KDE/KJob>

#include <QtTest>
#include <QtDebug>
#include <KDE/KDebug>

#include <qtest_kde.h>

/**
 * @file pipetvshow.cpp
 *
 * @test UnitTest for the NepomukPipe => TvShow.
 *       checks: imports a tvshow from QVariantList, checks if Nepomuk data is ok
 */
class PipeTvShow: public QObject
{
    Q_OBJECT

private slots:

    void importShow();
    void checkImportedShow();

    void cleanupTestCase();

private:
    QVariantMap testShow;
    QVariantMap testSeason;
    QVariantMap testEpisode;
};

QTEST_KDEMAIN_CORE(PipeTvShow)

void PipeTvShow::importShow()
{
    testEpisode.insert(QLatin1String("title"), QLatin1String("UNIT-TEST-EPISODE"));
    testEpisode.insert(QLatin1String("overview"), QLatin1String("UNIT-TEST-EPISODE overview"));
    testEpisode.insert(QLatin1String("number"), QLatin1String("4"));
    testEpisode.insert(QLatin1String("firstaired"), QLatin1String("2013-07-10"));
    testEpisode.insert(QLatin1String("genres"), QLatin1String("UNIT-TEST-GENRE1;UNIT-TEST-GENRE2;UNIT-TEST-GENRE3"));
    testEpisode.insert(QLatin1String("director"), QLatin1String("Unit-Test Director1;Unit-Test Director2;Unit Test-Director3"));
    testEpisode.insert(QLatin1String("writer"), QLatin1String("Unit-Test Writer1;Unit-Test Writer2;Unit-Test Writer3"));
    testEpisode.insert(QLatin1String("actors"), QLatin1String("Unit-Test Cast1;Unit-Test Cast2;Unit-Test cast3"));

    testSeason.insert(QLatin1String("number"), QLatin1String("5"));
    testSeason.insert(QLatin1String("episodes"), QVariantList() << testEpisode);

    testShow.insert(QLatin1String("title"), QLatin1String("UNIT-TEST-TVSHOW"));
    testShow.insert(QLatin1String("overview"), QLatin1String("UNIT-TEST-TVSHOW overview"));
    testShow.insert(QLatin1String("seealso"), QLatin1String("http://unit-test-see.also"));
    testShow.insert(QLatin1String("seasons"), QVariantList() << testSeason);

    NepomukWebMiner::Pipe::TvShowPipe tvsp;
    tvsp.overrideComponentName(QLatin1String("WebMiner-Unit-Test"));

    bool importSucessful = tvsp.import(testShow);

    QVERIFY2( importSucessful == true, "TvShow import was not possible");
}

void PipeTvShow::checkImportedShow()
{
    //now fetch the nepomuk database again and see if the tvshow and all data are imported correctly
    QString query = QString::fromLatin1("select ?r where {"
                                        "?r a nmm:TVSeries ."
                                        "?r nie:title ?t ."
                                        "FILTER regex(?t, \"UNIT-TEST-TVSHOW\") ."
                                        "}");
    Soprano::Model* model = Nepomuk2::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );

    Nepomuk2::Resource tvSeries;
    //we check only the first result
    if( !it.next()) {
        QFAIL("No tvshow with the title \"UNIT-TEST-TVSHOW\" found");
    }
    else {
        Soprano::BindingSet p = it.current();
        tvSeries = Nepomuk2::Resource::fromResourceUri(p.value("r").toString());
    }

    if( !tvSeries.isValid() || !tvSeries.exists()) {
        QFAIL("Found TvShow Resource is not valid");
    }

    // First check the TvShow data
    QString overview = tvSeries.property(Nepomuk2::Vocabulary::NIE::description()).toString();
    QCOMPARE(overview, testShow.value(QLatin1String("overview")).toString());

    QList<Nepomuk2::Resource> seasonListRes = tvSeries.property(Nepomuk2::Vocabulary::NMM::hasSeason()).toResourceList();
    QCOMPARE(seasonListRes.size(), 1);


    //Now check that the season data is correct
    Nepomuk2::Resource seasonRes = seasonListRes.first();

    QString seasonNumber = seasonRes.property(Nepomuk2::Vocabulary::NMM::seasonNumber()).toString();
    QCOMPARE(seasonNumber, testSeason.value("number").toString());

    QList<Nepomuk2::Resource> episodeListRes = seasonRes.property(Nepomuk2::Vocabulary::NMM::hasSeasonEpisode()).toResourceList();
    QCOMPARE(episodeListRes.size(), 1);


    //Now check that the episode data is correct
    Nepomuk2::Resource episodeRes = episodeListRes.first();

    QString title = episodeRes.property(Nepomuk2::Vocabulary::NIE::title()).toString();
    QCOMPARE(title, testEpisode.value(QLatin1String("title")).toString());

    QString synopsis = episodeRes.property(Nepomuk2::Vocabulary::NMM::synopsis()).toString();
    QCOMPARE(synopsis, testEpisode.value(QLatin1String("overview")).toString());

    QString episodeNumber = episodeRes.property(Nepomuk2::Vocabulary::NMM::episodeNumber()).toString();
    QCOMPARE(episodeNumber, testEpisode.value("number").toString());


    QStringList genreList = episodeRes.property(Nepomuk2::Vocabulary::NMM::genre()).toStringList();
    QVERIFY(genreList.size() == testEpisode.value(QLatin1String("genres")).toString().split(QLatin1String(";")).size());
    foreach(const QString &genre, genreList) {
        QVERIFY(testEpisode.value(QLatin1String("genres")).toString().contains(genre));
    }

    QList<Nepomuk2::Resource> directorListRes = episodeRes.property(Nepomuk2::Vocabulary::NMM::director()).toResourceList();
    QVERIFY(directorListRes.size() == testEpisode.value(QLatin1String("director")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &director, directorListRes) {
        QString personName = director.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testEpisode.value(QLatin1String("director")).toString().contains(personName));
    }

    QList<Nepomuk2::Resource> writerListRes = episodeRes.property(Nepomuk2::Vocabulary::NMM::writer()).toResourceList();
    QVERIFY(writerListRes.size() == testEpisode.value(QLatin1String("writer")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &writer, writerListRes) {
        QString personName = writer.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testEpisode.value(QLatin1String("writer")).toString().contains(personName));
    }

    QList<Nepomuk2::Resource> castListRes = episodeRes.property(Nepomuk2::Vocabulary::NMM::actor()).toResourceList();
    QVERIFY(castListRes.size() == testEpisode.value(QLatin1String("actors")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &cast, castListRes) {
        QString personName = cast.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testEpisode.value(QLatin1String("actors")).toString().contains(personName));
    }

    Nepomuk2::Resource seeAlsoRes = episodeRes.property(Soprano::Vocabulary::RDFS::seeAlso()).toResource();
    QString seeAlso = seeAlsoRes.property(Nepomuk2::Vocabulary::NIE::url()).toString();
    QCOMPARE(seeAlso, testEpisode.value(QLatin1String("seealso")).toString());
}

void PipeTvShow::cleanupTestCase()
{
    KComponentData kcn("WebMiner-Unit-Test");
    // remove all data created by this unittest from the nepomuk database again
    KJob *job = Nepomuk2::removeDataByApplication(Nepomuk2::RemoveSubResoures, kcn);

    if(!job->exec()) {
        qWarning() << job->errorString();
        QFAIL("Cleanup did not work");
    }
}

#include "pipetvshow.moc"
