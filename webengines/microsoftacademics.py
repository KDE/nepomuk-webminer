
import urllib
import re

#------------------------------------------------------------------------------
# Module options

# unique identification string, all lower-case no whitespace
identification = 'msa'

#the full name of this engine
name = 'Microsoft Academic Search'

#name of the icon for this engine
icon = 'microsoft-academic-search.png'

#a regular expression used to find the right engine for a specific website
regexp = r'http://academic.research.microsoft\.com/Publication/'

#what kind of nepomuk resource type can be fetched with this engine?
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
# extracts the single item results from the html page returned by
# doing the searchQuery() returns a list of dicts each dict has:
# 
# title   = name of the publication
# details = some more details on this entry (here Authors, year)
# url     = the url where to fetch the full data
def extractSearchResults(documentElement, metaData):

	# 1. check if we didn't get any results for our query
	noResult = documentElement.first('div', {'id':'ctl00_MainContent_divNoResult'})

	if noResult:
		print "no search result found"
		return None

	# 2. get the number of results we retrieved
	mainContent = documentElement.first('a', {'id':'ctl00_MainContent_PaperList_ctl00_HeaderLink'})
	itemCount = mainContent.first('span').text
	itemCount = itemCount.replace('(','')
	itemCount = itemCount.replace(')','')

	print "item count :" + itemCount
	itemCount = int(itemCount)
	# we do not return more than 100 results
	if(itemCount > 100):
	   itemCount = 100

	searchResults = []
	i = 1
	# now get for each result the masid and name
	while itemCount + 1 > i:
		if(itemCount < 10):
			fetchentry = '0' + str(i)
		else:
			fetchentry = str(i)

		entryTitle = documentElement.first('a', {'id':'ctl00_MainContent_PaperList_ctl' + fetchentry + '_Title'})
		
		#details = documentElement.first('span', {'id':'ctl00_MainContent_PaperList_ctl' + fetchentry + '_snippet'})
	
		linkPartTag = documentElement.first('a', {'id':'ctl00_MainContent_PaperList_ctl' + fetchentry + '_Title'})
		if linkPartTag:
			linkPart = linkPartTag['href']

			exp = re.compile(r'Publication/(\d+)/')
			match = exp.search(linkPart)
			if not match:
				print 'Error parsing publication masid'
			masid = match.group(1)

			fullUrl = 'http://academic.research.microsoft.com/Publication/' + masid

			entryDict = dict(
					title = entryTitle.text,
					url = fullUrl
					)

			searchResults.append(entryDict)

		i += 1

	return searchResults

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
def extractItemData(documentElement, metaData):

	# 1. lets get the microsoft masid again to do further requests
	# necessary, as not all data can easily be parsed from the current page content
	masidTag = documentElement.first('link', {'type':'application/rss+xml'})
	masidHref = masidTag['href']
	masidPart = masidHref.rpartition("id=")
	masid = masidPart[2]
	
	print 'masid' + masid

	finalEntry = {}
	
	# 2. we get propper publication data, this is available as bibtex file download
	# http://academic.research.microsoft.com/64764.bib?type=2&format=0&download=1
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
	finalEntry.update(dict(abstract = abstract.text))
	
	# todo parse downloadable files ...
	
	# and we can get the citations from it, each citation has its own page we should parse
	# todo again
	
	return finalEntry
