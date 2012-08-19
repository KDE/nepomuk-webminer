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
                 icon = 'microsoft-academic-search.png',
                 identifier = 'springer',
                 resource = ['publication'],
                 urlregex = ['http://www.springerlink.com/content/[0-9A-Za-z]/'],
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
        searchQuery = u'ti:(' + title + ')'
    if author and author != "":
        searchQuery = searchQuery + u'+au:(' + author + ')'

    WebExtractor.log( 'Start query via: ' + searchQuery )

    h = httplib2.Http(".cache")
    resp, content = h.request('http://www.springerlink.com/content/?k=' + urllib2.quote(searchQuery.encode("utf8")) )

    documentElement = BeautifulSoup( content )

    items = documentElement.findAll('ul', id="PrimaryManifest")

    searchResults = []

    for i in items:
        searchResults = i.findAll('li', "primitive")

        for sr in searchResults:

            itemType = sr.find('p', "itemTag").string

            href = sr.find('a')
            hrefTitle = href.string
            itemTitle = ''

            if hrefTitle is None:
                itemTitle = ''.join([e for e in href.recursiveChildGenerator() if isinstance(e,unicode)])
            else:
                itemTitle =  itemTitle

            itemLink = href['href']
            authorsType = sr.find('p', "authors")
            authorList = authorsType.findAll("a")
            itemAuthors = ";".join(a.string for a in authorList)

            entryDict = dict(
                            title = itemTitle,
                            details = itemType + ' - ' + itemAuthors,
                            url = 'http://www.springerlink.com/' + itemLink
                            )

            searchResults.append(entryDict)

    WebExtractor.searchResults( searchResults )

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

    # taken from tuuka 
    div = de.find('div', {'class':'primary'})
    journal = div.a.text

    div = div.nextSibling
    volume_number = div.a.text
    exp = re.compile(r'Volume (\d+), Number (\d+)')
    match = exp.search(volume_number)
    if not match:
        WebExtractor.error('Error parsing volume and number: ' + volume_number )
        return None
    volume, number = match.groups()

    pages = div.find('span', {'class':'pagination'}).text
    firstPage, lastPage = pages.split('-')
    
    doi = div.find('span', {'class':'doi'}).find('span', {'class':'value'}).text

    div = de.find('div', {'class':'text'})  
    title = div.h1.text
    authors = [a.text for a in div.p.findAll('a')]

    pdf = de.find('div', {'class':'heading'}).ul.li.a.get('href')

    dateContent = de.find('li', {'class':'rightslink'}).a.get('href')
    exp = re.compile(r'publicationDate=(.+?)&')
    match = exp.search(dateContent)
    if not match:
        WebExtractor.error('Error parsing date: ' + dateContent )
        return None
    date = match.group(1).replace('%2f', '-')

    finalEntry = dict(
        title = title,
        authors = authors,
        journal = journal,
        volume = volume,
        issue = number,
        pages = firstPage + '-' + lastPage,
        date = date,
        doi = doi,
        fulltextPdf = pdf) 

    data_string = json.dumps(finalEntry)
    WebExtractor.itemResultsJSON( 'publication', data_string )


if __name__=="__main__":
    print "Plugin information:"
    pluginInfo = info()
    for key in pluginInfo:
        print "%15s: %s" % (key, pluginInfo[key])