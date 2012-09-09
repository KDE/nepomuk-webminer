#!/usr.bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------
# @see https://github.com/dbr/tvdb_api need 1.6.2
#------------------------------------------------------------------------------

import re
import json
import urllib2
import PyKDE4.kdecore as kdecore
import PyQt4.QtCore as QtCore

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    import tvdb_api

except:
    isAvailable = False
    errorMsg = "The tvdb python module 1.6.2 needs to be installed. See https://github.com/dbr/tvdb_api"

try:
    import requests
except:
    isAvailable = False
    errorMsg = "The python 'requests' module is a requirement. See http://docs.python-requests.org/en/latest/user/install/"

confFileName = "tvdbrc"

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
    return dict( name = 'The TVDb (Augmented with MyAnimeList)',
                 homepage = 'http://thetvdb.com',
                 icon = 'tvdb.png',
                 identifier = 'tvdbmal',
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

    t = tvdb_api.Tvdb()

    searchResults = trySearch(parameters, t)
    WebExtractor.searchResults( searchResults )

#------------------------------------------------------------------------------
# put the logic code here into a helper so we can retry recursively (for cases where we have aliases)

def trySearch(parameters, t, recurse = True):
    title = parameters['title']
    episode = parameters['episode']
    WebExtractor.log("looking for configured aliases")
    parameters['showtitle'], parameters['season'] = getConfiguredAlias(parameters['showtitle'], parameters['season'])
    showtitle = parameters['showtitle']
    season = parameters['season']
    WebExtractor.log("Showtitle is " + showtitle + " and season is " + season)

    searchResults = []
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

    except (tvdb_api.tvdb_shownotfound, tvdb_api.tvdb_seasonnotfound, tvdb_api.tvdb_episodenotfound) as err:
        if recurse:
            # we try to find aliases
            WebExtractor.log("finding aliases")
            aliases = get_other_titles(showtitle)
            if aliases:
                for alias in aliases:
                    if alias == parameters['showtitle']:
                        continue # we tried this before
                    WebExtractor.log("trying " + alias)
                    newparameters = parameters
                    newparameters['showtitle'] = alias
                    searchResults = trySearch(newparameters, t, False) # try the alias
                    if searchResults:
                        return searchResults
            WebExtractor.log( str(err) )
            return searchResults # we genuinely cannot find nuts :C
        else:
            return searchResults

    except Exception as err:
        WebExtractor.error("Script error: \n" + str(err))

    return searchResults

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
# helper function that checks a config file for configured alias overrides
#
def getConfiguredAlias(showtitle, showseason):
    conf = kdecore.KConfig(confFileName)
    confgroup = kdecore.KConfigGroup(conf, "aliases")
    rawdata = confgroup.readEntry(showtitle, QtCore.QStringList()).toStringList()
    if len(rawdata) > 1:
        if rawdata[1] == "-":
            return str(rawdata[0]), showseason
        else:
            return str(rawdata[0]), str(rawdata[1])
    return showtitle, showseason



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

#------------------------------------------------------------------------------
# These are helper functions that attempt to find anime showtitle aliases using MyAnimeList
# get_other_titles is the main function of interest here, and simply returns an array of aliases
# for the supplied query, if found.
# Much of this code is adapted from malconstrict, a MAL api python wrapper

malapiurl = 'http://mal-api.com'

def raw_search_anime(query):
    response = requests.get(malapiurl + '/anime/search?q=' + urllib2.quote(query))
    if response.status_code != 200:
        return None
    return response.content

def raw_get_anime_details(anime_id):
    response = requests.get(
        malapiurl + '/anime/' + str(anime_id) + '?mine=0')

    if response.status_code == 404:
        return None
    return response.content

# TODO: search results aren't sorted by relevance, need to workaround that
def get_other_titles(query):
    entries = json.loads(raw_search_anime(query))
    if len(entries) < 1:
        return None
    details = json.loads(raw_get_anime_details(entries[0]['id']))
    result = [details['title']]
    if 'english' in details['other_titles']:
        for title in details['other_titles']['english']:
            result.append(title)
    if 'synonyms' in details['other_titles']:
        for title in details['other_titles']['synonyms']:
            result.append(title)
    return result

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])
