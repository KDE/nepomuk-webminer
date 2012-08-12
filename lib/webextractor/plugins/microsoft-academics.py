#!/usr/bin/env kross

import Kross
from PyQt4 import QtCore
import PyKDE4.kdecore as kdecore
from PyKDE4.kio import KIO

import json
import urllib
import re

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    from BeautifulSoup import BeautifulSoup # note switch to lxml

except:
    isAvailable = False
    errorMsg = "The BeautifulSoup python module needs to be installed."


finalEntry = {}
masid = ''
citations = []
citationResults = []

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
    return dict( name = 'Microsoft Academics Search',
                 icon = 'microsoft-academic-search.png',
                 identifier = 'mas',
                 resource = ['publication'],
                 urlregex = ['http://academic.research.microsoft.com/Publication/'],
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
# title, author, yearMin, yearMax, journal, season, episode
# values might be empty if not specified from the caller
#
# When all search results are processed, the list of entries must be returned via:
# WebExtractor.searchResultsInfo( dict() )
#
def searchItems( resourcetype, parameters ):

    #resourcetype ignored, we fetch publication only here

    title = parameters['title']
    author = parameters['author']
    yearMin = parameters['yearMin']
    yearMax = parameters['yearMax']
    journal = parameters['journal']

    title = title.replace(':',' ') #msn does not like the :
    queryPart = title

    if (author is not None) and (author):
        queryPart += ' author:(' + author + ')'

    if (yearMin is not None) and (yearMax is not None) and (yearMin) and (yearMax):
        queryPart += ' year>=' + yearMin + ' year<=' + yearMax

    if (journal is not None) and (journal):
        queryPart += ' jour:(' + journal + ')'

    query = "http://academic.research.microsoft.com/Search?query=" + queryPart + '&start=0&end=100'

    logMsg = 'start search query at: ' + query 
    WebExtractor.log( logMsg )

    # KIO part to fetch asyncron 
    data_url = kdecore.KUrl( query )
    retrieve_job = KIO.storedGet(data_url, KIO.NoReload, KIO.HideProgressInfo)
    retrieve_job.result.connect(handleSearchQuery)

#------------------------------------------------------------------------------
# process the returned search results from the KIO call
#
# calls extractSearchResults to extract the items or
# emits the error signal if something happend
#
def handleSearchQuery(job):

    if job.error():
        error = 'kio job error occured'
        WebExtractor.error( error )
        return

    data = job.data()

    #import os
    #gSysEncoding = os.environ["LANG"].split(".")[1]
    html = unicode(str(QtCore.QString(data).toUtf8()), 'utf-8')
    #print QtCore.QString(data).toUtf8()

    documentElement = BeautifulSoup( html ) # note switch to lxml

    # parse page to retrive all item data
    searchResults = extractSearchResults(documentElement, False)
    WebExtractor.searchResults( searchResults )

#------------------------------------------------------------------------------
# extracts the single item results from the html page returned by either 
#
#  * the intial search query
#  * the list of all available citations
#
# returns a list of dicts each dict has:
# 
# title   = name of the publication
# details = some more details on this entry (here Authors and number of available citations)
# url     = the url where to fetch the full data
#
# Or None if nothing was found
#
def extractSearchResults(documentElement, citation=False):

    # 1. check if we didn't get any results for our query
    noResult = documentElement.first('div', {'id':'ctl00_MainContent_divNoResult'})

    searchResults = []

    if noResult:
        WebExtractor.error( 'Could not extract Item info' )
        return searchResults

    if citation is True:
        i = 0
    else:
        i = 1

    for child in documentElement.findAll('li', {'class':'paper-item'}):
        if(i < 10):
            fetchentry = '0' + str(i)
        else:
            fetchentry = str(i)

        if citation is True:
            citationTag = child.find('a', {'id':'ctl00_MainContent_ObjectList_ctl' + fetchentry + '_Citation'})
            entryTitleTag = child.find('a', {'id':'ctl00_MainContent_ObjectList_ctl' + fetchentry + '_Title'})
        else:
            citationTag = child.find('a', {'id':'ctl00_MainContent_PaperList_ctl' + fetchentry + '_Citation'})
            entryTitleTag = child.find('a', {'id':'ctl00_MainContent_PaperList_ctl' + fetchentry + '_Title'})

        if entryTitleTag is not None:

            linkPart = entryTitleTag['href']

            exp = re.compile(r'Publication/(\d+)/')
            match = exp.search(linkPart)
            if not match:
                print 'Error parsing publication masid'
            masid = match.group(1)

            fullUrl = 'http://academic.research.microsoft.com/Publication/' + masid

            authorString = ''
            for author in child.findAll('a', {'class':'author-name-tooltip'}):
                authorString = authorString + author.renderContents() + ', '
                authorString = authorString[:-2]
                detailString = authorString

            if citationTag is not None:
                detailString = detailString + ', ' + citationTag.renderContents()

                entryDict = dict(
                                 title = entryTitleTag.renderContents(),
                                 details = detailString,
                                 url = fullUrl
                                )

                searchResults.append(entryDict)

            i += 1

    return searchResults

#------------------------------------------------------------------------------
# extracts the information of one item from the page at the url location
#
# in here it calls either the extarctItem function that loads the citations too or
# just the item without further citation information
#
# when all information are retrieved a call via:
# WebExtractor.itemResults( 'publication', dict() )
# will notify the caller that new data is available
#
# the dictionary follows the bibtex file standard + few exceptions
#
# bibtexentrytype =...          like book, article, inproceedings // necessary for the PublicationPipe to work properly
# references = ...              list of dicts, where each dict is another publication dict
#
# TODO: implement option to allow / disallow reference fetching in handleMainItemExtraction, refernce fetching takes a lot of time
def extractItemFromUri( url, options ):

    logMsg = 'start item extraction via: ' + url 
    WebExtractor.log( logMsg )

    global fetchCitations
    fetchCitations = options['references']

    # KIO part to fetch asyncron 
    data_url = kdecore.KUrl( url )
    retrieve_job = KIO.storedGet(data_url, KIO.NoReload, KIO.HideProgressInfo)
    retrieve_job.result.connect(handleMainItemExtraction)

#------------------------------------------------------------------------------
# process the returned item results from the KIO call
#
# calls extractItem to extract the items or
# emits the error signal if kio failed to fetch the data
# 
# and fetches the page with the possible reference list we need to process them
def handleMainItemExtraction(job):

    if job.error():
        error = 'kio job error occured'
        WebExtractor.error( error )
        return

    data = job.data()

    html = unicode(str(QtCore.QString(data).toUtf8()), 'utf-8')

    documentElement = BeautifulSoup( html ) # note switch to lxml

    # retrieve the basic data
    global finalEntry
    finalEntry = extractItem( documentElement )

    if fetchCitations:
        # check if we can fetch references
        url = 'http://academic.research.microsoft.com/Detail?entitytype=1&searchtype=5&id=' + masid + '&start=1&end=100'
        logMsg = 'fetch references via ' + url 
        WebExtractor.log( logMsg )
        # KIO part to fetch asyncron 
        data_url = kdecore.KUrl( url )
        retrieve_job = KIO.storedGet(data_url, KIO.NoReload, KIO.HideProgressInfo)
        retrieve_job.result.connect(handleCitationSearch)
    else:
        data_string = json.dumps(finalEntry)
        WebExtractor.itemResultsJSON( 'publication', data_string )

#------------------------------------------------------------------------------
# does the dirty work of extracting the html data
#
# extracts the mas id and downloads the bibtex data for the item to retrieve all other information
#
# returns the parsed dictionary with the publication data
#
def extractItem( documentElement ):
    entry = {}
    # 1. lets get the microsoft masid again to do further requests
    # necessary, as not all data can easily be parsed from the current page content
    global masid
    masidTag = documentElement.first('link', {'type':'application/rss+xml'})

    if masidTag is not None:
        masidHref = masidTag['href']
        masidPart = masidHref.rpartition("id=")
        masid = masidPart[2]
    else:
        WebExtractor.error( 'Could not fetch masid.' )
        return entry

    logMsg = 'masid found: ' + masid 
    WebExtractor.log( logMsg )

    # 2. we get propper publication data, this is available as bibtex file download
    # http://academic.research.microsoft.com/64764.bib?type=2&format=0&download=1
    # TODO:replace urllib by async appraoch
    WebExtractor.log( 'download bibtex file' )
    filehandle = urllib.urlopen('http://academic.research.microsoft.com/' + masid + '.bib?type=2&format=0&download=1')

    bibtexentrytype = re.compile(r'^@(\w+)\{(.*)')   # works on            :: @article{author = {Hans Wurst},
    bibtexKeys = re.compile(r'^(\w+)\s=\s\{+([^\{\}]*)\}+,') # works on any entry :: key = {value},

    # transform bibtex keys 1:1 into the dictionary we want to return
    for line in filehandle.readlines():
        typeMatch = bibtexentrytype.search(line)
        if(typeMatch):
            entry.update(dict(bibtexentrytype = typeMatch.group(1)))
            #ms does not format bibtex entry correctly, in the first line there is also the author field :/
            firstLineMatch = bibtexKeys.search(typeMatch.group(2))
            if(firstLineMatch):
                entry.update( {keysMatch.group(1) : keysMatch.group(2)})

        keysMatch = bibtexKeys.search(line)
        if(keysMatch):
            entry.update( {keysMatch.group(1) : keysMatch.group(2)})

    filehandle.close()

    # great, but there is more: fetch the abstract from the page
    abstract = documentElement.first('span', {'id':'ctl00_MainContent_PaperItem_snippet'})
    if abstract is not None:
        entry.update(dict(abstract = abstract.text))

    # TODO: parse downloadable files ... so we get informations on where to get the pdf file and so on

    return entry

#------------------------------------------------------------------------------
# Like the handleMainItemExtraction this processes the page where all references are listed as returned from the KIO call
#
# calls extractSearchResults tif we found a valid list to get the proper item info for each item
# if citations are found fetchNextCitation keeps track of fetching the next citation item until we are finished
# 
def handleCitationSearch(job):

    if job.error():
        error = 'kio job error occured'
        WebExtractor.error( error )
        return

    data = job.data()

    html = unicode(str(QtCore.QString(data).toUtf8()), 'utf-8')

    documentElement = BeautifulSoup( html ) # note switch to lxml

    global citations
    citations = extractSearchResults(documentElement, True)

    if citations:
        fetchNextCitation()
    else:
        WebExtractor.log( 'No references found' )
        #WebExtractor.itemResults( 'publication', finalEntry )
        # Use JSON was a workaround of an multiple nested dict issue with KROSS
        data_string = json.dumps(finalEntry)
        WebExtractor.itemResultsJSON( 'publication', data_string )


#------------------------------------------------------------------------------
#  checks the global "citations" if more citation etries are available
# 
# removes the first citation and parse it item datalobal 
# asyncron load the page and call handleCitationItemExtraction
def fetchNextCitation():

    global citations
    nextCitation = citations.pop()

    # check if we can fetch references
    logMsg = 'fetch reference' +  nextCitation['title']
    WebExtractor.log( logMsg )

    # KIO part to fetch asyncron 
    data_url = kdecore.KUrl( nextCitation['url'] )
    retrieve_job = KIO.storedGet(data_url, KIO.NoReload, KIO.HideProgressInfo)
    retrieve_job.result.connect(handleCitationItemExtraction)

#------------------------------------------------------------------------------
#  Process the ite mdata for a single citation element
#
# does not fetch any citation of this, to avoid a never endig loop where hundrets of citations are fetched
#
def handleCitationItemExtraction(job):

    global finalEntry
    if job.error():
        error = 'kio job error occured'
        WebExtractor.error( error )
        #WebExtractor.itemResults( 'publication', finalEntry )
        data_string = json.dumps(finalEntry)
        WebExtractor.itemResultsJSON( 'publication', data_string )
        return

    data = job.data()

    html = unicode(str(QtCore.QString(data).toUtf8()), 'utf-8')

    documentElement = BeautifulSoup( html ) # note switch to lxml

    # retrieve the basic data
    global citations
    citation = extractItem( documentElement )

    if citation:
        WebExtractor.log( 'citation fetched' )

    global citationResults
    citationResults.append( citation )

    if citations:
        fetchNextCitation()
    else:
        finalEntry['references'] = citationResults
        #WebExtractor.itemResults( 'publication', finalEntry )

        # this is a workaround, because multiple nested dictionaries cause some problems with the names of the keys
        data_string = json.dumps(finalEntry)
        WebExtractor.itemResultsJSON( 'publication', data_string )

