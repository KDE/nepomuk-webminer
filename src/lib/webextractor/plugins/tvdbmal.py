#!/usr.bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------
# @see https://github.com/dbr/tvdb_api need 1.6.2
#------------------------------------------------------------------------------

import re
import json
import urllib2
import difflib

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    import tvdb_api

except:
    isAvailable = False
    errorMsg = "The tvdb python module 1.6.2 needs to be installed. See https://github.com/dbr/tvdb_api"

try:
    import httplib2
except:
    isAvailable = False
    errorMsg = "The python 'httplib2' module is a requirement. See http://code.google.com/p/httplib2/"

try:
    import PyQt4.QtGui as QtGui
    import PyQt4.QtCore as QtCore
    from PyQt4 import uic
except:
    isAvailable = False
    errorMsg = "PyQt is a requirement. See http://www.riverbankcomputing.co.uk/software/pyqt/intro"

try:
    import PyKDE4.kdecore as kdecore
    import PyKDE4.kdeui as kdeui
except:
    isAvailable = False
    errorMsg = "PyKDE is a requirement. Please install Python KDE bindings."


# possible FIXME:is there a better way than this hardcoding?
uifileFolder = kdecore.KStandardDirs.locate("data", "nepomuk-webminer/plugins/") + "/uifiles";

#------------------------------------------------------------------------------
# A wrapper class that does the stuff we need with MAL: find aliases and deduce season numbers
# Some of this code is adapted from malconstrict, a MAL api python wrapper
class MalWrapper:
    def __init__(self):
        self.malapiurl = 'http://mal-api.com'
        self.the_mal_entry = -1
        self.cur_query = ""

    def __raw_search_anime(self, query):
        h = httplib2.Http("/tmp/httplib2")
        resp, content = h.request(self.malapiurl + '/anime/search?q=' + urllib2.quote(query))
        if int(resp['status']) != 200:
            return None
        return content

    def __raw_get_anime_details(self, anime_id):
        h = httplib2.Http("/tmp/httplib2")
        resp, content = h.request(self.malapiurl + '/anime/' + str(anime_id) + '?mine=0')

        if int(resp['status']) == 404:
            return None
        return content

    # tries to find the "best match" series entry on MAL for given query
    def get_mal_entry(self, query):
        if self.the_mal_entry != -1 and self.cur_query == query:
            return self.the_mal_entry;
        self.cur_query = query
        min_acceptable_score = 0.7
        entries = json.loads(self.__raw_search_anime(query))
        # put some back into finding the best result!
        details = None
        while len(entries) > 0:
            # first we try and pick out the best title match
            best_score = 0
            best_match_index = 0
            for i in range(len(entries)):
                if self.__match_score(entries[i]['title'], query) > best_score:
                    best_score = self.__match_score(entries[i]['title'], query)
                    best_match_index = i
            best_match = entries.pop(best_match_index)
            if (best_match['type'] != "TV"):
                continue # we ignore non TV-series for now
            details = json.loads(self.__raw_get_anime_details(best_match['id']))
            WebExtractor.log("Evaluating "+ details['title'])
            # then we check if there is at least one alias meeting the minimum score
            if (self.__match_score(details['title'], query) > min_acceptable_score):
                break;
            if 'english' in details['other_titles']:
                for title in details['other_titles']['english']:
                    if (self.__match_score(title, query) > min_acceptable_score):
                        break;
            if 'synonyms' in details['other_titles']:
                for title in details['other_titles']['synonyms']:
                    if (self.__match_score(title, query) > min_acceptable_score):
                        break;
            details = None
        return details

    # finds aliases
    def get_other_titles(self, query):
        details = self.get_mal_entry(query)
        result = None
        if details:
            result = [details['title']]
            if 'english' in details['other_titles']:
                for title in details['other_titles']['english']:
                    result.append(title)
            if 'synonyms' in details['other_titles']:
                for title in details['other_titles']['synonyms']:
                    result.append(title)
        return result

    # well, deduces season number
    def deduce_season(self, query, default):
        details = self.get_mal_entry(query)
        if details:
            WebExtractor.log("Deducing season number with MAL..")
            # firstly, a lot of shows list sequel shows as "<showname> Nth Season"
            # in the synonym list, we look for that, and assume that "Nth Season"
            # isn't actually part of the real title
            # TODO: think harder about this assumption, can we avoid it?
            if 'synonyms' in details['other_titles']:
                for title in details['other_titles']['synonyms']:
                    if title[-6:].lower() == "season":
                        if title[-10:-9] == "1" and title[-9:-7].lower() == "st":
                            WebExtractor.log("Deduced season with synonym " + title)
                            return 1
                        elif title[-10:-9] == "2" and title[-9:-7].lower() == "nd":
                            WebExtractor.log("Deduced season with synonym " + title)
                            return 2
                        elif title[-10:-9].isdigit() and title[-9:-7].lower() == "th":
                            WebExtractor.log("Deduced season with synonym " + title)
                            return int(title[-10:-9])
            # well that didn't work, guess we need to do it the hard way.
            # basically, we see if a prequel is specified, and if it is, we look
            # and see if the prequel has a prequel, and we deduce the season by
            # counting the length of the prequel chain, assuming the end of the chain is
            # season 1
            season_count = 0
            curentry = details
            while True:
                if curentry['type'] == "TV":
                    WebExtractor.log("Prequel count at " + curentry['title'] + " : " + str(season_count))
                    season_count += 1
                if len(curentry['prequels']) < 1:
                    break
                curentry = json.loads(self.__raw_get_anime_details(curentry['prequels'][0]['anime_id']))
            if season_count > 1:
                return season_count
        return default

    def __match_score(self, str1, str2):
        return difflib.SequenceMatcher(None, str1.lower(), str2.lower()).ratio()


