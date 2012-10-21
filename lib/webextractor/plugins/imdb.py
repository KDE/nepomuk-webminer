#!/usr/bin/env kross

import re
import json

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
                 homepage = 'http://www.imdb.com',
                 icon = 'imdb.png',
                 identifier = 'imdbmovies',
                 urlregex = ['http://www.imdb.com/title/tt'],
                 resource = ['movie', 'tvshow'],
                 description = 'IMDb is the world’s most popular and authoritative source for movie, TV and celebrity content.',
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
    season = parameters['season']
    episode = parameters['episode']
    showtitle = parameters['showtitle']

    if season is None:
        season = ""
    if episode is None:
        episode = ""
    if showtitle is None:
        showtitle = ""
    if yearMax is None:
        yearMax = ""
    if yearMin is None:
        yearMin = ""
    if title is None:
        title = ""

    WebExtractor.log( 'search ' + resourcetype +' with title: ' + title )
    try:
        ia = IMDb()
        if resourcetype == 'movie':
            WebExtractor.log( 'search movie with title: ' + title )
            results = ia.search_movie(title)
            searchMovieResults( results )

        elif resourcetype == 'tvshow':
            if title and not (season or episode):
                WebExtractor.log( 'search episode with title: ' + title + ' S:' + season + ' E:' + episode  )
                results = ia.search_episode(title)
                searchTvEpisodeResults( ia, results, showtitle, season, episode )

            else:
                WebExtractor.log( 'search tvshow with title: ' + showtitle + ' S:' + season + ' E:' + episode )
                results = ia.search_movie(showtitle)
                searchTvShowResults( ia, results, showtitle, title, season, episode )

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
        if not item.has_key('kind'):
            raise Exception("There is no \"kind\" key in \"results\" parameter.")

	WebExtractor.log( 'found search result: ' + str(item['title'].encode("utf-8")) )
        itemKind = str(item['kind'])
        if itemKind == 'movie':
            detailString = itemKind

            if item.has_key('year'):
                detailString += ', ' + str(item['year'])

            fullUrl = 'http://www.imdb.com/title/tt' + str(item.movieID)
            entryDict = dict(
                            title = item['title'].encode("utf-8"),
                            details = detailString,
                            url = fullUrl
                            )

            searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

def searchTvEpisodeResults(ia, results, showtitle, season, episode):
    searchResults = []
    for item in results :
        if not item.has_key('kind'):
            raise Exception("There is no \"kind\" key in \"results\" parameter.")

        itemKind = str(item['kind'])
        if itemKind == 'episode':
            detailString = str(item['series title']) + ' (S' + item['season'] + 'E' + item['episode'] + ')'
            fullUrl = 'http://www.imdb.com/title/tt' + item.movieID
            entryDict = dict(
                            title = item['title'],
                            details = detailString,
                            url = fullUrl
                            )

            searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

def searchTvShowResults( ia, results, showtitle, title, selectedSeason, selectedEpisode ):

    searchResults = []
    for show in results :
        showKind = str(show['kind'])

        if showKind == 'tv series' or showKind == 'tv mini series':
            WebExtractor.log( 'show found: ' + showKind + show['title'].encode('utf-8') )
            WebExtractor.log( 'search Episode: S' + selectedSeason + ' E' + selectedEpisode )

            # first fetch episode details
            ia.update(show, info=('episodes cast', 'akas', 'main', 'episodes')) #BUG: fetching 'episodes' instead of 'episodes cast' will not work. imdbPy is broken here

            key = ";".join(show.keys())

            if not show.has_key('episodes'):
                WebExtractor.log( 'No episodes available')
                continue

            # Now get the right episode details
            # if we have a defined season and episode number, take thos
            # if we have no episode but a season, list all episodes
            # if we have an episode and no season, list all episodes with the number from each season

            if selectedSeason and selectedEpisode:

                if len(show['episodes']) < int(selectedSeason):
                    continue
                if int(selectedSeason) not in show['episodes']:
                    continue
                if len(show['episodes'][int(selectedSeason)]) < int(selectedEpisode):
                    continue

                episode = show['episodes'][int(selectedSeason)][int(selectedEpisode)]
                ia.update(episode)

                searchResults.append( getEpisodeInfo(show, episode) )

            elif selectedSeason and selectedEpisode == "":
                if len(show['episodes']) < int(selectedSeason):
                    continue

                episodeNumber = 1
                while episodeNumber <= len(show['episodes'][int(selectedSeason)]):
                    episode = show['episodes'][int(selectedSeason)][episodeNumber]
                    ia.update(episode)
                    searchResults.append( getEpisodeInfo(show, episode) )
                    episodeNumber += 1

            elif selectedSeason == "" and selectedEpisode:
                seasonNumber = 1
                while seasonNumber <= show['number of seasons']:
                    episode = show['episodes'][seasonNumber][int(selectedEpisode)]
                    ia.update(episode)
                    searchResults.append( getEpisodeInfo(show, episode) )
                    seasonNumber += 1


    WebExtractor.searchResults( searchResults )

