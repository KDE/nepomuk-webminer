#!/usr/bin/env kross

#------------------------------------------------------------------------------
# @todo replace IMDb python by a different script with better search capabilities
#------------------------------------------------------------------------------

import re

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    from imdb import IMDb

except:
    isAvailable = False
    errorMsg = "The IMDbPY 4.9 module is needed. See http://imdbpy.sourceforge.net/"


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
    return dict( name = 'The Internet Movie Database',
                 icon = 'imdb.png',
                 identifier = 'imdbmovies',
                 urlregex = ['http://www.imdb.com/title/tt'],
                 resource = ['movie'],
                 #resource = ['movie','tvshow', 'person'],
                 description = 'some description',
                 author = 'Joerg Ehrichs',
                 email = 'some@mail.com',
                 isAvailable = isAvailable,
                 errorMsg = errorMsg)

#------------------------------------------------------------------------------
# starts a search query
#
# build the query url from the given parameters
#
# currently supported keys are:
# title, yuthor, yearMin, yearMax, journal, season, episode
# values might be empty if not specified from the caller
#
# When all search results are processed, the list of entries must be returned via:
# WebExtractor.searchResultsInfo( dict() )
#
# @todo implement person search
#
def searchItems( resourcetype, parameters ):

    title = parameters['title']
    yearMin = parameters['yearMin']
    yearMax = parameters['yearMax']
    season = parameters['season']
    episode = parameters['episode']
    showtitle = parameters['showtitle']

    try:
        ia = IMDb()
        if resourcetype == 'movie':
            WebExtractor.log( 'search movie with title: ' + title )
            results = ia.search_movie(title)
            searchMovieResults( results )

        elif resourcetype == 'tvshow':
            if title:
                WebExtractor.log( 'search episode with title: ' + title )
                results = ia.search_episode(title)
                searchTvEpisodeResults( ia, results, season, episode )

            else:
                WebExtractor.log( 'search tvshow with title: ' + showtitle )
                results = ia.search_movie(showtitle)
                searchTvShowResults( ia, results, season, episode )

        elif resourceType == "person":
            raise Exception("Person search not available yet.")

        else:
            if (type(resourceType).__name__ == "str"):
                raise Exception("\"%s\" search not available yet." % resourceType)

            else:
                raise Exception("Can't identify search type.")

    except Exception as err:
        WebExtractor.log("Script error: \n" + str(err))
        WebExtractor.error("Script error: \n" + str(err))

def searchMovieResults(results):
    searchResults = []
    for item in results :
        itemKind = str(item['kind'])

        if itemKind == 'movie':
            detailString = str(item['kind']) + ', ' + str(item['year'])
            fullUrl = 'http://www.imdb.com/title/tt' + item.movieID
            entryDict = dict(
                            title = item['title'],
                            details = detailString,
                            url = fullUrl
                            )

            searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

def searchTvEpisodeResults(ia, results, season, episode):
    searchResults = []
    for item in results :
        itemKind = str(item['kind'])

        if itemKind == 'episode':
            detailString = str(item['series title']) + ' (S' + item['season'] + 'E' + item['episode'] + ') : ' + item['plot']
            fullUrl = 'http://www.imdb.com/title/tt' + item.movieID
            entryDict = dict(
                            title = item['title'],
                            details = detailString,
                            url = fullUrl
                            )

            searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

def searchTvShowResults( ia, results, selectedSeason, selectedEpisode ):

    WebExtractor.log( 'results found:' + str(len(results)) )
    searchResults = []
    for show in results :
        showKind = str(show['kind'])
        WebExtractor.log( 'show found:' + showKind )

        if showKind == 'tv series':
            detailString = str(show['kind'])# + ', ' + str(show['series years'])
            fullUrl = 'http://www.imdb.com/title/tt' + show.movieID
            entryDict = dict(
                            title = show['title'],
                            details = detailString,
                            url = fullUrl
                            )

            searchResults.append(entryDict)

            # also add all seasons and episodes that fit to it
            ia.update(show, 'episodes')
            key = ";".join(show.keys())
            WebExtractor.log( 'show keys: ' + key )

            if not 'episodes' in show.keys():
                WebExtractor.log( 'no episodes available ')
            else:
                for season in show['episodes']:
                    if not selectedSeason or selectedSeason == season:
                        if selectedEpisode:
                            searchResults.append( getEpisodeInfo( show['episodes'][season][int(selectedEpisode)] ) )
                        else:
                            for ep in show2['episodes'][season]:
                                ia.update(ep)
                                searchResults.append( getEpisodeInfo( ep ) )

    WebExtractor.searchResults( searchResults )

def getEpisodeInfo(episode):

    detailString = str(episode['kind']) + ', ' + str(episode['series title']) + ' (S' + episode['season'] + 'E' + episode['episode'] + ') : ' + episode['plot']
    fullUrl = 'http://www.imdb.com/title/tt' + episode.movieID
    entryDict = dict(
                    title = episode['title'],
                    details = detailString,
                    url = fullUrl
                    )

    return entryDict

def extractItemFromUri( url, options ):
    exp = re.compile(r'title/tt(\d*)')
    match = exp.search(url)

    if not match:
        WebExtractor.error('Error parsing url: ' + url)
        return

    movieId = match.group(1)
    WebExtractor.log('parse imdb id: ' + movieId)

    ia = IMDb()

    try:
        movie = ia.get_movie(movieId)
    except Exception as err:
        WebExtractor.error("Script error: \n" + str(err))
        return

    movieDirector = ""
    movieWriter = ""
    movieCast = ""
    movieGenres = ""
    moviePlot = ""
    movieRuntime = ""
    movieCountries = ""
    movieCover = ""

    # chek if all the values exist
    #if 'director' in movie:
    movieDirector = ";".join([item["name"] for item in movie['director']])
    #if 'writer' in movie:
    movieWriter = ";".join([item["name"] for item in movie['writer']])
    #if 'cast' in movie:
    movieCast = ";".join([item["name"] for item in movie['cast']])
    #if 'genres' in movie:
    movieGenres = ';'.join(movie['genres'])
    #if 'plot' in movie:
    moviePlot = ';'.join(movie['plot'])
    #if 'runtimes' in movie:
    movieRuntime = ';'.join(movie['runtimes'])
    #if 'countries' in movie:
    movieCountries = ';'.join(movie['countries'])
    #if 'cover url' in movie:
    movieCover = movie['cover url']


    result = dict(  title = movie['title'],
                    year = movie['year'],
                    director = movieDirector,
                    writer = movieWriter,
                    cast = movieCast,
                    genre = movieGenres,
                    plot = moviePlot,
                    runtime = movieRuntime,
                    countries = movieCountries,
                    poster = movieCover
                  )

    # todo check how to do the results for tvshows
    WebExtractor.itemResults( 'movie', result )

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])
