
#from PySide.QtGui import *
from PySide.QtCore import QObject, QUrl
from BeautifulSoup import BeautifulSoup

import sys, re, traceback
import webengines
from async import *

# selects the right module in the webengines folder by a short name
# usefull if the user wants to search with a specific engine
def selectModuleByName(moduleid):
	for module in webengines.modules:
		if re.search(module.identification, moduleid):return module
	return None

# selects the right module by the full url
# will be used by konquerer(or other browers) to find teh right plugin for a given url
def selectModuleByUrl(url):
	for module in webengines.modules:
		if re.search(module.regexp, url): return module
	return None

def availableSearchEngines(resourceType):
	
	engineList = {}
	for module in webengines.modules:
		if re.search(module.resourceType, resourceType):
			engineList.update(dict(
						identification = module.identification,
						name = module.name,
						icon = module.icon,
						regexp = module.regexp,
						resourceType = module.resourceType
						))

	return engineList

def getMetaData(doc):
	metaData = dict()
	for field in doc('meta'):
		metaData.setdefault(field.get('name'), []).append(field.get('content'))
	return metaData

@async
def loadUrl(url):
	from PySide.QtNetwork import QNetworkAccessManager, QNetworkRequest
	nm = QNetworkAccessManager()
	reply = nm.get(QNetworkRequest(QUrl(url)))
	yield reply.finished
	result = reply.readAll()
	nm.deleteLater()
	asyncReturn(result)
	
# handles a  the call to get the search result via the right module and calls
# a c++ slot at the end to return the values
@async
def asyncSearch(moduleId, title, author=None, freetext=None, year=None):

	# 1. get the right module for the name we specified
	module = selectModuleByName(moduleId)

	if not module:
		print 'No module named', moduleId
		asyncReturn(None)

	# 2. get the search query url from the module
	urlQuery = module.searchQuery(title, author, freetext, year)
  
	# 3. fetch the html page
	loadJob = loadUrl(urlQuery)
	html = yield loadJob.finished
	html = str(html)
	
	doc = BeautifulSoup(html)
	
	try:
		result = module.extractSearchResults(doc, getMetaData(doc), urlQuery)
	except Exception:
		print 'Error with parsing HTML.'
		traceback.print_exc()
		
	if not result:
		cppObj.noSearchResultsFound()
	else:
		# 4. send the variantmap back to c++
		cppObj.searchResults(result)

def search(url, html=None):
	asyncSearch(url, html)

@async
def asyncExtract(url, html=None):

	# 1. get the right module for the name we specified
	module = selectModuleByUrl(url)

	if not module:
		print 'No module for url', url
		asyncReturn()

	# 3. fetch the html page
	loadJob = loadUrl(url)
	html = yield loadJob.finished
	html = str(html)
	
	doc = BeautifulSoup(html)

	try:
		if module.asyncUsage is 'true':
			extractJob = module.extractItemData(doc, getMetaData(doc), url)
			result = yield extractJob.finished
		else:
			result = module.extractItemData(doc, getMetaData(doc), url)
		
		# 4. send the variantmap back to c++
		cppObj.itemResult(result)
	except Exception:
		print 'Error with parsing HTML.'
		traceback.print_exc()


def extract(url, html=None):
	asyncExtract(url, html)
	

'''
def runSearch(module, title):
	app = QApplication([])
	extractJob = asyncSearch(module, title)
	def quitSlot(args): app.quit()
	extractJob.finished.connect(quitSlot)
	app.exec_()
	
def runExtract(url):
	app = QApplication([])
	extractJob = asyncExtract(url)
	def quitSlot(args): app.quit()
	extractJob.finished.connect(quitSlot)
	app.exec_()

if __name__ == '__main__':

	#print availableSearchEngines('movie')

	module = 'imdb'
	title = 'Matrix'
	
	runSearch(module, title)

	
	#url = 'http://academic.research.microsoft.com/Publication/64764'
	#runExtract(url)
'''
