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
    return dict( name = 'Springer Link',
                 homepage = 'http://www.springerlink.com',
                 icon = 'springer.png',
                 identifier = 'springer',
                 resource = ['publication'],
                 urlregex = ['http://www.springerlink.com/content/[0-9A-Za-z]+/'],
                 description = 'SpringerLink is an integrated full-text database for journals, books, protocols, eReferences, and book series published by Springer.',
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
    # TODO: Springerlink shows 10 results per page only, need to recursively parse the otehr pages
    if title and title != "":
        #searchQuery = 'ti:(' + title.decode("utf8") + ')'
        searchQuery = title.decode("utf8")
    if author and author != "":
        searchQuery = searchQuery + '+au:(' + author + ')'

    WebExtractor.log( 'Start query via: ' + searchQuery )

    h = httplib2.Http("/tmp/httplib2")
    resp, content = h.request('http://www.springerlink.com/content/?k=' + urllib2.quote(searchQuery.encode("utf8")) )

    documentElement = BeautifulSoup( content )

    items = documentElement.findAll('ul', id="PrimaryManifest")

    searchResults = []

    if items is not None:
        for i in items:
            searchResultsList = i.findAll('li', "primitive")

            for sr in searchResultsList:

                itemType = sr.find('p', "itemTag").string

                href = sr.find('a')
                itemTitle = ''.join([e for e in href.recursiveChildGenerator() if isinstance(e,unicode)])

                abstractString = ''
                abstractType = sr.find('p', "snippets")
                if abstractType is not None:
                    abstractString = ''.join([e for e in abstractType.recursiveChildGenerator() if isinstance(e,unicode)])

                itemLink = href['href']
                authorsType = sr.find('p', "authors")
                authorList = authorsType.findAll("a")
                itemAuthors = "; ".join(a.string for a in authorList)

                entryDict = dict(
                                title = itemTitle,
                                details = itemType + ' - ' + itemAuthors,
                                url = 'http://www.springerlink.com' + itemLink,
                                authors = itemAuthors,
                                plaintext = abstractString,
                                publicationtype = str(itemType),
                                date = ''
                                )

                searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

#------------------------------------------------------------------------------
# extracts the information of one item from the page at the url location
#
# the dictionary follows the bibtex file standard + few exceptions
#
# bibtexentrytype =...          like book, article, inproceedings // necessary for the PublicationPipe to work properly
# bibreferences = ...              list of dicts, where each dict is another publication dict
#
def extractItemFromUri( url, options ):

    logMsg = 'start item extraction via: ' + url 
    WebExtractor.log( logMsg )

    h = httplib2.Http("/tmp/httplib2")

    #--------------------------------------------------------
    # fetch the abstract page first
    resp, content = h.request( url + '/abstract' )

    documentElement = BeautifulSoup( content )

    abstract = ""
    abstractElements = documentElement.find('div', class_="Abstract")
    if abstractElements is not None:
        abstract = abstractElements.string
        if abstract is None:
            abstract = ''.join([e for e in abstractElements.recursiveChildGenerator() if isinstance(e,unicode)])


    #--------------------------------------------------------
    # instead of parsing the table part of the about page we parse the information from
    # the encoded details on the top of the page
    journal = ""
    volume = ""
    issue = ""
    pages = ""
    doi = ""
    title = ""
    authors = ""
    itemType = ""

    journalNameEle = documentElement.find("a", title="Link to the Journal of this Article")
    if journalNameEle is not None:
        journal = journalNameEle.string

    issueNameEle = documentElement.find("a", title="Link to the Issue of this Article")
    if issueNameEle is not None:
        exp = re.compile(r'Volume (\d+), Number (\d+)')
        match = exp.search(issueNameEle.string)
        if match:
            volume, number = match.groups()

    pagesEle = documentElement.find("span", class_="pagination")
    if pagesEle is not None:
        pages = pagesEle.string

    doiEle = documentElement.find("span", class_="value")
    if doiEle is not None:
        doi = doiEle.string

    titleEle = documentElement.find("a", title="Link to Article")
    if titleEle is not None:
        itemType = "article"
        title = ''.join([e for e in titleEle.recursiveChildGenerator() if isinstance(e,unicode)])

    titleEle = documentElement.find("a", title="Link to Book")
    if titleEle is not None:
        itemType = "book"
        title = ''.join([e for e in titleEle.recursiveChildGenerator() if isinstance(e,unicode)])

    authorsEle = documentElement.find("p", class_="authors")
    if authorsEle is not None:
        title = ';'.join([e for e in authorsEle.recursiveChildGenerator() if isinstance(e,unicode)])



    #--------------------------------------------------------
    #Now fetch the item publication details
    resp, content = h.request( url + '/about' )

    documentElement = BeautifulSoup( content )

    finalEntry = dict(
        title = title,
        authors = authors,
        journal = journal,
        volume = volume,
        issue = issue,
        pages = pages,
        abstract = abstract,
        #date = date,
        doi = doi) 

    data_string = json.dumps(finalEntry)
    WebExtractor.itemResultsJSON( 'publication', data_string )


if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])