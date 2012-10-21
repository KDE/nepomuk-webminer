#!/usr.bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------
# @see https://github.com/dbr/tvdb_api need 1.6.2
#------------------------------------------------------------------------------

import re
import json

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    import tvdb_api

except:
    isAvailable = False
    errorMsg = "The tvdb python module 1.6.2 needs to be installed. See https://github.com/dbr/tvdb_api"


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
    return dict( name = 'The TVDb',
                 homepage = 'http://thetvdb.com',
                 icon = 'tvdb.png',
                 identifier = 'tvdb',
#                 urlregex = ['http://thetvdb.com/\\?tab=episode&seriesid=(\\d+)&seasonid=(\\d+)&id=(\\d+).*', 'http://thetvdb.com/\\?tab=season&seriesid=(\\d+)&seasonid=(\\d+).*', 'http://thetvdb.com/\\?tab=series&id=(\\d+).*'],
                 urlregex = ['http://thetvdb.com/\\?tab=episode&seriesid=(\\d+)&seasonid=(\\d+)&id=(\\d+).*'],
                 resource = ['tvshow'],
                 description = 'An open database for television fans',
                 author = 'Joerg Ehrichs',
                 email = 'joerg.ehrichs@gmx.de',
                 isAvailable = isAvailable,
                 errorMsg = errorMsg)

#------------------------------------------------------------------------------
# starts a search query
#
# resourcetype = what kind of resource we want to search for. Either 'tvshow' or 'person'
# parameters   = the actual search parameters
#                'title'   = show title or person name
#                'season'  = the season number.
#                'episode' = the episode number.
#
# If season Number and episode number is missing, return all episodes of the show. + all seasons + the show object
# If only episode number is missing return all episodes of the season + season entry
# if only season number is missing return all episodes with correct number for any season + show object
#
# @todo add resourcetype 'series', 'season' to retrieve overview info of all episodes
#       good for bulk download/select a folder and then fetch all episode info
#
def searchItems( resourcetype, parameters ):

    title = ''
    season = ''
    episode = ''
    showtitle = ''
    if(parameters.has_key('title') ):
        title = parameters['title']
    if(parameters.has_key('season') ):
        season = parameters['season']
    if(parameters.has_key('episode') ):
        episode = parameters['episode']
    if(parameters.has_key('showtitle') ):
        showtitle = parameters['showtitle']

    searchResults = []
    t = tvdb_api.Tvdb()


    try:
        if showtitle and season and episode:
            episodeResult = t[showtitle.decode('utf')][int(season)][int(episode)] # the utf part is to prevent flipping out when filename has unicode
            searchResults.append( getEpisodeDetails(episodeResult) )
        
        elif showtitle and season:
            seasonResult = t[showtitle][int(season)]

            for episodeNumber in seasonResult:
                episodeResult = seasonResult[episodeNumber]
                searchResults.append( getEpisodeDetails(episodeResult) )
        
        elif showtitle:
            showResult = t[showtitle]

            for seasonNumber in showResult:
                seasonResult = showResult[seasonNumber]

                for episodeNumber in seasonResult:
                    episodeResult = seasonResult[episodeNumber]
                    searchResults.append( getEpisodeDetails(episodeResult) )
        else:
            WebExtractor.error('no useful search parameters defined')

        WebExtractor.searchResults( searchResults )
 
    except (tvdb_api.tvdb_shownotfound, tvdb_api.tvdb_seasonnotfound) as err:
    #BUG: find out why tvdb_api.tvdb_episodenotfound crashes KROSS:Python 
    #except (tvdb_api.tvdb_shownotfound, tvdb_api.tvdb_seasonnotfound, tvdb_api.tvdb_episodenotfound) as err: 
        WebExtractor.log( str(err) )
        WebExtractor.searchResults( searchResults )

    except Exception as err:
        WebExtractor.error("Script error: \n" + str(err))
        WebExtractor.searchResults( searchResults )
    
#------------------------------------------------------------------------------
# helper function to fill the search result dictionary
#
def getEpisodeDetails(episode):

    if not episode['overview']: # sometimes this is not available. Don't panic don't crash!
        episode['overview'] = ""
    detailString =  episode.season.show['seriesname'] + ' (S' + episode['seasonnumber'] + 'E' + episode['episodenumber'] + ') : ' + episode['overview']
    fullUrl = 'http://thetvdb.com/?tab=episode&seriesid='  + episode['seriesid'] + '&seasonid=' + episode['seasonid'] + '&id=' + episode['id']

    return dict(
                title = episode['episodename'],
                details = detailString,
                url = fullUrl
                )

