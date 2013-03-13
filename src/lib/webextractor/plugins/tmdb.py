#!/usr/bin/env kross

import re
import json

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
  import httplib2
except:
    isAvailable = False
    errorMsg = "The httplib2 is needed"
try:
  import urllib2
except:
    isAvailable = False
    errorMsg = "The urllib2 is needed"

API_KEY = '8c55704d2c3b2adefd6c2496fcaa3754'
base_url = 'http://private-e81f-themoviedb.apiary.io/3/'

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
    return dict( name = 'The Movie Database',
                 homepage = 'http://www.themoviedb.org/',
                 icon = 'tmdb.png',
                 identifier = 'tmdb',
                 urlregex = ['http://www.themoviedb.org/movie/'],
                 resource = ['movie'],
                 description = 'themoviedb.org is a free and community maintained movie database. It\'s completely user built by people like you.',
                 author = 'Joerg Ehrichs',
                 email = 'Joerg.Ehrichs@gmx.de',
                 isAvailable = isAvailable,
                 errorMsg = errorMsg)

#------------------------------------------------------------------------------
# starts a search query
#
# build the query url from the given parameters
#
# currently supported keys are:
# title, author, yearMin, yearMax, showtitle, season, episode
# values might be empty if not specified from the caller
#
# When all search results are processed, the list of entries must be returned via:
# WebExtractor.searchResultsInfo( dict() )
#
#
def searchItems( resourcetype, parameters ):

    title = parameters['title']
    yearMin = parameters['yearMin']
    yearMax = parameters['yearMax']

    if yearMax is None:
        yearMax = ""
    if yearMin is None:
        yearMin = ""
    if title is None:
        title = ""

    WebExtractor.log( 'search ' + resourcetype +' with title: ' + title )
    try:
        if resourcetype == 'movie':
            WebExtractor.log( 'search movie with title: ' + title )
            
            
            h = httplib2.Http("/tmp/httplib2")
            #TODO: add year query
            service = 'search/movie'
            url = base_url +  service + '?api_key=' + API_KEY + '&query=' + urllib2.quote(title)
            resp, content = h.request(url, headers={'Accept':'application/json'})

            results = json.loads(content)
            searchMovieResults( results )

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
    
    
    for item in results['results']:

        WebExtractor.log( 'found search result: ' + item['title'] )

        fullUrl = 'http://www.themoviedb.org/movie/' + str(item['id'])
        entryDict = dict(
                         title = item['title'].encode("utf-8"),
                         details = item['release_date'],
                         url = fullUrl,
                         distanceString = item['title'].encode("utf-8")
                        )

        searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

def extractItemFromUri( url, options ):
    exp = re.compile(r'/movie/(\d*)')
    match = exp.search(url)

    if not match:
        WebExtractor.error('Error parsing url: ' + url)
        return

    movieId = match.group(1)
    WebExtractor.log('parse tmdb id: ' + movieId)

    #fetch movie data
    h = httplib2.Http("/tmp/httplib2")
    service = 'movie/' + str(movieId)
    url = base_url +  service + '?api_key=' + API_KEY
    resp, content = h.request(url, headers={'Accept':'application/json'})

    movieInfo = json.loads(content)

    #parse genres
    genres = []
    for g in movieInfo['genres']:
        genres.append(g['name'])

    #parse cast information
    movieDirector = []
    movieWriter = []
    movieCast = []
    
    service = 'movie/' + str(movieId) + '/casts'
    url = base_url +  service + '?api_key=' + API_KEY
    resp, content = h.request(url, headers={'Accept':'application/json'})

    movieCasts = json.loads(content)
    
    for person in movieCasts['cast']:
        movieCast.append(person['name'])
    
    for person in movieCasts['crew']:
      if person['department'] == 'Directing':
        movieDirector.append(person['name'])
        
      if person['department'] == 'Writing':
        movieWriter.append(person['name'])
    


    #------------------------------------------
    # Now create the dictinary

    result = dict(  title = movieInfo['title'],
                    year = movieInfo['release_date'],
                    director = ';'.join(movieDirector),
                    writer = ';'.join(movieWriter),
                    cast = ';'.join(movieCast),
                    genre = ';'.join(genres),
                    plot = movieInfo['overview'],
                    runtime = str(movieInfo['runtime']),
                    poster = 'http://cf2.imgobject.com/t/p/original' + movieInfo['poster_path'],
                    seealso = movieInfo['homepage']
                  )
                    
    # this is a workaround, because multiple nested dictionaries cause some problems with the names of the keys
    #WebExtractor.itemResults( 'tvshow', seriesDict )
    data_string = json.dumps(result)
    WebExtractor.itemResultsJSON( 'movie', data_string )

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])
