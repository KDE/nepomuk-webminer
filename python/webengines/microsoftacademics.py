
import urllib
import re
from BeautifulSoup import BeautifulSoup
from PySide.QtCore import QObject, QUrl

from async import *
from extractor import *

#------------------------------------------------------------------------------
# Module options

# unique identification string, all lower-case no whitespace
identification = 'msa'

# the full name of this engine
name = 'Microsoft Academic Search'

# name of the icon for this engine
icon = 'microsoft-academic-search.png'

# a regular expression used to find the right engine for a specific website
regexp = r'http://academic.research.microsoft\.com/Publication/'

# what kind of nepomuk resource type can be fetched with this engine?
resourceType = 'publication'

#------------------------------------------------------------------------------
# creates the proper search query used for this side
# currently only for the title, author, freeform text and year search comes later
# we do not return more than 100 results
def searchQuery(title, author=None, freetext=None, year=None):
	title = title.replace(':','')
	query = "http://academic.research.microsoft.com/Search?query=" + title + '&start=0&end=100'
	return query

#------------------------------------------------------------------------------
# parse either the normal search result or the the list of citations
def extractSearchResults(documentElement, metaData, url):

	return extractSearchResultsExtended(documentElement, metaData, False)

#------------------------------------------------------------------------------
# extracts the single item results from the html page returned by
# doing the searchQuery()
# returns a list of dicts each dict has:
# 
# title   = name of the publication
# details = some more details on this entry (here Authors and number of available citations)
# url     = the url where to fetch the full data
def extractSearchResultsExtended(documentElement, metaData, citation=False):

	# 1. check if we didn't get any results for our query
	noResult = documentElement.first('div', {'id':'ctl00_MainContent_divNoResult'})

	if noResult:
		return None
	
	searchResults = []
	
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
# extract item info for one 1 item from a specific page
def extractItemData(documentElement, metaData, url):

	return extractItemDataExtended(documentElement, metaData, True)

#------------------------------------------------------------------------------
# extract item info for one 1 item from a specific page
# this is called by a browserr when we surfed to a specific webpage
# or as a result from a selection of search results
#
# returns a dictionary with all relevant item data
# the dictionary follows the bibtex file standard + few exceptions
#
#  bibtexentrytype =...          like book, article, inproceedings
#  references = ...              list of dicts, where each dict is another publication dict
@async
def extractItemDataExtended(documentElement, metaData, withCitation=False):

	# 1. lets get the microsoft masid again to do further requests
	# necessary, as not all data can easily be parsed from the current page content
	masid = ''
	masidTag = documentElement.first('link', {'type':'application/rss+xml'})
	if masidTag is not None:
		masidHref = masidTag['href']
		masidPart = masidHref.rpartition("id=")
		masid = masidPart[2]
	else:
		print 'Could not fetch masid.'

	finalEntry = {}
	
	# 2. we get propper publication data, this is available as bibtex file download
	# http://academic.research.microsoft.com/64764.bib?type=2&format=0&download=1
	# @todo replace urllib by async appraoch
	filehandle = urllib.urlopen('http://academic.research.microsoft.com/' + masid + '.bib?type=2&format=0&download=1')

	bibtexentrytype = re.compile(r'^@(\w+)\{.*')
	bibtexKeys = re.compile(r'^(\w+)\s=\s\{(.*)\},')

	for line in filehandle.readlines():
		typeMatch = bibtexentrytype.search(line)
		if(typeMatch):
			finalEntry.update(dict(bibtexentrytype = typeMatch.group(1)))
		keysMatch = bibtexKeys.search(line)
		if(keysMatch):
			finalEntry.update( {keysMatch.group(1) : keysMatch.group(2)})

	filehandle.close()
	
	# great, but there is more fetch the abstract from the page
	abstract = documentElement.first('span', {'id':'ctl00_MainContent_PaperItem_snippet'})
	if abstract is not None:
		finalEntry.update(dict(abstract = abstract.text))
	
	# todo parse downloadable files ...
	
	if withCitation is True:
		# and we can get the citations from it, each citation has its own page we should parse
		# http://academic.research.microsoft.com/Detail?entitytype=1&searchtype=5&id=64764&start=1&end=100
		url = 'http://academic.research.microsoft.com/Detail?entitytype=1&searchtype=5&id=' + masid + '&start=1&end=100'
		loadJob = loadUrl(url)
		html = yield loadJob.finished
                html = str(html)
		
                doc = BeautifulSoup(html)
		
		citations = extractSearchResultsExtended(doc, getMetaData(doc), True)
		
		if citations is None:
			print 'No references found.'
		
		else:
			citationResults = []
			for subPublication in citations:
				print 'Fetch reference: ' + subPublication['title']
				subLoadJob = loadUrl(subPublication['url'])
				subHtml = yield subLoadJob.finished
				subHtml = str(subHtml)

				subDoc = BeautifulSoup(subHtml)
				
				extractJob = extractItemDataExtended(subDoc, getMetaData(subDoc), False)
				result = yield extractJob.finished

				citationResults.append(result)
			
			finalEntry.update(dict(references = citationResults))

        asyncReturn(finalEntry)
	