#------------------------------------------------------------------------------
# extracts all information from the given url
#
# What we need to return is a dictionary containing all necessary information.
# TvShows are a bit different from other searches, as they are connected in show/season/episode way
# Thats why a more complicated result is necessary.
#
# Depending on the given url we either extract:
# 1) only one episode
# 2) all episodes in 1 season
# 3) all seasons in the show and all episodes in each season
#
# In most cases only the first option is used, but when someone wants to "bulk-fetch" a complete show at once
# This structure allows one to do it in one go rather than quering the plugin over and over again for each episode.
# How the results are processed is up to the ui using it. The structure of the results is in each case the same
#
# Look at the TvShow pipe from the metadata extarctor for more details
#
# The options are not used at the moment. They will ofer some more details like, language, banners and so on
# @todo maybe allow person fetching in more detail rather than the name itself
#       could be added as option, otherwise we have to fetch the data for each person again
#       this of course needs a change in the returned QVariantMap that needs to be done.
#
def extractItemFromUri( url, options ):

    # @todo implement search option 2 and 3 currently only 1 episode is returned
    exp = re.compile(r'seriesid=(\d+)&seasonid=(\d+)&id=(\d+)')
    match = exp.search(url)

    if not match:
        WebExtractor.error('Error parsing url: ' + url)
        return

    seriesid = match.group(1)
    seasonid = match.group(2)
    episodeid = match.group(3)

    WebExtractor.log('fetch show :' + seriesid)
    WebExtractor.log('fetch season :' + seasonid)
    WebExtractor.log('fetch episode :' + episodeid)

    t = tvdb_api.Tvdb(banners = True, actors = True)
    showItem = t[int(seriesid)]
    episodeItemList = showItem.search(episodeid, key = 'id')
    episodeItem = episodeItemList[0]

    if not episodeItem['gueststars']:
        allActors = showItem['actors']
    elif not showItem['actors']:
        allActors = episodeItem['gueststars']
    else:
        allActors = episodeItem['gueststars'] + showItem['actors']

    actorList = allActors.replace('|',';').strip(';')

    if episodeItem['writer']:
        writerList = episodeItem['writer'].replace('|',';').strip(';')
    else:
        writerList= []
    if episodeItem['director']:
        directorList = episodeItem['director'].replace('|',';').strip(';')
    else:
        directorList= []
    if showItem['genre']:
        genreList = showItem['genre'].replace('|',';').strip(';')
    else:
        genreList= []

    #------
    # season posters
    seasonPoster = ""
    seasonBanner = ""
    seasonPosterList = {}
    seasonBannerList = {}

    if 'season' in showItem['_banners']:
        if 'season' in showItem['_banners']['season']:
            seasonPosterList = showItem['_banners']['season']['season']
        if 'seasonwide' in showItem['_banners']['season']:
            seasonBannerList = showItem['_banners']['season']['seasonwide']

    #TODO: don't assume english language, take system locale?
    if seasonPosterList is not None:
        for key in seasonPosterList.iterkeys():
            if (seasonPosterList[key]['language'] == "en") and (seasonPosterList[key]['season'] == str(episodeItem['seasonnumber'])):
                seasonPoster = seasonPosterList[key]['_bannerpath']
                break

    if seasonBannerList is not None:
        for key in seasonBannerList.iterkeys():
            if (seasonBannerList[key]['language'] == "en") and (seasonBannerList[key]['season'] == str(episodeItem['seasonnumber'])):
                seasonBanner = seasonBannerList[key]['_bannerpath']
                break

    episodeDict = dict (
                        title = episodeItem['episodename'],
                        overview = episodeItem['overview'],
                        number = episodeItem['episodenumber'],
                        firstaired = episodeItem['firstaired'],
                        director = directorList,
                        writer = writerList,
                        actors = actorList,
                        genre = genreList,
                        banner = episodeItem['filename']
                        )

    seasonDict = dict (
                       episodes = [episodeDict],
                       number = episodeItem['seasonnumber'],
                       genre = genreList,
                       banner = seasonBanner,
                       poster = seasonPoster
                       )

    seriesDict = dict (
                        seasons = [seasonDict],
                        title = showItem['seriesname'],
                        overview = showItem['overview'],
                        banner = showItem['banner'],
                        poster = showItem['poster'],
                        genre = genreList
                       )

    # this is a workaround, because multiple nested dictionaries cause some problems with the names of the keys
    #WebExtractor.itemResults( 'tvshow', seriesDict )
    data_string = json.dumps(seriesDict)
    WebExtractor.itemResultsJSON( 'tvshow', data_string )

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])
