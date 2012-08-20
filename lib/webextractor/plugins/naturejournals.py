#!/usr/bin/env kross

import json
import re

# Prerrequisites.
isAvailable = True
errorMsg = ""
try:
    import urllib2
    import httplib2
    from BeautifulSoup import BeautifulSoup

except:
    isAvailable = False
    errorMsg = "Module requirements missing. springer.py needs:\nThe BeautifulSoup python module. see: http://www.crummy.com/software/BeautifulSoup/\nThe Httplib2 python module. See: http://code.google.com/p/httplib2/"

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
    return dict( name = 'Nature Publishing Group',
                 icon = 'naturejournals.png',
                 identifier = 'nature',
                 resource = ['publication'],
                 urlregex = ['http://www.nature\.com/\w+/journal/'],
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
    searchQuery = ""

    # TODO: implement volume/issue/year may and year min
    if title and title != "":
        searchQuery = u'sp-q=' + title
    if author and author != "":
        searchQuery = searchQuery + u'&sp-q-2=' + author

    WebExtractor.log( 'Start query via: ' + searchQuery )

    h = httplib2.Http(".cache")
    resp, content = h.request('http://www.nature.com/search/executeSearch?sp-advanced=true&' + searchQuery)

    documentElement = BeautifulSoup( content )

    items = documentElement.findAll('ol', "results-list")

    searchResults = []

    for i in items:
        searchResultsList = i.findAll('li', recursive=False)

        for sr in searchResultsList:

            itemDate = sr.find('span', "date")
            pubDate = ""
            if itemDate is not None:
                pubDate = itemDate.string
                pubDate = " ".join(pubDate.split())

            href = sr.find('a', "result-title")
            pubLink = ""
            pubTitle = ""
            if href is not None:
                pubLink = href['href']
                pubTitle = ''.join([e for e in href.recursiveChildGenerator() if isinstance(e,unicode)])
                pubTitle = " ".join(pubTitle.split())

            authorList = sr.find("ul", "authors")
            pubAuthors = ""
            if authorList is not None:
                pubAuthors = ';'.join([e for e in authorList.recursiveChildGenerator() if isinstance(e,unicode)])
                pubAuthors = " ".join(pubAuthors.split())
                pubAuthors = re.sub(r'\s;\s', ' ', pubAuthors)

            pubJournal = ""
            itemJournal = sr.find("span", "journal-title")
            if itemJournal is not None:
                pubJournal = sr.find("span", "journal-title").string

            itemVolume = sr.find("span", "journalnumber")
            pubVolume = ""
            if itemVolume is not None:
                pubVolume = itemVolume.string

            pubType = u'Article'

            detailsString = ""
            if pubVolume != "":
                detailsString = pubType + ':' + pubJournal + ' ('+ pubVolume +'), ' + pubDate +'; ' + pubAuthors
            else:
                detailsString = pubType + ':' + pubJournal + ', ' + pubDate +'; ' + pubAuthors

            #we ignore the page numbers and doi string here, not really of interrest in the search overview

            entryDict = dict(
                            title = pubTitle,
                            details = detailsString,
                            url = pubLink
                            )

            searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

#------------------------------------------------------------------------------
# Create a dictionary of all metadata in the html header
# @author tuukka verho
def getMetaData(doc):
    metaData = dict()
    for field in doc('meta'):
        metaData.setdefault(field.get('name'), []).append(field.get('content'))
    return metaData

#------------------------------------------------------------------------------
# extracts the information of one item from the page at the url location
#
# the dictionary follows the bibtex file standard + few exceptions
#
# bibtexentrytype =...          like book, article, inproceedings // necessary for the PublicationPipe to work properly
# references = ...              list of dicts, where each dict is another publication dict
#
def extractItemFromUri( url, options ):

    logMsg = 'start item extraction via: ' + url 
    WebExtractor.log( logMsg )

    h = httplib2.Http(".cache")
    resp, content = h.request( url )

    documentElement = BeautifulSoup( content )
    metaData = getMetaData(documentElement)

    publicationPDF = ""
    fileElements = documentElement.find('div', "box supp-info")
    if fileElements is not None:
        fileHref = fileElements.find('a')
        if fileHref is not None:
            publicationPDF = 'http://www.nature.com' + fileHref['href']

    finalEntry = dict(
        bibtexentrytype = 'article',
        abstract = metaData.get('description')[0],
        #archive = ,
        copyright = metaData.get('prism.copyright')[0],
        doi = metaData.get('citation_doi')[0],
        #edition = ,
        #isbn = ,
        issn = metaData.get('prism.issn')[0],
        language = metaData.get('DC.language')[0],
        number = metaData.get('citation_issue')[0],
        pages = metaData.get('prism.startingPage')[0] + '--' + metaData.get('prism.endingPage')[0],
        volume = metaData.get('citation_volume')[0],
        date = metaData.get('citation_date')[0],
        author = metaData.get('citation_authors')[0],
        journal = metaData.get('citation_journal_title')[0],
        publisher = metaData.get('citation_publisher')[0],
        #school = ,
        #series = ,
        #event = ,
        title = metaData.get('citation_title')[0],
        url = url,
        biburl = publicationPDF,
        #references = ,
        keywords = metaData.get('keywords')[0]) 

    data_string = json.dumps(finalEntry)
    WebExtractor.itemResultsJSON( 'publication', data_string )


if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])