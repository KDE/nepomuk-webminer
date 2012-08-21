#!/usr/bin/env kross

import json
import re

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    import string
    import urllib2
    import httplib2
    from BeautifulSoup import BeautifulSoup

except:
    isAvailable = False
    errorMsg = "Module requirements missing. springer.py needs:\nThe BeautifulSoup python module. see: http://www.crummy.com/software/BeautifulSoup/\nThe Httplib2 python module. See: http://code.google.com/p/httplib2/"


masid = ''
citations = []
citationResults = []

#------------------------------------------------------------------------------
# Module options

# Must have:
# name          = Long name of the service
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

    query = "http://academic.research.microsoft.com/Search?query=" + urllib2.quote(queryPart.encode("utf8")) + '&start=0&end=50'

    logMsg = 'start search query at: ' + query 
    WebExtractor.log( logMsg )

    h = httplib2.Http(".cache")
    resp, content = h.request(query)

    documentElement = BeautifulSoup( content )

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

    pubItems = documentElement.findAll('li', 'paper-item')
    for child in pubItems:
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
                WebExtractor.error( 'Error parsing publication masid' )
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
# If the options allow to fetch citations (which is very slow as it might need to fetch lots of them) we call fetchNextCitation() for each 
# citation element available
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
def extractItemFromUri( url, options ):

    logMsg = 'start item extraction via: ' + url 
    WebExtractor.log( logMsg )

    fetchCitations = options['references']

    #clear all citations available from an earlier call
    global citations
    citations = []
    global citations
    citationResults = []

    h = httplib2.Http(".cache")
    resp, content = h.request(url)

    documentElement = BeautifulSoup( content )

    # retrieve the basic data
    finalEntry = extractItem( documentElement )

    if fetchCitations:
        # check if we can fetch references
        # masid is a global variable retrieved in extractItem() 
        url = 'http://academic.research.microsoft.com/Detail?entitytype=1&searchtype=5&id=' + masid + '&start=1&end=100'
        logMsg = 'fetch references via ' + url 
        WebExtractor.log( logMsg )

        h = httplib2.Http(".cache")
        resp, content = h.request(url)

        documentElement = BeautifulSoup( content )

        global citations
        citations = extractSearchResults(documentElement, True)

        if not citations:
            WebExtractor.log( 'No references found' )

        while citations:
            fetchNextCitation()

        # Add all found citations to the final entry
        global citationResults
        if citationResults:
            finalEntry['references'] = citationResults


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
    WebExtractor.log( 'download bibtex file' )

    h = httplib2.Http(".cache")
    resp, content = h.request( 'http://academic.research.microsoft.com/' + masid + '.bib?type=2&format=0&download=1' )

    bibtexentrytype = re.compile(r'^@(\w+)\{(.*)')   # works on            :: @article{author = {Hans Wurst},
    bibtexKeys = re.compile(r'^(\w+)\s=\s\{+([^\{\}]*)\}+,') # works on any entry :: key = {value},

    # transform bibtex keys 1:1 into the dictionary we want to return
    for line in string.split(content, "\n"):
        pp.pprint(line)
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

    # great, but there is more: fetch the abstract from the page
    abstract = documentElement.first('span', {'id':'ctl00_MainContent_PaperItem_snippet'})
    if abstract is not None:
        entry.update(dict(abstract = abstract.text))

    # TODO: parse downloadable files ... so we get informations on where to get the pdf file and so on

    return entry

#------------------------------------------------------------------------------
#  checks the global "citations" if more citation etries are available
# 
# removes the first citation and parse it item datalobal 
# asyncron load the page and call handleCitationItemExtraction
def fetchNextCitation():

    global citations
    nextCitation = citations.pop()

    # check if we can fetch references
    logMsg = 'fetch reference: ' +  nextCitation['title']
    WebExtractor.log( logMsg )

    h = httplib2.Http(".cache")
    resp, content = h.request(nextCitation['url'])

    documentElement = BeautifulSoup( content )

    # retrieve the basic data
    citation = extractItem( documentElement )

    global citationResults
    citationResults.append( citation )

if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])