def getEpisodeInfo(show, episode):

    detailString = str(akaTitleSelect(show)) + ' (S' + str(episode['season']) + 'E' + str(episode['episode']) + ')'
    fullUrl = 'http://www.imdb.com/title/tt' + str(episode.movieID)
    entryDict = dict(
                    title = episode['title'].decode('utf'),
                    details = detailString,
                    url = fullUrl
                    )

    return entryDict

'''
This part is copied from imdbPy

#FIXME:Needs to be replaced with imdp.helpers getAKAsInLanguage(movie, 'English')
Sadly importing this function always returns in errors. Anyone has any ideas to solve this?
'''
_re_akas_lang = re.compile('(?:[(])([a-zA-Z]+?)(?: title[)])')
_re_akas_country = re.compile('\(.*?\)')

# Maps a language to countries where it is the main language.
# If you want to add an entry for another language or country, mail it at
# imdbpy-devel@lists.sourceforge.net .
LANG_COUNTRIES = {
    'English': ('Canada', 'Swaziland', 'Ghana', 'St. Lucia', 'Liberia', 'Jamaica', 'Bahamas', 'New Zealand', 'Lesotho', 'Kenya', 'Solomon Islands', 'United States', 'South Africa', 'St. Vincent and the Grenadines', 'Fiji', 'UK', 'Nigeria', 'Australia', 'USA', 'St. Kitts and Nevis', 'Belize', 'Sierra Leone', 'Gambia', 'Namibia', 'Micronesia', 'Kiribati', 'Grenada', 'Antigua and Barbuda', 'Barbados', 'Malta', 'Zimbabwe', 'Ireland', 'Uganda', 'Trinidad and Tobago', 'South Sudan', 'Guyana', 'Botswana', 'United Kingdom', 'Zambia'),
    'Italian': ('Italy', 'San Marino', 'Vatican City'),
    'Spanish': ('Spain', 'Mexico', 'Argentina', 'Bolivia', 'Guatemala', 'Uruguay', 'Peru', 'Cuba', 'Dominican Republic', 'Panama', 'Costa Rica', 'Ecuador', 'El Salvador', 'Chile', 'Equatorial Guinea', 'Spain', 'Colombia', 'Nicaragua', 'Venezuela', 'Honduras', 'Paraguay'),
    'French': ('Cameroon', 'Burkina Faso', 'Dominica', 'Gabon', 'Monaco', 'France', "Cote d'Ivoire", 'Benin', 'Togo', 'Central African Republic', 'Mali', 'Niger', 'Congo, Republic of', 'Guinea', 'Congo, Democratic Republic of the', 'Luxembourg', 'Haiti', 'Chad', 'Burundi', 'Madagascar', 'Comoros', 'Senegal'),
    'Portuguese': ('Portugal', 'Brazil', 'Sao Tome and Principe', 'Cape Verde', 'Angola',  'Mozambique', 'Guinea-Bissau'),
    'German': ('Liechtenstein', 'Austria', 'West Germany', 'Switzerland', 'East Germany', 'Germany'),
    'Arabic': ('Saudi Arabia', 'Kuwait', 'Jordan', 'Oman', 'Yemen', 'United Arab Emirates', 'Mauritania', 'Lebanon', 'Bahrain', 'Libya', 'Palestinian State (proposed)', 'Qatar', 'Algeria', 'Morocco', 'Iraq', 'Egypt', 'Djibouti', 'Sudan', 'Syria', 'Tunisia'),
    'Turkish': ('Turkey', 'Azerbaijan'),
    'Swahili': ('Tanzania',),
    'Swedish': ('Sweden',),
    'Icelandic': ('Iceland',),
    'Estonian': ('Estonia',),
    'Romanian': ('Romania',),
    'Samoan': ('Samoa',),
    'Slovenian': ('Slovenia',),
    'Tok Pisin': ('Papua New Guinea',),
    'Palauan': ('Palau',),
    'Macedonian': ('Macedonia',),
    'Hindi': ('India',),
    'Dutch': ('Netherlands', 'Belgium', 'Suriname'),
    'Marshallese': ('Marshall Islands',),
    'Korean': ('Korea, North', 'Korea, South', 'North Korea', 'South Korea'),
    'Vietnamese': ('Vietnam',),
    'Danish': ('Denmark',),
    'Khmer': ('Cambodia',),
    'Lao': ('Laos',),
    'Somali': ('Somalia',),
    'Filipino': ('Philippines',),
    'Hungarian': ('Hungary',),
    'Ukrainian': ('Ukraine',),
    'Bosnian': ('Bosnia and Herzegovina',),
    'Georgian': ('Georgia',),
    'Lithuanian': ('Lithuania',),
    'Malay': ('Brunei',),
    'Tetum': ('East Timor',),
    'Norwegian': ('Norway',),
    'Armenian': ('Armenia',),
    'Russian': ('Russia',),
    'Slovak': ('Slovakia',),
    'Thai': ('Thailand',),
    'Croatian': ('Croatia',),
    'Turkmen': ('Turkmenistan',),
    'Nepali': ('Nepal',),
    'Finnish': ('Finland',),
    'Uzbek': ('Uzbekistan',),
    'Albanian': ('Albania', 'Kosovo'),
    'Hebrew': ('Israel',),
    'Bulgarian': ('Bulgaria',),
    'Greek': ('Cyprus', 'Greece'),
    'Burmese': ('Myanmar',),
    'Latvian': ('Latvia',),
    'Serbian': ('Serbia',),
    'Afar': ('Eritrea',),
    'Catalan': ('Andorra',),
    'Chinese': ('China', 'Taiwan'),
    'Czech': ('Czech Republic', 'Czechoslovakia'),
    'Bislama': ('Vanuatu',),
    'Japanese': ('Japan',),
    'Kinyarwanda': ('Rwanda',),
    'Amharic': ('Ethiopia',),
    'Persian': ('Afghanistan', 'Iran'),
    'Tajik': ('Tajikistan',),
    'Mongolian': ('Mongolia',),
    'Dzongkha': ('Bhutan',),
    'Urdu': ('Pakistan',),
    'Polish': ('Poland',),
    'Sinhala': ('Sri Lanka',),
}

