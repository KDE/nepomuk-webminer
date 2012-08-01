/*
 * This file is part of the Nepomuk KDE project.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file "COPYING" for the exact licensing terms.
 */

/*
 * This file has been generated by the Nepomuk Resource class generator.
 * DO NOT EDIT THIS FILE.
 * ANY CHANGES WILL BE LOST.
 */

#ifndef _MUSICPIECE_H_
#define _MUSICPIECE_H_

class QDateTime;
class QDate;
class QTime;

namespace Nepomuk2 {
    class Resource;
    class MusicAlbum;

}

#include <Nepomuk2/Resource>


namespace Nepomuk2 {

    /**
     * Used to assign music-specific properties such a BPM to video 
     * and audio 
     */
    class  MusicPiece : public Resource
    {
    public:
        /**
         * Create a new empty and invalid MusicPiece instance
         */
        MusicPiece();

        /**
         * Default copy constructor
         */
        MusicPiece( const MusicPiece& );
        MusicPiece( const Resource& );

        /**
         * Create a new MusicPiece instance representing the resource
         * referenced by \a uriOrIdentifier.
         */
        MusicPiece( const QString& uriOrIdentifier );

        /**
         * Create a new MusicPiece instance representing the resource
         * referenced by \a uri.
         */
        MusicPiece( const QUrl& uri );
        ~MusicPiece();

        MusicPiece& operator=( const MusicPiece& );

        /**
         * Get property 'musicAlbum'. Album the music belongs to 
         */
        QList<MusicAlbum> musicAlbums() const;

        /**
         * Set property 'musicAlbum'. Album the music belongs to 
         */
        void setMusicAlbums( const QList<MusicAlbum>& value );

        /**
         * Add a value to property 'musicAlbum'. Album the music belongs 
         * to 
         */
        void addMusicAlbum( const MusicAlbum& value );

        /**
         * Get property 'musicAlbum'. Album the music belongs to 
         */
        MusicAlbum musicAlbum() const;

        /**
         * Set property 'musicAlbum'. Album the music belongs to 
         */
        void setMusicAlbum( const MusicAlbum& value );

        /**
         * \return The URI of the property 'musicAlbum'. 
         */
        static QUrl musicAlbumUri();

        /**
         * Get property 'beatsPerMinute'. beats per minute 
         */
        QList<qint64> beatsPerMinutes() const;

        /**
         * Set property 'beatsPerMinute'. beats per minute 
         */
        void setBeatsPerMinutes( const QList<qint64>& value );

        /**
         * Add a value to property 'beatsPerMinute'. beats per minute 
         */
        void addBeatsPerMinute( const qint64& value );

        /**
         * \return The URI of the property 'beatsPerMinute'. 
         */
        static QUrl beatsPerMinuteUri();

        /**
         * Get property 'performer'. Performer 
         */
        QList<Resource> performers() const;

        /**
         * Set property 'performer'. Performer 
         */
        void setPerformers( const QList<Resource>& value );

        /**
         * Add a value to property 'performer'. Performer 
         */
        void addPerformer( const Resource& value );

        /**
         * \return The URI of the property 'performer'. 
         */
        static QUrl performerUri();

        /**
         * Get property 'composer'. Composer 
         */
        QList<Resource> composers() const;

        /**
         * Set property 'composer'. Composer 
         */
        void setComposers( const QList<Resource>& value );

        /**
         * Add a value to property 'composer'. Composer 
         */
        void addComposer( const Resource& value );

        /**
         * \return The URI of the property 'composer'. 
         */
        static QUrl composerUri();

        /**
         * Get property 'lyricist'. Lyricist 
         */
        QList<Resource> lyricists() const;

        /**
         * Set property 'lyricist'. Lyricist 
         */
        void setLyricists( const QList<Resource>& value );

        /**
         * Add a value to property 'lyricist'. Lyricist 
         */
        void addLyricist( const Resource& value );

        /**
         * \return The URI of the property 'lyricist'. 
         */
        static QUrl lyricistUri();

        /**
         * Get property 'trackNumber'. Track number of the music in its 
         * album 
         */
        QList<qint64> trackNumbers() const;

        /**
         * Set property 'trackNumber'. Track number of the music in its 
         * album 
         */
        void setTrackNumbers( const QList<qint64>& value );

