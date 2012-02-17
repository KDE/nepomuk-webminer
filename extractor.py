

from PySide.QtCore import *
from PySide.QtGui import *
from PySide.QtWebKit import *

import sys, re, traceback
import webengines
import resultProcessing 

from async import *

# selects the right module in the webengines folder by a short name
# usefull if the user wants to search with a specific engine
def selectModuleByName(moduleid):
	for module in webengines.modules:
		if re.search(module.moduleid, moduleid): return module
	return None

# selects the right module by the full url
# will be used by konquerer(or other browers) to find teh right plugin for a given url
def selectModuleByUrl(url):
	for module in webengines.modules:
		if re.search(module.regexp, url): return module
	return None


# handles a  the call to get the search result via the right module and calls
# a c++ slot at the end to return the values
@async
def asyncSearch(moduleId, title, author=None, freetext=None):

	print 'started python part for the searching'
	# 1. get the right module for the name we specified
	module = selectModuleByName(moduleId)

	if not module:
		print 'No module named', moduleId
		asyncReturn()
	else:
		print 'Start search via:', module.moduleName

	# 2. get the search query url from the module
	urlQuery = module.searchQuery(title, author, freetext)
	
	print 'Used search query', urlQuery
  
	# 3. fetch the html page
	page = QWebPage()
	frame = page.mainFrame()
	page.settings().setAttribute(QWebSettings.AutoLoadImages, False)
	frame.load(QUrl(urlQuery))

	yield page.loadFinished
	print 'Page loaded:'

	try:
		result = module.extractSearchResults(frame.documentElement(), frame.metaData())
		if not result:
			cppObj.noSearchResultsFound()
		else:
			# 4. send the variantmap back to c++
			cppObj.searchResults(result)
			
	except Exception:
		print 'Error with parsing HTML.'
		traceback.print_exc()
		cppObj.noSearchResultsFound()

	# PyQt doesn't handle deletion of Qt objects properly
	# Removing these lines leads to segfaults later
	page.deleteLater()

	
# start the search from c++ part
def search(moduleId, title, author=None, freetext=None):
	asyncSearch(moduleId, title, author, freetext)


@async
def asyncExtract(url, html=None):

	# 1. get the right module for the name we specified
	module = selectModuleByUrl(url)

	if not module:
		print 'No module for url', url
		asyncReturn()
	else:
		print 'Start extraction via:', module.moduleName

	# 3. fetch the html page
	page = QWebPage()
	frame = page.mainFrame()

	if not html:
		page.settings().setAttribute(QWebSettings.AutoLoadImages, False)
		frame.load(QUrl(url))
	else:
		frame.setHtml(html)

	yield page.loadFinished
	print 'Page loaded:' + url

	try:
		result = module.extractItemData(frame.documentElement(), frame.metaData())
	except Exception:
		print 'Error with parsing HTML.'
		traceback.print_exc()

	# 4. send the variantmap back to c++
	cppObj.itemResult(result)

	# PyQt doesn't handle deletion of Qt objects properly
	# Removing these lines leads to segfaults later
	page.deleteLater()

# extract item results for 1 item from the given url
# html might be prefetched by a browser, otherwise we fetch it again
def extract(url, html=None):
	asyncExtract(url, html)

