#!/usr.bin/python
# -*- coding: utf-8 -*-

#------------------------------------------------------------------------------
# @see https://github.com/dbr/tvdb_api need 1.6.2
#------------------------------------------------------------------------------

import re
import json
import urllib2

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


confFileName = "tvdbrc"
# possible FIXME:is there a better way than this hardcoding?
uifileFolder = kdecore.KStandardDirs.locate("data", "nepomuk-webminer/plugins/") + "/uifiles";

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
        showtitle, season, episode = getConfiguredAlias(showtitle, season, episode)
    
    WebExtractor.log("Showtitle is " + str(showtitle) + " and season is " + str(season) + " and episode is " + str(episode))

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
def getConfiguredAlias(showtitle, showseason, showepisode):
  
    if showtitle is None or showtitle is '':
        return showtitle, showseason, showepisode
    
    conf = kdecore.KConfig(confFileName)
    confgroup = kdecore.KConfigGroup(conf, "aliases")
    rawdata = confgroup.readEntry(showtitle, QtCore.QStringList()).toStringList()
    if len(rawdata) > 1:
        if rawdata[1] == "-":
            return str(rawdata[0]), showseason, showepisode
        else:
            alias = rawdata.takeFirst()
            season, episode = process_seasonrule(rawdata, showseason, showepisode)
            return str(alias), season, episode
    # in case there's unicode involved
    rawdata = confgroup.readEntry(showtitle.decode('utf'), QtCore.QStringList()).toStringList()
    if len(rawdata) > 1:
        if rawdata[1] == "-":
            return str(rawdata[0]), showseason, showepisode
        else:
            alias = rawdata.takeFirst()
            season, episode = process_seasonrule(rawdata, showseason, showepisode)
            return str(alias), season, episode
    return showtitle, showseason, showepisode

# Helper function for above
def process_seasonrule(rules, showseason, showepisode):
    for rule in rules:
        components = QtCore.QString(rule).split(ruleSeparator)
        if components.count() < 2:
            return str(components[0]), showepisode
        if showepisode >= str(components[1]) and (components[2] == "-" or showepisode <= str(components[2])) :
            if components[3] == "1":
                return str(components[0]), str(int(str(showepisode)) - int(str(components[1])) + 1)
            else:
                return str(components[0]), showepisode
    return showseason, showepisode # no rules matched

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
    h = httplib2.Http("/tmp/httplib2")
    resp, content = h.request(malapiurl + '/anime/search?q=' + urllib2.quote(query))
    if int(resp['status']) != 200:
        return None
    return content

def raw_get_anime_details(anime_id):
    h = httplib2.Http("/tmp/httplib2")
    resp, content = h.request(malapiurl + '/anime/' + str(anime_id) + '?mine=0')

    if int(resp['status']) == 404:
        return None
    return content

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

#------------------------------------------------------------------------------
# Code for plugin-specific config dialog.
# Just a simple interface to add and delete custom aliases.
# Dealing with uic from python is pretty ill-documented, so not sure
# how good the methods here are, but they work well enough.
ruleSeparator = "|"
class ConfDialogController:
    def __init__(self):
        self.ui = uic.loadUi(uifileFolder + "/tvdbmalconfig.ui", QtGui.QDialog())
        self.conf = kdecore.KConfig(confFileName)
        self.confgroup = kdecore.KConfigGroup(self.conf, "aliases")
        self.refresh_widgets()
        self.ui.addButton.clicked.connect(self.add_item)
        self.ui.deleteButton.clicked.connect(self.remove_item)
        self.ui.modifyButton.clicked.connect(self.modify_item)

    def execdialog(self):
        self.ui.exec_()

    def refresh_widgets(self):
        self.ui.overrideList.clear()
        for key in self.confgroup.keyList():
            entry = self.confgroup.readEntry(key, QtCore.QStringList()).toStringList()
            alias = entry[0]
            if entry.__len__ > 1 and entry[1] != "-":
              alias += " (custom season rules)"
            i = QtGui.QListWidgetItem(key + " -> " + alias)
            i.setData(4, key)
            i.setData(5, self.confgroup.readEntry(key, QtCore.QStringList()))
            self.ui.overrideList.addItem(i)

    def remove_item(self):
        i = self.ui.overrideList.currentItem()
        if i:
            self.confgroup.deleteEntry(i.data(4).toString())
            self.conf.sync()
            self.refresh_widgets()

    def add_item(self):
        self.show_aliasui("", QtCore.QStringList(["", "-"]))

    def modify_item(self):
        i = self.ui.overrideList.currentItem()
        if i:
            self.show_aliasui(i.data(4).toString(), i.data(5).toStringList())

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
        self.aliasui.afterText.setText(value.takeFirst())
        self.definedRuleList = []
        if value[0] != "-":
            self.definedRuleList.extend(value)
            self.aliasui.manualRadio.setChecked(True)
            self.enable_aliascustom(True)
        self.refresh_rulelist_widget()
        self.aliasui.exec_()
        self.definedRuleList = []

    def save_new_alias(self):
        newEntry, newRule = self.get_new_aliasrule()
        self.confgroup.writeEntry(newEntry, newRule)
        self.confgroup.sync()
        self.refresh_widgets()

    def get_new_aliasrule(self):
        newEntry = self.aliasui.beforeText.text()
        newRule = []
        newRule.append(self.aliasui.afterText.text())
        if self.aliasui.autoRadio.isChecked():
            newRule.append("-")
        else:
            newRule.extend(self.definedRuleList)
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
            self.definedRuleList.append(str(season))
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
            self.definedRuleList.append(str(season) + ruleSeparator + str(fromEpisode) + ruleSeparator + "-" + ruleSeparator + reset)
            self.clear_rule_area()
            self.refresh_rulelist_widget()
            return
        toEpisode, ok = self.aliasui.toEpisodeEdit.text().toInt()
        if not ok:
            kdeui.KMessageBox.error(None, "Enter a numeric value for episode numbers!")
            return
        self.definedRuleList.append(str(season) + ruleSeparator + str(fromEpisode) + ruleSeparator + str(toEpisode) + ruleSeparator + reset)
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
            components = QtCore.QString(rule).split(ruleSeparator)
            if components.__len__() == 1:
                self.aliasui.ruleList.addItem("Use season " + components[0] + " for all episodes")
            elif components[2] == "-":
                addition = ""
                if components[3] == "1":
                    addition = " (reset)"
                self.aliasui.ruleList.addItem("Use season " + components[0] + " for episode " + components[1] + " and after" + addition)
            else:
                addition = ""
                if components[3] == "1":
                    addition = " (reset)"
                self.aliasui.ruleList.addItem("Use season " + components[0] + " for episodes " + components[1] + " through " + components[2] + addition)

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