COUNTRY_LANG = {}
for lang in LANG_COUNTRIES:
    for country in LANG_COUNTRIES[lang]:
        COUNTRY_LANG[country] = lang

def akaTitleSelect(show):

    if show.has_key('akas'):
        akas = set((show.get('akas') or []) + (show.get('akas from release info') or []))
        for aka in akas:
            # split aka
            aka = aka.encode('utf8').split('::')
            # sometimes there is no countries information
            if len(aka) == 2:
                # search for something like "(... title)" where ... is a language
                language = _re_akas_lang.search(aka[1])
                if language:
                    language = language.groups()[0]
                else:
                    # split countries using , and keep only the first one (it's sufficient)
                    country = aka[1].split(',')[0]
                    # remove parenthesis
                    country = _re_akas_country.sub('', country).strip()
                    # given the country, get corresponding language from dictionary
                    #language = country
                    language = COUNTRY_LANG.get(country)
            else:
                continue
            
            if language == 'English':
                WebExtractor.log( 'found aka: ' + aka[0].decode('utf8') )
                fixedTitle = re.sub(r'"', '', aka[0])
                return fixedTitle

    # if we have no akas or no english aka return normal title
    WebExtractor.log( 'did not find aka: ' + show['title'].decode('utf8') )
    return show['title'].decode('utf8')

