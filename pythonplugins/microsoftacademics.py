
import urllib
from array import array
import re

moduleid = 'msa'
moduleName = 'Microsoft Academic Search'
regexp = r'http://academic.research.microsoft\.com/Publication/'

# creates the proper search query used for this side
# currently only for the title, author freeform text search comes laters
# we do not return more than 100 results
def searchQuery(title, author=None, freetext=None):
	query = "http://academic.research.microsoft.com/Search?query=" + title + '&start=0&end=100'
	return query

# extracts the single item results from the html page returned by doing the searchQuery()
# returns a list of dicts
# each dict has:
#              title = name of the publication
#              url   = the url where to fetch the full data
def extractSearchResults(documentElement, metaData):

	# 1. check if we didn't get any any results for our query
	exp = re.compile(r'We did not find any result related to')
	match = exp.search(documentElement.toPlainText())
	if match:
		print "no search result found"
		return None

	# 2. get the number of results we retrieved
	mainContent = documentElement.findFirst('a#ctl00_MainContent_PaperList_ctl00_HeaderLink')
	itemCount = mainContent.findFirst('span').toPlainText()
	itemCount = itemCount.replace('(','')
	itemCount = itemCount.replace(')','')

	print "item count :" + itemCount
	itemCount = int(itemCount)
	if(itemCount > 100):
	   itemCount = 100

	searchResults = []
	i = 1
	# now get for each result the msai and name
	while itemCount + 1 > i:
		if(itemCount < 10):
			fetchentry = '0' + str(i)
		else:
			fetchentry = str(i)

		entryTitle = documentElement.findFirst('a#ctl00_MainContent_PaperList_ctl' + fetchentry + '_Title')
	
		linkPart = documentElement.findFirst('a#ctl00_MainContent_PaperList_ctl' + fetchentry + '_Title').attribute('href')

		exp = re.compile(r'Publication/(\d+)/', re.VERBOSE)
		match = exp.search(linkPart)
		if not match:
			print 'Error parsing publication msai'
		msai = match.group(1)

		fullUrl = 'http://academic.research.microsoft.com/Publication/' + msai

		entryDict = dict(
				title = entryTitle.toPlainText(),
				url = fullUrl
				)

		searchResults.append(entryDict)

		i += 1

	return searchResults

# extarct item info for one 1 fro m aspecific page
# this is called by a browerser when we surfed to a specific webpage
# or as a result from a selection of search results
def extractItemData(documentElement, metaData):

	# 1. lets get the microsoft msaid again to do further requests
	# necessary, as not all data can easily be parsed from teh current page content
	msaidHref = documentElement.findFirst('link[type=application\/rss\+xml]').attribute('href')
	msaidPart = msaidHref.rpartition("id=")
	msaid = msaidPart[2]

	finalEntry = {}
	# 2. we get propper publication data, this is available as bibtex file download
	# http://academic.research.microsoft.com/64764.bib?type=2&format=0&download=1

	filehandle = urllib.urlopen('http://academic.research.microsoft.com/' + msaid + '.bib?type=2&format=0&download=1')

	bibtexentrytype = re.compile(r'^@(\w+)\{.*', re.VERBOSE)
	bibtexKeys = re.compile(r'^(\w+)\s=\s\{(.*)\},', re.VERBOSE)

	for line in filehandle.readlines():
		typeMatch = bibtexentrytype.search(line)
		if(typeMatch):
			finalEntry.update(dict(bibtexentrytype = typeMatch.group(1)))
		keysMatch = bibtexKeys.search(line)
		if(keysMatch):
			finalEntry.update( {keysMatch.group(1) : keysMatch.group(2)})

	filehandle.close()
	
	# great, but there is more fetch the abstract from the page
	abstract = documentElement.findFirst('span#ctl00_MainContent_PaperItem_snippet')
	finalEntry.update(dict(abstract = abstract.toPlainText()))
	
	# todo parse downloadable files ...
	
	# and we can get the citations from it, each citation has its own page we should parse
	# todo again
	
	return finalEntry
