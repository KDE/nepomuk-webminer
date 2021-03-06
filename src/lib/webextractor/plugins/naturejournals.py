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
    errorMsg = "Module requirements missing. naturejournals.py needs:\nThe BeautifulSoup python module. see: http://www.crummy.com/software/BeautifulSoup/\nThe Httplib2 python module. See: http://code.google.com/p/httplib2/"

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
    return dict( name = 'Nature Publishing Group',
                 homepage = 'http://www.nature.com',
                 icon = 'naturejournals.png',
                 identifier = 'nature',
                 resource = ['publication'],
                 urlregex = ['http://www.nature\.com/\w+/journal/'],
                 description = 'Nature Publishing Group (NPG) is a publisher of high impact scientific and medical information in print and online. NPG publishes journals, online databases, and services across the life, physical, chemical and applied sciences and clinical medicine.',
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
        searchQuery = 'sp-q=' + title
    if author and author != "":
        searchQuery = searchQuery + '&sp-q-2=' + author

    WebExtractor.log( 'Start query via: ' + searchQuery )

    h = httplib2.Http("/tmp/httplib2")
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
                            url = pubLink,
                            authors = pubAuthors,
                            publicationtype = pubType,
                            date = pubDate,
                            plaintext = '',
                            distanceString = pubTitle
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
# bibreferences = ...           list of dicts, where each dict is another publication dict
#
def extractItemFromUri( url, options ):

    logMsg = 'start item extraction via: ' + url 
    WebExtractor.log( logMsg )

    h = httplib2.Http("/tmp/httplib2")
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
        abstract = metaData.get('description')[0] if metaData.get('description') else "",
        #archive = ,
        copyright = metaData.get('prism.copyright')[0] if metaData.get('prism.copyright') else "",
        doi = metaData.get('citation_doi')[0] if metaData.get('citation_doi') else "",
        #edition = ,
        #isbn = ,
        issn = metaData.get('prism.issn')[0] if metaData.get('prism.issn') else "",
        language = metaData.get('DC.language')[0] if metaData.get('DC.language') else "",
        number = metaData.get('citation_issue')[0] if metaData.get('citation_issue') else "",
        pages = metaData.get('prism.startingPage')[0] if metaData.get('prism.startingPage') else "" + '--' + metaData.get('prism.endingPage')[0] if metaData.get('prism.endingPage') else "" ,
        volume = metaData.get('citation_volume')[0] if metaData.get('citation_volume') else "",
        date = metaData.get('citation_date')[0] if metaData.get('citation_date') else "",
        author = metaData.get('citation_authors')[0] if metaData.get('citation_authors') else "",
        journal = metaData.get('citation_journal_title')[0] if metaData.get('citation_journal_title') else "",
        publisher = metaData.get('citation_publisher')[0] if metaData.get('citation_publisher') else "",
        #school = ,
        #series = ,
        #event = ,
        title = metaData.get('citation_title')[0] if metaData.get('citation_title') else "",
        url = url,
        biburl = publicationPDF,
        #references = ,
        keywords = metaData.get('keywords')[0] if metaData.get('keywords') else "") 

    data_string = json.dumps(finalEntry)
    WebExtractor.itemResultsJSON( 'publication', data_string )


if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])