'''
#################################
# end copy from imdbpy
'''

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

    #TODO: fetch only fields we actually need not all (which is super slow)
    ia.update(movie, 'all')

    # fetch general information
    movieDirector = movieWriter = movieCast = movieGenres = moviePlot = movieRuntime = movieCountries = movieCover = ""

    # Read existing values.
    if movie.has_key('director'):
        movieDirector = ";".join([item["name"] for item in movie['director']])

    if movie.has_key('writer'):
        movieWriter = ";".join([item["name"] for item in movie['writer']])

    if movie.has_key('cast'):
        movieCast = ";".join([item["name"] for item in movie['cast']])

    if movie.has_key('genres'):
        movieGenres = ';'.join(movie['genres'])

    if movie.has_key('synopsis') and movie['synopsis'] != "":
        moviePlot = movie['synopsis']
    elif movie.has_key('plot') and movie['plot'] != "":
        moviePlot = ';'.join(movie['plot'])
    elif movie.has_key('plot outline') and movie['plot outline'] != "":
        moviePlot = movie['plot outline']

    if movie.has_key('runtimes'):
        movieRuntime = ';'.join(movie['runtimes'])

    if movie.has_key('countries'):
        movieCountries = ';'.join(movie['countries'])

    if movie.has_key('cover url'):
        movieCover = movie['cover url']

    #------------------------------------------
    # Now create the right dictinaries depending on the movie type
    if movie['kind'] == 'movie':

        result = dict(  title = movie['title'],
                        year = movie['year'],
                        director = movieDirector,
                        writer = movieWriter,
                        cast = movieCast,
                        genre = movieGenres,
                        plot = moviePlot,
                        runtime = movieRuntime,
                        countries = movieCountries,
                        poster = movieCover,
                        seealso = url
                    )

        WebExtractor.itemResults( 'movie', result )

    elif movie['kind'] == 'episode':

        episodeDict = dict (
                            title = movie['title'].encode('utf-8'),
                            overview = moviePlot,
                            number = movie['episode'],
                            firstaired = movie['original air date'],
                            director = movieDirector,
                            writer = movieWriter,
                            actors = movieCast,
                            genre = movieGenres,
                            banner = movieCover,
                            seealso = url
                            )

        seasonDict = dict (
                        episodes = [episodeDict],
                        number = movie['season'],
                        genre = movieGenres,
                        poster = movieCover
                        )

        # we need to search for the show again :(
        # movie['eposide of'] only returns the original title
        # in case of animes it is the japanese one, but i want the english (or as configured a different translated one)
        results = ia.search_movie(movie['series title'])
        selectedShow = None

        for show in results :
            showKind = str(show['kind'])

            if show['title'] == movie['series title'] and (showKind == 'tv series' or showKind == 'tv mini series'):
                ia.update(show, info=('akas', 'main'))
                WebExtractor.log( 'Found original show: ' + show['title'] )
                if( show.has_key('akas') ):
		  WebExtractor.log( 'with akas: ' + ';'.join(show['akas']) )
                else:
		  WebExtractor.log( 'no akas available' )
                selectedShow = show
                break;

        showGenres = showPlot = showCover = ""

        if selectedShow.has_key('genres'):
            showGenres = ';'.join(selectedShow['genres'])

        if selectedShow.has_key('synopsis') and selectedShow['synopsis'] != "":
            showPlot = selectedShow['synopsis']
        elif selectedShow.has_key('plot') and selectedShow['plot'] != "":
            showPlot = ';'.join(selectedShow['plot'])
        elif selectedShow.has_key('plot outline') and selectedShow['plot outline'] != "":
            showPlot = selectedShow['plot outline']

        if selectedShow.has_key('full-size cover url'):
            showCover = selectedShow['full-size cover url']
        elif selectedShow.has_key('cover url'):
            showCover = selectedShow['cover url']

        seriesDict = dict (
                            seasons = [seasonDict],
                            title = akaTitleSelect(selectedShow),
                            overview = showPlot,
                            poster = showCover,
                            genre = showGenres,
                            seealso = 'http://www.imdb.com/title/tt' + str(selectedShow.movieID)
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
