#!/usr.bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------
# @see https://github.com/alastair/python-musicbrainz-ngs/ need current master (0.3dev)
#------------------------------------------------------------------------------

import re
import json

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    import musicbrainzngs
    from musicbrainzngs import mbxml

except:
    isAvailable = False
    errorMsg = "The python-musicbrainz-ngs python module git master needs to be installed. See https://github.com/alastair/python-musicbrainz-ngs/"

# general script settings
musicbrainzngs.set_rate_limit(False) # set on again to reduce server load?
musicbrainzngs.set_useragent(
    "python-musicbrainz-ngs-example",
    "0.1",
    "https://github.com/alastair/python-musicbrainz-ngs/",
)

#------------------------------------------------------------------------------
# Module options
#
# Must have:
# name          = Long name of the service
# icon          = download the favicon and put is as png in the folder, icon is than name.png
# identifier    = short unique identifier for the service, no special chars, whitespaces
# resource      = list of resources this plugin can fetch for example ['tvshow', 'movie']
# urlregex      = a list of regular expressions that fits a url to this plugin
# isAvailable   = True if plugin can be used, False if some dependencies are missing
# errorMsg      = Detailed messsage with the error
#
# Can have:
# description   = Description to add some more information about the service
# author        = plugin author
# email         = plugin author email
#
def info():
    return dict( name = 'MusicBrainz',
                 icon = 'musicbrainz.png',
                 identifier = 'musicbrainz',
                 #urlregex = ['http://musicbrainz.org/release/.*', 'http://musicbrainz.org/recording.*'], // todo add /release to fetch all musicPiece in an musicAlbum
                 urlregex = ['http://musicbrainz.org/recording.*'],
                 resource = ['music'],
                 description = 'MusicBrainz is an open music encyclopedia that collects music metadata and makes it available to the public.',
                 author = 'Joerg Ehrichs',
                 email = 'joerg.ehrichs@gmx.de',
                 isAvailable = isAvailable,
                 errorMsg = errorMsg)


#------------------------------------------------------------------------------
# starts a search query
#
# resourcetype = what kind of resource we want to search for. Here only 'music'
# parameters   = the actual search parameters
#                'title'   = name of the track
#                'track'  = the tracknumer
#                'artist' = the artist of the track
#                'album' = the album name where this track is on
#
def searchItems( resourcetype, parameters ):

    luceneQuery = ""
    title = parameters['title']
    if title is not None and title != "":
        luceneQuery = 'recording:"' + title + '" '

    track = parameters['track']
    if track is not None and track != "":
        luceneQuery = 'tnum:' + track + ' '

    artist = parameters['author']
    if artist is not None and artist != "":
        luceneQuery = 'artistname:"' + artist + '" '

    album = parameters['album']
    if album is not None and album != "":
        luceneQuery = 'release:"' + album + '" '

    searchResults = []

    try:
        result = musicbrainzngs.search_recordings(luceneQuery, strict=True)

        if not result['recording-list']:
            WebExtractor.log( 'No results found for: ' + luceneQuery )
            WebExtractor.searchResults( searchResults )


        # iterate over all available results
        for recording in result['recording-list']:

            if int(recording['ext:score']) < 50:
                continue

            detailString =""
            albumRelese = recording['release-list'][0]
            if albumRelese is not None:
                detailString = albumRelese['title']

            if 'date' in albumRelese:
                detailString = detailString + ' (' + albumRelese['date'] + ')'

            if 'type' in albumRelese['release-group']:
                detailString = detailString + ' - ' + albumRelese['release-group']['type']

            entryDict = dict (
                              title = recording["artist-credit-phrase"] + ' - ' + recording["title"],
                              details = detailString,
                              url = 'http://musicbrainz.org/recording/' + recording['id']
                             )

            searchResults.append(entryDict)

        WebExtractor.searchResults( searchResults )

    except Exception as err:
        WebExtractor.error("Script error: \n" + str(err))

#------------------------------------------------------------------------------
# extracts all information from the given url
#
# returns a dictionary of the album containing a dictionary of one or all given tracks
#
# In most cases this will return only 1 track (as used by metadata extractor)
# but one could also extract a complete album with all tracks
#
def extractItemFromUri( url, options ):

    exp = re.compile(r'/(\w+)/(.*)')
    match = exp.search(url)

    if not match:
        WebExtractor.error('Error parsing url: ' + url)
        return

    extractType = match.group(1)
    puid = match.group(2)

    WebExtractor.log('fetch :' + extractType + ' with id :' + puid)

    if extractType == 'release':
        #TODO extract all tracks from this relese
        WebExtractor.error('Full Album extraction not implemented: ')
        return

        try:
            musicbrainzngs.get_release_by_id

        except Exception as err:
            WebExtractor.error("Script error: \n" + str(err))

    else:
        # extract just the single track

        try:
            # get_recording_by_id need authorization :/
            result = musicbrainzngs.get_recording_by_id(puid, includes=["artists", "artist-credits", "releases", "media", "tags"])

            recording = result['recording']

            trackNumber = ""
            releaseType = ""
            date = ""
            albumName = ""
            albumArtist = ""
            albumId = ""
            genre = ""
            trackPerformer = ""

            for person in recording['artist-credit']:
                if 'artist' in person:
                    if trackPerformer != "":
                        trackPerformer = trackPerformer + '; '

                    trackPerformer = trackPerformer + person['artist']['name']

            if 'tag-list' in recording:
                genre = ';'.join(tag['name'] for tag in recording['tag-list'])

            if 'release-list' in recording:
                albumRelese = recording['release-list'][0]
                if albumRelese is not None:
                    albumName = albumRelese['title']
                    albumId = albumRelese['id']
                    #albumArtist = albumRelese['artist-credit-phrase']
                    for person in albumRelese['artist-credit']:
                        if 'artist' in person:
                            if albumArtist != "":
                                albumArtist = albumArtist + '; '
                            albumArtist = albumArtist + person['artist']['name']

                if 'date' in albumRelese:
                    date = albumRelese['date']

                #if 'type' in albumRelese['release-group']:
                #    releaseType = albumRelese['release-group']['type']

                if albumRelese['medium-list'][0] is not None:
                    if 'track-list' in albumRelese['medium-list'][0]:
                        trackNumber = albumRelese['medium-list'][0]['track-list'][0]['number']

            trackDict = dict (
                                title = recording['title'],
                                performer = trackPerformer,#recording['artist-credit-phrase'],
                                number = trackNumber,
                                releasedate = date,
                                genre = genre,
                                musicbrainz = recording['id']
                            )

            albumDict  = dict (
                                title = albumName,
                                #trackCount = track.releases[0].getTracksCount(),
                                performer = albumArtist,
                                #genre = ';'.join(tag.value for tag in track.tags),
                                musicbrainz = albumId,
                                #releasetype = releaseType,
                                tracks = [trackDict]
                            )
            # this is a workaround, because multiple nested dictionaries cause some problems with the names of the keys
            #WebExtractor.itemResults( 'music', trackDict )
            data_string = json.dumps(albumDict)
            WebExtractor.itemResultsJSON( 'music', data_string )

        except Exception as err:
            WebExtractor.error("Script error: \n" + str(err))

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])
