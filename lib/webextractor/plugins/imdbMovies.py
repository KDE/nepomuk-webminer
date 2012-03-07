#!/usr/bin/env kross

#------------------------------------------------------------------------------
# @todo replace IMDb python by a different script with better search capabilities
#------------------------------------------------------------------------------


import re
from imdb import IMDb

#------------------------------------------------------------------------------
# Module options

# Must have:
# name          = Long name of teh service
# icon          = download the favicon and put is as png in the folder, icon is than name.png
# identifier    = short unique identifier for the service, no special chars, whitespaces
# resource      = must be either publication, tvshow or movie. Describes what kind of data this plugin fetches
# urlregex      = regular expression that fits a url to this plugin
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
                 resource = ['movie','tvshow'],
                 description = 'some description',
                 author = 'Joerg Ehrichs',
                 email = 'some@mail.com')

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
def searchItems( resourcetype, parameters ):

	title = parameters['title']
	yearMin = parameters['yearMin']
	yearMax = parameters['yearMax']
	season = parameters['season']
	episode = parameters['episode']

	ia = IMDb()
	if resourcetype == 'movie':
		results = ia.search_movie(title)
	elif resourcetype == 'tvshow':
		results = ia.search_episode(title)

	searchResults = []
	for item in results :
                itemKind = str(item['kind'])

                # ignore tv show and such
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


def extractItemFromUri( url ):
	exp = re.compile(r'title/tt(\d*)')
	match = exp.search(url)
	if not match:
		print 'Error getting movie id from url'
	movieId = match.group(1)

	ia = IMDb()
	movie = ia.get_movie(movieId)

        result = dict(  title = movie['title'],
                        year = movie['year'],
                        director = ";".join([item["name"] for item in movie['director']]),
                        writer = ";".join([item["name"] for item in movie['writer']]),
                        cast = ";".join([item["name"] for item in movie['cast']]),
                        genres = ';'.join(movie['genres']),
                        plot = ';'.join(movie['plot']),
                        runtime = ';'.join(movie['runtimes']),
                        countries = ';'.join(movie['countries'])
                        )

        # todo check how to do the results for tvshows
	WebExtractor.itemResults( 'movie', result )
