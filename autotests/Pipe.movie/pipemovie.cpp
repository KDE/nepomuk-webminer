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

#include "nepomukpipe/moviepipe.h"
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
 * @file pipemovie.cpp
 *
 * @test UnitTest for the NepomukPipe => Movie.
 *       checks: imports a movie from QVariantList, checks if Nepomuk data is ok
 */
class PipeMovie: public QObject
{
    Q_OBJECT

private slots:

    void importMovie();
    void checkImportedMovie();

    void cleanupTestCase();

private:
    QVariantMap testMovie;
};

QTEST_KDEMAIN_CORE(PipeMovie)

void PipeMovie::importMovie()
{
    // Define the movie that will be imported
    testMovie.insert(QLatin1String("resourceuri"), QString());
    testMovie.insert(QLatin1String("title"), QLatin1String("UNITTEST-Movie"));
    testMovie.insert(QLatin1String("plot"), QLatin1String("UNITTEST-Movie the long plot"));
    testMovie.insert(QLatin1String("year"), QLatin1String("2013"));
    testMovie.insert(QLatin1String("genre"), QLatin1String("Unit-Test1;Unit-Test2;Unit-Test3"));
    testMovie.insert(QLatin1String("director"), QLatin1String("Unit-Test Director1;Unit-Test Director2;Unit Test-Director3"));
    testMovie.insert(QLatin1String("writer"), QLatin1String("Unit-Test Writer1;Unit-Test Writer2;Unit-Test Writer3"));
    testMovie.insert(QLatin1String("cast"), QLatin1String("Unit-Test Cast1;Unit-Test Cast2;Unit-Test cast3"));
    //testMovie.insert(QLatin1String("poster"), QString()); // would donwload a poster, we do not test this
    testMovie.insert(QLatin1String("seealso"), QLatin1String("http://unit-test-see.also"));

    NepomukWebMiner::Pipe::MoviePipe mp;
    mp.overrideComponentName(QLatin1String("WebMiner-Unit-Test"));

    bool importSucessful = mp.import(testMovie);

    QVERIFY2( importSucessful == true, "Movie import was not possible");
}

void PipeMovie::checkImportedMovie()
{
    //now fetch the nepomuk database again and see if the movie and all data are imported correctly
    QString query = QString::fromLatin1("select ?r where {"
                                        "?r a nmm:Movie ."
                                        "?r nie:title ?t ."
                                        "FILTER regex(?t, \"UNITTEST-Movie\") ."
                                        "}");
    Soprano::Model* model = Nepomuk2::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );

    Nepomuk2::Resource movieResource;
    //we check only the first result
    if( !it.next()) {
        QFAIL("No movie with the title \"UNITTEST-Movie\" found");
    }
    else {
        Soprano::BindingSet p = it.current();
        movieResource = Nepomuk2::Resource::fromResourceUri(p.value("r").toString());
    }

    if( !movieResource.isValid() || !movieResource.exists()) {
        QFAIL("Found Movie Resource is not valid");
    }

    QString plot = movieResource.property(Nepomuk2::Vocabulary::NMM::synopsis()).toString();
    QCOMPARE(plot, testMovie.value(QLatin1String("plot")).toString());

    QString releaseDate = movieResource.property(Nepomuk2::Vocabulary::NMM::releaseDate()).toString();
    QVERIFY(releaseDate.startsWith(testMovie.value(QLatin1String("year")).toString()));

    QStringList genreList = movieResource.property(Nepomuk2::Vocabulary::NMM::genre()).toStringList();
    QVERIFY(genreList.size() == testMovie.value(QLatin1String("genre")).toString().split(QLatin1String(";")).size());
    foreach(const QString &genre, genreList) {
        QVERIFY(testMovie.value(QLatin1String("genre")).toString().contains(genre));
    }

    QList<Nepomuk2::Resource> directorListRes = movieResource.property(Nepomuk2::Vocabulary::NMM::director()).toResourceList();
    QVERIFY(directorListRes.size() == testMovie.value(QLatin1String("director")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &director, directorListRes) {
        QString personName = director.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testMovie.value(QLatin1String("director")).toString().contains(personName));
    }

    QList<Nepomuk2::Resource> writerListRes = movieResource.property(Nepomuk2::Vocabulary::NMM::writer()).toResourceList();
    QVERIFY(writerListRes.size() == testMovie.value(QLatin1String("writer")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &writer, writerListRes) {
        QString personName = writer.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testMovie.value(QLatin1String("writer")).toString().contains(personName));
    }

    QList<Nepomuk2::Resource> castListRes = movieResource.property(Nepomuk2::Vocabulary::NMM::actor()).toResourceList();
    QVERIFY(castListRes.size() == testMovie.value(QLatin1String("cast")).toString().split(QLatin1String(";")).size());
    foreach(const Nepomuk2::Resource &cast, castListRes) {
        QString personName = cast.property(Nepomuk2::Vocabulary::NCO::fullname()).toString();
        QVERIFY(testMovie.value(QLatin1String("cast")).toString().contains(personName));
    }

    Nepomuk2::Resource seeAlsoRes = movieResource.property(Soprano::Vocabulary::RDFS::seeAlso()).toResource();
    QString seeAlso = seeAlsoRes.property(Nepomuk2::Vocabulary::NIE::url()).toString();
    QCOMPARE(seeAlso, testMovie.value(QLatin1String("seealso")).toString());
}

void PipeMovie::cleanupTestCase()
{
    KComponentData kcn("WebMiner-Unit-Test");
    // remove all data created by this unittest from the nepomuk database again
    KJob *job = Nepomuk2::removeDataByApplication(Nepomuk2::RemoveSubResoures, kcn);

    if(!job->exec()) {
        qWarning() << job->errorString();
        QFAIL("Cleanup did not work");
    }
}

#include "pipemovie.moc"
