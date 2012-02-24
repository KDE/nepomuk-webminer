
import re
from imdb import IMDb

#------------------------------------------------------------------------------
# Module options

# unique identification string, all lower-case no whitespace
identification = 'imdb'

#the full name of this engine
name = 'The Internet Movie Database'

#name of the icon for this engine
icon = 'imdb.png'

#a regular expression used to find the right engine for a specific website
regexp = r'http://www.imdb\.com/title/'

#what kind of nepomuk resource type can be fetched with this engine?
resourceType = 'movie'

asyncUsage = 'false'

#------------------------------------------------------------------------------
# creates the proper search query used for this side
# currently only for the title, author, freeform text and year search comes later
# we do not return more than 100 results
def searchQuery(title, author=None, freetext=None, year=None):
	query = "http://www.imdb.com/find?q=" + title + "&s=all"
	return query

#------------------------------------------------------------------------------
# here we actually search twice ...
# normally we would parse the website for the search
# here we get the title again from the url and rather ask the already existing
# imdb python plugin to do it properly.
def extractSearchResults(documentElement, metaData, url=None):
	exp = re.compile(r'q=(.*)&s=all')
	match = exp.search(url)
	if not match:
		print 'Error getting search sting from url'
		return None

	searchstring = match.group(1)
	
	ia = IMDb()
	results = ia.search_movie(searchstring)
	
	searchResults = []
	for item in results :

		detailString = str(item['kind']) + ', ' + str(item['year'])
		fullUrl = 'http://www.imdb.com/title/tt' + item.movieID
		entryDict = dict(
				title = item['title'],
				details = detailString,
				url = fullUrl
				)

		searchResults.append(entryDict)
	
	return searchResults


#------------------------------------------------------------------------------
# again we load the webpages twice ...
# but it is easier to let imdb handle the extarction instead of doing it
# on my own again.
def extractItemData(documentElement, metaData, url=None):
	exp = re.compile(r'title/tt(\d*)')
	match = exp.search(url)
	if not match:
		print 'Error getting movie id from url'
	movieId = match.group(1)
	
	ia = IMDb()
	movie = ia.get_movie(movieId)
	
	directorString = ''
	for director in movie['director']:
		directorString = directorString + ';' + director['name']

	writerString = ''
	for writer in movie['writer']:
		writerString = writerString + ';' + writer['name']

	castString = ''
	for cast in movie['cast']:
		castString = castString + ';' + cast['name']

	result = dict(  title = movie['title'],
			year = movie['year'],
			director = directorString,
			writer = writerString,
			cast = castString,
			genres = ';'.join(movie['genres']),
			plot = ';'.join(movie['plot']),
			runtime = ';'.join(movie['runtimes']),
			countries = ';'.join(movie['countries'])
			)
		
	return result
