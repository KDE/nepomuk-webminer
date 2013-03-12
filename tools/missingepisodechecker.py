#!/usr/bin/python
# -*- coding: utf-8 -*-

## This file is part of the Nepomuk KDE project.
## Copyright (C) 2013 Jörg Ehrichs <joerg.ehrichs@gmx.de>
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) version 3, or any
## later version accepted by the membership of KDE e.V. (or its
## successor approved by the membership of KDE e.V.), which shall
## act as a proxy defined in Section 6 of version 3 of the license.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library.  If not, see <http://www.gnu.org/licenses/>.

from tvdb_api import Tvdb

from PyKDE4.soprano import Soprano
from PyKDE4.nepomuk import Nepomuk
from PyQt4 import QtCore

from datetime import date, datetime
import sys
import argparse

def fetchLocalEposides(ignoreList):
    '''
    Query Nepomuk for all TVShows that are accociated with a file.

    Returns a dictionarylist with
    * TvShow
    ** SeasonNumber
    *** List of available Episodes
    '''

    tvshowDict = dict()

    model = Nepomuk.ResourceManager.instance().mainModel()
    query = 'select ?e ?ShowName ?SeasonNumer ?EpisodeNumber ?url where {'
    query +=    '?e a nmm:TVShow.'
    query +=    '?e nie:url ?url .'
    query +=    '?e nmm:series ?series .'
    query +=    '?series nie:title ?ShowName .'
    query +=    '?e nmm:episodeNumber ?EpisodeNumber .'
    query +=    '?e nmm:isPartOfSeason ?s .'
    query +=    '?s nmm:seasonNumber ?SeasonNumer .'

    if ignoreList:
        query += 'FILTER (str(?ShowName) != "'
        query += '" && str(?ShowName) != "'.join(ignoreList)
        query +=  '") .'
    query +=    '}'
    query +=    'ORDER BY ?ShowName ?SeasonNumer ?EpisodeNumber'

    it = model.executeQuery(query, Soprano.Query.QueryLanguageSparql)

    while it.next():
        show = it['ShowName'].toString().toLatin1().data()
        season = it['SeasonNumer'].literal().toInt()
        episode = it['EpisodeNumber'].literal().toInt()

        if tvshowDict.has_key(show):
            if tvshowDict[show].has_key(season):
                # add eposide number to available show/season
                tvshowDict[show][season].add(episode)

            else:
                # create new season entry for available show
                episodeSet = set()
                episodeSet.add(episode)
                seasonDict = {season : episodeSet}
                tvshowDict[show].update(seasonDict)

        else:
            # create now show entry
            episodeSet = set()
            episodeSet.add(episode)
            seasonDict = {season : episodeSet}

            tvshowDict.update({show: seasonDict})

    return tvshowDict

def findMissingEpisodes(seriesInfo, localData, onlyNew = False, onlyOld = False):
    '''
    Checks all seasons/episodes for the show on tvdb

    returns a list of all episodes in each season that is missing

    @p seriesInfo Tvdb() handler of the show
    @p localData dictionary with seasons and the set of episodes in them
    '''

    missingData = dict()

    # now start with season 1 until no season is available in the seriesInfo
    for s in range(1,len(seriesInfo)):

        # season info from tvdb
        season = seriesInfo[s]


        if localData.has_key(s):
            #check if an episode is missing local
            for e in range(1,len(season)):
                if e not in localData[s]:

                    missingEpisode = addEpisode(season, e, onlyNew, onlyOld)

                    if missingEpisode:
                        if s not in missingData:
                            missingData[s] = dict()

                        missingData[s].update(missingEpisode)

        # full season missing
        else:
            for e in range(1,len(season)):
                #missingData[s].update({str(e) : season[e]['firstaired']})
                missingEpisode = addEpisode(season, e, onlyNew, onlyOld)
                if missingEpisode:
                    if s not in missingData:
                        missingData[s] = dict()
                    missingData[s].update(missingEpisode)

    return missingData

def addEpisode(season, e, onlyNew = False, onlyOld = False):
    '''
    Check if the missing episode should be added to the list based on the first aired date

    return None if the episode should be skipped
    '''

    missingEpisode = dict()
    if season[e]['firstaired']:
        firstairedDate = datetime.strptime(season[e]['firstaired'], "%Y-%m-%d").date()

    if onlyOld and season[e]['firstaired'] and firstairedDate < date.today():
        return dict({str(e) : season[e]['firstaired']})

    elif onlyNew and season[e]['firstaired'] and firstairedDate > date.today():
        return dict({str(e) : season[e]['firstaired']})

    elif not onlyNew and not onlyOld:
        return dict({str(e) : season[e]['firstaired']})

    return None

def printData(data, printDate=True, listOnly=False):
    '''
    Prints out the list of missing data as
    ShowName s01e01 \t firstAiredDate
    '''

    for show, seasons in data.items():

        if not listOnly:
            print 'Missing episodes for ' + show

        for season, episodes in seasons.items():
            for episode, data in episodes.items():
                s = str(season)
                e = str(episode)

                if len(s) < 2:
                    s = '0' + s
                if len(e) < 2:
                    e = '0' + e

                if data and printDate:
                    print show + ' s' + s + 'e' + e + '\t' + data
                else:
                    print show + ' s' + s + 'e' + e

        if not listOnly:
            print '\n'

def main():

    parser = argparse.ArgumentParser(description='Check Nepomuk database for missing TvShow episodes')

    parser.add_argument('--old', '-o', action="store_true", dest="oldOnly", default=False,
                        help='Show only already aired episodes')

    parser.add_argument('--new', '-n', action="store_true", dest="newOnly", default=False,
                        help='Show only not yet aired episodes')

    parser.add_argument('--date', '-d', action="store_true", dest="printDate", default=False,
                        help='Print aired date with episode data')

    parser.add_argument('--list', '-l', action="store_true", dest="listOnly", default=False,
                        help='Only the list without additional info')

    parser.add_argument('--ignore', '-i', action="append", dest="ignoreList",
                        help='Add showname to ignore (can be specified multiply times -i Show1 -i show2 ...')

    args = parser.parse_args()

    # first get a nice list of all tvshows/season/episodes known to nepomuk
    if not args.listOnly:
        print 'Fetch data from Nepomuk storage'

    tvShowDict = fetchLocalEposides(args.ignoreList)

    missingData = dict()

    t = Tvdb()
    for key, value in tvShowDict.items():

        if not args.listOnly:
            print 'Check Tvdb for: ' + key

        series = t[key]
        if series:
            missingEpisodes = findMissingEpisodes(series, value, args.newOnly, args.oldOnly)

            if missingEpisodes:
                missingData[key] = missingEpisodes
        elif not args.listOnly:
            print key + ' not found on Tvdb.'

    #Now print the data neatly on the screen
    if not args.listOnly:
        print '\nResults:\n'

    printData(missingData, args.printDate, args.listOnly)

if __name__ == "__main__":
    main()
