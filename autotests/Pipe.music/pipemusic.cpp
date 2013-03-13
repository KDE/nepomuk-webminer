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

#include "nepomukpipe/musicpipe.h"
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
 * @file pipemusic.cpp
 *
 * @test UnitTest for the NepomukPipe => Music.
 *       checks: imports a music Ppiece from QVariantList, checks if Nepomuk data is ok
 */
class PipeMusic: public QObject
{
    Q_OBJECT

private slots:

    void importMusicPiece();
    void checkImportedMusicPiece();

    void cleanupTestCase();

private:
    QVariantMap testTrack;
    QVariantMap testAlbum;
};

QTEST_KDEMAIN_CORE(PipeMusic)

void PipeMusic::importMusicPiece()
{
    // Define the music piece that will be imported
    testTrack.insert(QLatin1String("resourceuri"), QString());
    testTrack.insert(QLatin1String("title"), QLatin1String("UNITTEST-MusicPiece"));
    testTrack.insert(QLatin1String("number"), QLatin1String("5"));
    testTrack.insert(QLatin1String("releasedate"), QLatin1String("2013"));
    testTrack.insert(QLatin1String("genres"), QLatin1String("Unit-Test1;Unit-Test2;Unit-Test3"));
    testTrack.insert(QLatin1String("performer"), QLatin1String("Unit-Test Performer1;Unit-Test Performer2;Unit-Test Performer3"));
    testTrack.insert(QLatin1String("seealso"), QLatin1String("http://unit-test-see.also"));;
    testTrack.insert(QLatin1String("lyrics"), QLatin1String("UNITTEST lyrics"));

    testAlbum.insert(QLatin1String("title"), QLatin1String("UNITTEST-MusicAlbum"));
    testAlbum.insert(QLatin1String("trackcount"), QLatin1String("10"));
    testAlbum.insert(QLatin1String("genre"), QLatin1String("Unit-Test1;Unit-Test2;Unit-Test3"));
    testAlbum.insert(QLatin1String("performer"), QLatin1String("Unit-Test Performer1;Unit-Test Performer2;Unit-Test Performer3"));

    testAlbum.insert(QLatin1String("tracks"), QVariantList() << testTrack);

    NepomukWebMiner::Pipe::MusicPipe mp;
    mp.overrideComponentName(QLatin1String("WebMiner-Unit-Test"));

    bool importSucessful = mp.import(testAlbum);

    QVERIFY2( importSucessful == true, "Music Piece import was not possible");
}

void PipeMusic::checkImportedMusicPiece()
{

    //now fetch the nepomuk database again and see if the movie and all data are imported correctly
    QString query = QString::fromLatin1("select ?r where {"
                                        "?r a nmm:MusicPiece ."
                                        "?r nie:title ?t ."
                                        "FILTER regex(?t, \"UNITTEST-MusicPiece\") ."
                                        "}");
    Soprano::Model* model = Nepomuk2::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );

    Nepomuk2::Resource trackResource;
    //we check only the first result
    if( !it.next()) {
        QFAIL("No movie with the title \"UNITTEST-MusicPiece\" found");
    }
    else {
        Soprano::BindingSet p = it.current();
        trackResource = Nepomuk2::Resource::fromResourceUri(p.value("r").toString());
    }

    if( !trackResource.isValid() || !trackResource.exists()) {
        QFAIL("Found Music Track Resource is not valid");
    }

    QString track = trackResource.property(Nepomuk2::Vocabulary::NMM::trackNumber()).toString();
    QCOMPARE(track, testTrack.value(QLatin1String("number")).toString());

    QString releaseDate = trackResource.property(Nepomuk2::Vocabulary::NMM::releaseDate()).toString();
    QVERIFY(releaseDate.startsWith(testTrack.value(QLatin1String("releasedate")).toString()));

    QStringList genreList = trackResource.property(Nepomuk2::Vocabulary::NMM::genre()).toStringList();
    QVERIFY(genreList.size() == testTrack.value(QLatin1String("genres")).toString().split(QLatin1String(";")).size());
    foreach(const QString &genre, genreList) {
        QVERIFY(testTrack.value(QLatin1String("genres")).toString().contains(genre));
    }

    QList<Nepomuk2::Resource> performerListRes = trackResource.property(Nepomuk2::Vocabulary::NMM::performer()).toResourceList();
    QVERIFY(performerListRes.size() == testTrack.value(QLatin1String("performer")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &performer, performerListRes) {
        QString personName = performer.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testTrack.value(QLatin1String("performer")).toString().contains(personName));
    }

    Nepomuk2::Resource seeAlsoRes = trackResource.property(Soprano::Vocabulary::RDFS::seeAlso()).toResource();
    QString seeAlso = seeAlsoRes.property(Nepomuk2::Vocabulary::NIE::url()).toString();
    QCOMPARE(seeAlso, testTrack.value(QLatin1String("seealso")).toString());

    QString lyrics = trackResource.property(Nepomuk2::Vocabulary::NIE::plainTextContent()).toString();
    QCOMPARE(lyrics, testTrack.value(QLatin1String("lyrics")).toString());

    // get the music album of the Track
    Nepomuk2::Resource albumResource = trackResource.property(Nepomuk2::Vocabulary::NMM::musicAlbum()).toResource();

    if(!albumResource.isValid() || !albumResource.exists()) {
        QFAIL("Found Music Album Resource is not valid");
    }

    QString albumTitle = albumResource.property(Nepomuk2::Vocabulary::NIE::title()).toString();
    QCOMPARE(albumTitle, testAlbum.value(QLatin1String("title")).toString());

    QString trackcount = albumResource.property(Nepomuk2::Vocabulary::NMM::albumTrackCount()).toString();
    QCOMPARE(trackcount, testAlbum.value(QLatin1String("trackcount")).toString());

/* //BUG: Album genres and album preformer not supported yet
    QStringList albumGenreList = albumResource.property(Nepomuk2::Vocabulary::NMM::genre()).toStringList();
    QVERIFY(albumGenreList.size() == testAlbum.value(QLatin1String("genre")).toString().split(QLatin1String(";")).size());
    foreach(const QString &genre, albumGenreList) {
        QVERIFY(testAlbum.value(QLatin1String("genre")).toString().contains(genre));
    }

    QList<Nepomuk2::Resource> albumPerformerListRes = albumResource.property(Nepomuk2::Vocabulary::NMM::performer()).toResourceList();
    QVERIFY(albumPerformerListRes.size() == testAlbum.value(QLatin1String("performer")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &performer, albumPerformerListRes) {
        QString personName = performer.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testAlbum.value(QLatin1String("performer")).toString().contains(personName));
    }
*/
}

void PipeMusic::cleanupTestCase()
{
    KComponentData kcn("WebMiner-Unit-Test");
    // remove all data created by this unittest from the nepomuk database again
    KJob *job = Nepomuk2::removeDataByApplication(Nepomuk2::RemoveSubResoures, kcn);

    if(!job->exec()) {
        qWarning() << job->errorString();
        QFAIL("Cleanup did not work");
    }
}

#include "pipemusic.moc"
