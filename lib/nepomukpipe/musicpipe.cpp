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

#include "musicpipe.h"

#include <Nepomuk2/SimpleResource>
#include <Nepomuk2/SimpleResourceGraph>
#include <Nepomuk2/DataManagement>
#include <Nepomuk2/StoreResourcesJob>

#include "sro/nmm/musicpiece.h"
#include "sro/nmm/musicalbum.h"
#include "sro/nfo/image.h"
#include "sro/nco/personcontact.h"
#include <Soprano/Vocabulary/NAO>

#include <KDE/KJob>

#include <KDE/KDebug>

using namespace Soprano::Vocabulary;

NepomukMetaDataExtractor::Pipe::MusicPipe::MusicPipe(QObject *parent)
    : NepomukPipe(parent)
{
}

void NepomukMetaDataExtractor::Pipe::MusicPipe::pipeImport(const QVariantMap &musicEntry)
{
    Nepomuk2::SimpleResourceGraph graph;

    // create the  Music Album
    Nepomuk2::NMM::MusicAlbum albumResource;

    QString albumTitle = musicEntry.value(QLatin1String("title")).toString();
    if(!albumTitle.isEmpty()) {
        albumResource.setTitle( albumTitle );
    }

    QString musicBrainz = musicEntry.value(QLatin1String("musicbrainz")).toString();
    if(!musicBrainz.isEmpty()) {
        albumResource.setMusicBrainzAlbumID( musicBrainz );
    }

    /*
    //FIXME: NMM:MusicAlbum is missing the genre parameter for Music albums
    QString genreList = movieEntry.value(QLatin1String("genres")).toString();
    QStringList genres = genreList.split(';');
    QStringList genres2;

    foreach(const QString &genre, genres) {
        QString gen = genre.trimmed();
        if(!gen.isEmpty())
            genres2 << gen;
    }

    album.setGenres( genres2 );
    */

    //FIXME: NMM:MusicAlbum is missing the artist parameter for Music albums
    QString albumArtist = musicEntry.value(QLatin1String("performer")).toString();
//    if(!albumArtist.isEmpty()) {
//        albumResource.setArtist( albumArtist.);
//    }

    if( !musicEntry.value(QLatin1String("trackcount")).isNull()) {
        int trackCount = musicEntry.value(QLatin1String("trackcount")).toInt();
        albumResource.setAlbumTrackCount( trackCount );
    }

    // download cover
    QString albumCoverUrl = musicEntry.value(QLatin1String("cover")).toString();
    KUrl localCoverUrl;
    if( !albumCoverUrl.isEmpty() ) {
        KUrl resourceFolder(musicEntry.value(QLatin1String("resourceuri")).toString() );
        localCoverUrl = downloadBanner( albumCoverUrl,
                                              QString("%1 - %2").arg(albumArtist).arg(albumTitle),
                                              albumArtist,
                                              resourceFolder.directory()
                                              );

        if(!localCoverUrl.isEmpty()) {
            Nepomuk2::NFO::Image banner(localCoverUrl);
            albumResource.addDepiction(banner.uri());
            graph << banner;
        }
    }

    graph << albumResource;

    // now generate all the music pieces
    QVariantList trackList = musicEntry.value(QLatin1String("tracks")).toList();

    foreach( const QVariant &track, trackList) {
        QVariantMap trackInfo = track.toMap();

        kDebug() << "create new music track from url" << trackInfo.value(QLatin1String("resourceuri")).toString();
        KUrl localFile( trackInfo.value(QLatin1String("resourceuri")).toString() );
        Nepomuk2::NMM::MusicPiece trackResource(localFile);

        // Note: Why album cover as artwork of the track? not of the music album?
        // thats how Nepoogle expect it to be
        if(!localCoverUrl.isEmpty()) {
            Nepomuk2::NFO::Image banner(localCoverUrl);
            trackResource.addArtwork( banner.uri() );
        }

        QString title = trackInfo.value(QLatin1String("title")).toString();
        if(!title.isEmpty()) {
            trackResource.setTitle( title );
        }

        if(trackInfo.value(QLatin1String("number")).isValid()) {
            int trackNumber = trackInfo.value(QLatin1String("number")).toInt();
            trackResource.setTrackNumber( trackNumber );
        }

        //BUG: NMM:MusicBrainzTrackID must be string not integer
        //QString musicBrainz = trackInfo.value(QLatin1String("musicbrainz")).toString();
        //trackResource.setMusicBrainzTrackID( musicBrainz );

        // all performer of the track
        // TODO: guest artist, lyricswriter and so on?
        QList<Nepomuk2::NCO::PersonContact> artistList = createPersonContacts( trackInfo.value(QLatin1String("performer")).toString() );
        foreach(const Nepomuk2::NCO::PersonContact &artist, artistList) {
            graph << artist;
            trackResource.addPerformer( artist.uri() );
            trackResource.addProperty(NAO::hasSubResource(), artist.uri());
        }

        // all genres of the track
        QString genreList = trackInfo.value(QLatin1String("genres")).toString();
        QStringList genres = genreList.split(';');
        QStringList genres2;

        foreach(const QString &genre, genres) {
            QString gen = genre.trimmed();
            if(!gen.isEmpty())
                genres2 << gen;
        }

        if(!genres2.isEmpty()) {
            trackResource.setGenres( genres2 );
        }

        // release date
        QDateTime releaseDate = createDateTime( trackInfo.value(QLatin1String("releasedate")).toString() );
        if(releaseDate.isValid()) {
            trackResource.setReleaseDate( releaseDate );
        }

        // now connect music album and track
        trackResource.setMusicAlbum( albumResource.uri() );

        graph << trackResource;
    }

    Nepomuk2::storeResources(graph, Nepomuk2::IdentifyNew, Nepomuk2::OverwriteProperties);
}
