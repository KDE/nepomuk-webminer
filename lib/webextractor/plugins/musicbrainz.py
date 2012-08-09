#!/usr.bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------
# @see http://musicbrainz.org/doc/python-musicbrainz2 need 1.7.4
#------------------------------------------------------------------------------

#TODO: Seems outdated and the replacement has only an announcement from 2010 also python-musicbrainz-ngs gives a lot of errors
import re
import json

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    from musicbrainz2.webservice import Query, TrackFilter, WebServiceError
    import musicbrainz2.utils as u
    import musicbrainz2.webservice as mbws

except:
    isAvailable = False
    errorMsg = "The musicbrainz2 python module 1.7.4 needs to be installed. See http://musicbrainz.org/doc/python-musicbrainz2"


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

    title = parameters['title']
    track = parameters['track']
    artist = parameters['author']
    album = parameters['album']

    searchResults = []
    q = Query()

    try:
        f = TrackFilter(title=title, artistName=artist)
        results = q.getTracks(f)

        # iterate over all available results
        for result in results:
            track = result.track

            #TODO check esult.score? return nothing below 50 ?

            # create the search result dictionary
            types = ''.join(track.releases[0].types)

            entryDict = dict(
                            title = str(track.artist.name) + ' - ' + str(track.title),
                            # in MusicBrainz every track is only related to 1 release at the moment, so we can safely take the first one always
                            details = track.releases[0].title + ' (' + u.getReleaseTypeName(types) + ')',
                            url = track.id
                            )

            searchResults.append(entryDict)

        WebExtractor.searchResults( searchResults )

    except WebServiceError as err:
        WebExtractor.log( str(err) )
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
    mbid = match.group(2)

    WebExtractor.log('fetch :' + extractType + ' with id :' + mbid)

    if extractType == 'release':
        #TODO extract all tracks from this relese
        WebExtractor.error('Full Album extraction not implemented: ')
        return
    else:
        # extract just the single track
        q = Query()
        try:
            inc = mbws.TrackIncludes(artist=True, releases=True, artistRelations=False, tags=True)
            track = q.getTrackById(mbid, inc)

            trackDict = dict (
                                title = track.title,
                                performer = track.artist.name,
                                number = track.releases[0].getTracksOffset(),
                                genre = ';'.join(tag.value for tag in track.tags),
                                musicbrainz = track.id
                            )

            albumArtist = ""
            if track.releases[0].artist is not None:
                albumArtist = track.releases[0].artist.name

            albumDict  = dict (
                                title = track.releases[0].title,
                                trackCount = track.releases[0].getTracksCount(), #returns always nothing?
                                performer = albumArtist,
                                genre = ';'.join(tag.value for tag in track.tags),
                                tracks = [trackDict],
                                musicbrainz = track.releases[0].id
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