#------------------------------------------------------------------------------
# Wrapper for config file and the rules therein
#
class ConfigRulesWrapper:
    def __init__(self):
        self.confFileName = "tvdbmalrc"
        self.ruleSeparator = "|"
        self.conf = kdecore.KConfig(self.confFileName)
        self.confgroup = kdecore.KConfigGroup(self.conf, "aliases")

    def apply_configured_rules(self, showtitle, showseason, showepisode):
      
        if showtitle is None or showtitle is '':
            return showtitle, showseason, showepisode
        
        rawdata = self.confgroup.readEntry(showtitle, QtCore.QStringList()).toStringList()
        if len(rawdata) > 1:
            entry = self.interpret_rules(rawdata)
            if entry['rules'].__len__() < 1:
                return str(entry['alias']), showseason, showepisode
            else:
                alias = entry['alias']
                season, episode = showseason, showepisode
                for rule in entry['rules']:
                    season, episode = self.apply_rule(rule, season, episode)
                return str(alias), season, episode
        # in case there's unicode involved
        rawdata = self.confgroup.readEntry(showtitle.decode('utf'), QtCore.QStringList()).toStringList()
        if len(rawdata) > 1:
            entry = self.interpret_rules(rawdata)
            if entry['rules'].__len__() < 1:
                return str(entry['alias']), showseason, showepisode
            else:
                alias = entry['alias']
                season, episode = showseason, showepisode
                for rule in entry['rules']:
                    season, episode = self.apply_rule(rule, season, episode)
                return str(alias), season, episode
        return showtitle, showseason, showepisode

    def apply_rule(self, rule, showseason, showepisode):
        components = rule
        if components['type'] == 'globalseason':
            return str(components['season']), showepisode
        if components['type'] == 'customseason' and showepisode >= str(components['fromepisode']) and (components['toepisode'] == "-" or showepisode <= str(components[['toepisode']])) :
            if components['reset'] == "1":
                return str(components['season']), str(int(str(showepisode)) - int(str(components['fromepisode'])) + 1)
            else:
                return str(components['season']), showepisode
        if components['type'] == 'addepisode' and showseason == str(components['season']) and showepisode >= str(components['fromepisode']) and (components['toepisode'] == "-" or showepisode <= str(components[['toepisode']])) :
            return showseason, str(int(str(showepisode)) + int(str(components['amount'])))
        if components['type'] == 'skipepisode' and showseason == str(components['season']) and showepisode >= str(components['fromepisode']) and (components['toepisode'] == "-" or showepisode <= str(components[['toepisode']])) :
            return showseason, str(int(str(showepisode)) - int(str(components['amount'])))
        return showseason, showepisode

    def get_titles(self):
        return self.confgroup.keyList()

    def get_rawentry(self, key):
        return self.confgroup.readEntry(key, QtCore.QStringList()).toStringList()

    def get_entry(self, key):
        return self.interpret_rules(self.get_rawentry(key))

    def interpret_rules(self, entry):
        element = {}
        element['alias'] = entry.takeFirst()
        rules = []
        if entry.__len__() > 0 and entry[0] != "-":
            for rule in entry:
                rules.append(self.convert_seasonepisode_rule(rule))
        element['rules'] = rules
        return element

    def convert_seasonepisode_rule(self, rule):
        components = QtCore.QString(rule).split(self.ruleSeparator)
        if components[0] == 'globalseason':
            return {'type': 'globalseason', 'season': components[1]}
        if components[0] == 'customseason':
            return {'type': 'customseason', 'season': components[1], 'fromepisode': components[2], 'toepisode': components[3], 'reset': components[4]}
        if components[0] == 'addepisode':
            return {'type': 'addepisode', 'season': components[1], 'fromepisode': components[2], 'toepisode': components[3], 'amount': components[4]}
        if components[0] == 'skipepisode':
            return {'type': 'skipepisode', 'season': components[1], 'fromepisode': components[2], 'toepisode': components[3], 'amount': components[4]}

    def entry_to_raw(self, entry):
        if entry['rules'].__len__() < 1:
            return QtCore.QStringList([entry['alias'], "-"])
        rawentry = QtCore.QStringList([entry['alias']])
        for rule in entry['rules']:
            if rule['type'] == 'globalseason':
                rawentry.append('globalseason' + self.ruleSeparator + str(rule['season']))
            elif rule['type'] == 'customseason':
                rawentry.append('customseason' + self.ruleSeparator + str(rule['season']) + self.ruleSeparator + str(rule['fromepisode']) + self.ruleSeparator + str(rule['toepisode']) + self.ruleSeparator + str(rule['reset']))
            elif rule['type'] == 'addepisode':
                rawentry.append('addepisode' + self.ruleSeparator + str(rule['season']) + self.ruleSeparator + str(rule['fromepisode']) + self.ruleSeparator + str(rule['toepisode']) + self.ruleSeparator + str(rule['amount']))
            elif rule['type'] == 'skipepisode':
                rawentry.append('skipepisode' + self.ruleSeparator + str(rule['season']) + self.ruleSeparator + str(rule['fromepisode']) + self.ruleSeparator + str(rule['toepisode']) + self.ruleSeparator + str(rule['amount']))
        return rawentry

    def save_new_entry(self, title, entry):
        self.confgroup.writeEntry(title, self.entry_to_raw(entry))
        self.confgroup.sync()

    def remove_entry(self, title):
        self.confgroup.deleteEntry(title)
        self.confgroup.sync()

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
# hasConfig     = set to true if plugin has its own config dialog. Should show it using showConfigDialog()
#
def info():
    return dict( name = 'The TVDb (Augmented with MyAnimeList)',
                 homepage = 'http://thetvdb.com',
                 icon = 'tvdb.png',
                 identifier = 'tvdbmal',
#                 urlregex = ['http://thetvdb.com/\\?tab=episode&seriesid=(\\d+)&seasonid=(\\d+)&id=(\\d+).*', 'http://thetvdb.com/\\?tab=season&seriesid=(\\d+)&seasonid=(\\d+).*', 'http://thetvdb.com/\\?tab=series&id=(\\d+).*'],
                 urlregex = ['http://thetvdb.com/\\?tab=episode&seriesid=(\\d+)&seasonid=(\\d+)&id=(\\d+).*'],
                 resource = ['tvshow'],
                 description = 'An open database for television fans. This plugin will also attempt to find show aliases using the MyAnimeList API, as well as allow manual alias configurations',
                 author = 'Lim Yuen Hoe, Joerg Ehrichs',
                 email = 'yuenhoe@hotmail.com',
                 isAvailable = isAvailable,
                 hasConfig = True,
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
malwrapper = MalWrapper()
confwrapper = ConfigRulesWrapper()
def searchItems( resourcetype, parameters ):

    t = tvdb_api.Tvdb()

    searchResults = trySearch(parameters, t)
    WebExtractor.searchResults( searchResults )

#------------------------------------------------------------------------------
# put the logic code here into a helper so we can retry recursively (for cases where we have aliases)

def trySearch(parameters, t, recurse = True):
    
    title = ''
    showtitle = ''
    season = ''
    episode = ''
    
    if(parameters.has_key('title') ):
        title = parameters['title']
    if(parameters.has_key('season') ):
        season = parameters['season']
    if(parameters.has_key('episode') ):
        episode = parameters['episode']
    
    if(parameters.has_key('showtitle') ):
        showtitle = parameters['showtitle']
      
        WebExtractor.log("looking for configured aliases")
        oldseason = season
        showtitle, season, episode = confwrapper.apply_configured_rules(showtitle, season, episode)
        parameters['season'] = season
        parameters['episode'] = episode
        if season == oldseason and season == "1" and recurse == True:
            # means we probably can't get a definite season from the filename or the alias config
            # so we make an effort to figure out the season ourself
            # Note we don't need to change the showtitle becauses tvdb seems to list sequel names searchably as well
            # Also, if recurse if false it means we're doing an alias, and we already went through season deduction, so do only if true
            season = malwrapper.deduce_season(showtitle, season)
            parameters['season'] = season
            
    
    WebExtractor.log("Showtitle is " + str(showtitle) + " and season is " + str(season) + " and episode is " + str(episode))

    searchResults = []
    try:
        if showtitle and season and episode:
            # automatically advance to the next season if we can't find enough episodes
            # in the current season. Some series are listed this way, esp long series like
            # One Piece/Naruto.
            seasoninfo = t[showtitle.decode('utf')][int(season)] # the utf part is to prevent flipping out when filename has unicode
            while int(episode) > len(seasoninfo.keys()):
                episode = int(episode) - len(seasoninfo.keys())
                season = int(season) + 1
                WebExtractor.log("Season " + str(season - 1) + " only has " + str(len(seasoninfo.keys())) + " episodes, auto-advancing to season " + str(season))
                seasoninfo = t[showtitle.decode('utf')][int(season)]
            episodeResult = t[showtitle.decode('utf')][int(season)][int(episode)]
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
            
            aliases = malwrapper.get_other_titles(showtitle)
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
# Code for plugin-specific config dialog.
# Just a simple interface to add and delete custom aliases.
# Dealing with uic from python is pretty ill-documented, so not sure
# how good the methods here are, but they work well enough.
class ConfDialogController:
    def __init__(self):
        self.ui = uic.loadUi(uifileFolder + "/tvdbmalconfig.ui", QtGui.QDialog())
        self.refresh_widgets()
        self.ui.addButton.clicked.connect(self.add_item)
        self.ui.deleteButton.clicked.connect(self.remove_item)
        self.ui.modifyButton.clicked.connect(self.modify_item)

    def execdialog(self):
        self.ui.exec_()

    def refresh_widgets(self):
        self.ui.overrideList.clear()
        for key in confwrapper.get_titles():
            entry = confwrapper.get_entry(key)
            alias = entry['alias']
            if entry['rules'].__len__() > 0:
                alias += " (custom season rules)"
            i = QtGui.QListWidgetItem(key + " -> " + alias)
            i.setData(4, key)
            i.setData(5, confwrapper.get_rawentry(key))
            self.ui.overrideList.addItem(i)

    def remove_item(self):
        i = self.ui.overrideList.currentItem()
        if i:
            confwrapper.remove_entry(i.data(4).toString())
            self.refresh_widgets()

    def add_item(self):
        self.show_aliasui("", {'alias': '', 'rules':[] })

    def modify_item(self):
        i = self.ui.overrideList.currentItem()
        if i:
            self.show_aliasui(i.data(4).toString(), confwrapper.interpret_rules(i.data(5).toStringList()))

    def show_aliasui(self, field, value):
        self.aliasui = uic.loadUi(uifileFolder + "/tvdbmalaliasconfig.ui", QtGui.QDialog())
        self.aliasui.accepted.connect(self.save_new_alias)
        self.aliasui.autoRadio.clicked.connect(self.disable_aliascustom)
        self.aliasui.manualRadio.clicked.connect(self.enable_aliascustom)
        self.aliasui.allOrRangeCombo.currentIndexChanged.connect(self.toggle_episode_from)
        self.aliasui.toOrOnwardsCombo.currentIndexChanged.connect(self.toggle_episode_to)
        self.aliasui.addButton.clicked.connect(self.add_rule)
        self.aliasui.deleteButton.clicked.connect(self.delete_rule)
        self.aliasui.okButton.clicked.connect(self.validate_newalias)
        self.aliasui.beforeText.setText(field)
        self.aliasui.afterText.setText(value['alias'])
        self.definedRuleList = []
        if value['rules'].__len__() > 0:
            self.definedRuleList.extend(value['rules'])
            self.aliasui.manualRadio.setChecked(True)
            self.enable_aliascustom(True)
        self.refresh_rulelist_widget()
        self.aliasui.exec_()
        self.definedRuleList = []

    def save_new_alias(self):
        newEntry, newRule = self.get_new_aliasrule()
        confwrapper.save_new_entry(newEntry, newRule)
        self.refresh_widgets()

    def get_new_aliasrule(self):
        newEntry = self.aliasui.beforeText.text()
        newRule = {}
        newRule['alias'] = self.aliasui.afterText.text()
        newRule['rules'] = []
        if not self.aliasui.autoRadio.isChecked():
            newRule['rules'].extend(self.definedRuleList)
        return newEntry, newRule

    def delete_rule(self):
        i = 0
        self.definedRuleList.pop(self.aliasui.ruleList.currentRow())
        self.refresh_rulelist_widget()

    def validate_newalias(self):
        if self.aliasui.beforeText.text().isEmpty() or self.aliasui.afterText.text().isEmpty():
            kdeui.KMessageBox.error(None, "Enter the original and the alias to configure!")
        elif self.aliasui.manualRadio.isChecked() and self.definedRuleList.__len__() == 0:
            kdeui.KMessageBox.error(None, "You haven't created any custom season rules!")
        else:
            self.aliasui.accept()

    def add_rule(self):
        ok = False
        season, ok = self.aliasui.seasonEdit.text().toInt()
        if not ok:
            kdeui.KMessageBox.error(None, "Enter an integer for season number!")
            return
        if self.aliasui.allOrRangeCombo.currentIndex() == 0:
            self.definedRuleList.append({'type': 'globalseason', 'season': str(season)})
            self.clear_rule_area()
            self.refresh_rulelist_widget()
            return
        fromEpisode, ok = self.aliasui.fromEpisodeEdit.text().toInt()
        if not ok:
            kdeui.KMessageBox.error(None, "Enter a numeric value for episode numbers!")
            return
        reset = "0"
        if self.aliasui.resetEpisodes.isChecked():
            reset = "1"
        if self.aliasui.toOrOnwardsCombo.currentIndex() == 0:
            self.definedRuleList.append({'type': 'customseason', 'season': str(season), 'fromepisode': str(fromEpisode), 'toepisode': "-", 'reset': reset})
            self.clear_rule_area()
            self.refresh_rulelist_widget()
            return
        toEpisode, ok = self.aliasui.toEpisodeEdit.text().toInt()
        if not ok:
            kdeui.KMessageBox.error(None, "Enter a numeric value for episode numbers!")
            return
        self.definedRuleList.append({'type': 'customseason', 'season': str(season), 'fromepisode': str(fromEpisode), 'toepisode': str(toEpisode), 'reset': reset})
        self.clear_rule_area()
        self.refresh_rulelist_widget()

    def clear_rule_area(self):
        self.aliasui.seasonEdit.setText("")
        self.aliasui.allOrRangeCombo.setCurrentIndex(0)
        self.aliasui.fromEpisodeEdit.setText("")
        self.aliasui.toOrOnwardsCombo.setCurrentIndex(0)
        self.aliasui.toEpisodeEdit.setText("")
        self.aliasui.resetEpisodes.setChecked(True);

    def refresh_rulelist_widget(self):
        self.aliasui.ruleList.clear()
        for rule in self.definedRuleList:
            if rule['type'] == 'globalseason':
                self.aliasui.ruleList.addItem("Use season " + rule['season'] + " for all episodes")
            elif rule['type'] == 'customseason' and rule['toepisode'] == "-":
                addition = ""
                if rule['reset'] == "1":
                    addition = " (reset)"
                self.aliasui.ruleList.addItem("Use season " + rule['season'] + " for episode " + rule['fromepisode'] + " and after" + addition)
            else:
                addition = ""
                if rule['reset'] == "1":
                    addition = " (reset)"
                self.aliasui.ruleList.addItem("Use season " + rule['season'] + " for episodes " + rule['fromepisode'] + " through " + rule['toepisode'] + addition)

    def toggle_episode_from(self, index):
        if index == 1:
            self.aliasui.fromEpisodeEdit.setEnabled(True);
            self.aliasui.toOrOnwardsCombo.setEnabled(True);
            self.aliasui.resetEpisodes.setEnabled(True);
            if self.aliasui.toOrOnwardsCombo.currentIndex() == 1:
                self.aliasui.toEpisodeEdit.setEnabled(True);
        elif index == 0:
            self.aliasui.fromEpisodeEdit.setEnabled(False);
            self.aliasui.toOrOnwardsCombo.setEnabled(False);
            self.aliasui.toEpisodeEdit.setEnabled(False);
            self.aliasui.resetEpisodes.setEnabled(False);

    def toggle_episode_to(self, index):
        if index == 1:
            self.aliasui.toEpisodeEdit.setEnabled(True);
        elif index == 0:
            self.aliasui.toEpisodeEdit.setEnabled(False);

    def disable_aliascustom(self, checked):
        if checked:
            self.aliasui.seasonEdit.setEnabled(False);
            self.aliasui.allOrRangeCombo.setEnabled(False);
            self.aliasui.fromEpisodeEdit.setEnabled(False);
            self.aliasui.toOrOnwardsCombo.setEnabled(False);
            self.aliasui.toEpisodeEdit.setEnabled(False);
            self.aliasui.resetEpisodes.setEnabled(False);
            self.aliasui.addButton.setEnabled(False);
            self.aliasui.ruleList.setEnabled(False);
            self.aliasui.deleteButton.setEnabled(False);

    def enable_aliascustom(self, checked):
        if checked:
            self.aliasui.seasonEdit.setEnabled(True);
            self.aliasui.allOrRangeCombo.setEnabled(True);
            if self.aliasui.allOrRangeCombo.currentIndex() == 1:
                self.aliasui.fromEpisodeEdit.setEnabled(True);
                self.aliasui.toOrOnwardsCombo.setEnabled(True);
                self.aliasui.resetEpisodes.setEnabled(True);
                if self.aliasui.toOrOnwardsCombo.currentIndex() == 1:
                    self.aliasui.toEpisodeEdit.setEnabled(True);
            self.aliasui.addButton.setEnabled(True);
            self.aliasui.ruleList.setEnabled(True);
            self.aliasui.deleteButton.setEnabled(True);


def showConfigDialog():
    test = ConfDialogController()
    test.execdialog()

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])