        /**
         * Add a value to property 'trackNumber'. Track number of the music 
         * in its album 
         */
        void addTrackNumber( const qint64& value );

        /**
         * Get property 'trackNumber'. Track number of the music in its 
         * album 
         */
        qint64 trackNumber() const;

        /**
         * Set property 'trackNumber'. Track number of the music in its 
         * album 
         */
        void setTrackNumber( const qint64& value );

        /**
         * \return The URI of the property 'trackNumber'. 
         */
        static QUrl trackNumberUri();

        /**
         * Get property 'musicBrainzTrackID'. MusicBrainz track ID 
         */
        QList<qint64> musicBrainzTrackIDs() const;

        /**
         * Set property 'musicBrainzTrackID'. MusicBrainz track ID 
         */
        void setMusicBrainzTrackIDs( const QList<qint64>& value );

        /**
         * Add a value to property 'musicBrainzTrackID'. MusicBrainz 
         * track ID 
         */
        void addMusicBrainzTrackID( const qint64& value );

        /**
         * Get property 'musicBrainzTrackID'. MusicBrainz track ID 
         */
        qint64 musicBrainzTrackID() const;

        /**
         * Set property 'musicBrainzTrackID'. MusicBrainz track ID 
         */
        void setMusicBrainzTrackID( const qint64& value );

        /**
         * \return The URI of the property 'musicBrainzTrackID'. 
         */
        static QUrl musicBrainzTrackIDUri();

        /**
         * Get property 'trackGain'. ReplayGain track gain 
         */
        QList<double> trackGains() const;

        /**
         * Set property 'trackGain'. ReplayGain track gain 
         */
        void setTrackGains( const QList<double>& value );

        /**
         * Add a value to property 'trackGain'. ReplayGain track gain 
         */
        void addTrackGain( const double& value );

        /**
         * Get property 'trackGain'. ReplayGain track gain 
         */
        double trackGain() const;

        /**
         * Set property 'trackGain'. ReplayGain track gain 
         */
        void setTrackGain( const double& value );

        /**
         * \return The URI of the property 'trackGain'. 
         */
        static QUrl trackGainUri();

        /**
         * Get property 'trackPeakGain'. ReplayGain track peak gain 
         */
        QList<double> trackPeakGains() const;

        /**
         * Set property 'trackPeakGain'. ReplayGain track peak gain 
         */
        void setTrackPeakGains( const QList<double>& value );

        /**
         * Add a value to property 'trackPeakGain'. ReplayGain track 
         * peak gain 
         */
        void addTrackPeakGain( const double& value );

        /**
         * Get property 'trackPeakGain'. ReplayGain track peak gain 
         */
        double trackPeakGain() const;

        /**
         * Set property 'trackPeakGain'. ReplayGain track peak gain 
         */
        void setTrackPeakGain( const double& value );

        /**
         * \return The URI of the property 'trackPeakGain'. 
         */
        static QUrl trackPeakGainUri();

        /**
         * Get property 'setNumber'. The part of a set the audio came from. 
         */
        QList<qint64> setNumbers() const;

        /**
         * Set property 'setNumber'. The part of a set the audio came from. 
         */
        void setSetNumbers( const QList<qint64>& value );

        /**
         * Add a value to property 'setNumber'. The part of a set the audio 
         * came from. 
         */
        void addSetNumber( const qint64& value );

        /**
         * Get property 'setNumber'. The part of a set the audio came from. 
         */
        qint64 setNumber() const;

        /**
         * Set property 'setNumber'. The part of a set the audio came from. 
         */
        void setSetNumber( const qint64& value );

        /**
         * \return The URI of the property 'setNumber'. 
         */
        static QUrl setNumberUri();

        /**
         * Retrieve a list of all available MusicPiece resources. This 
         * list consists of all resource of type MusicPiece that are stored 
         * in the local Nepomuk meta data storage and any changes made locally. 
         * Be aware that in some cases this list can get very big. Then it 
         * might be better to use libKNep directly. 
         */
        static QList<MusicPiece> allMusicPieces();


        /**
         * \return The URI of the resource type that is used in MusicPiece instances.
         */
        static QString resourceTypeUri();

    protected:
       MusicPiece( const QString& uri, const QUrl& type );
       MusicPiece( const QUrl& uri, const QUrl& type );
    };
}

#endif
