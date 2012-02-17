import re

# In order to keep the extractor plugins simple, different date, author
# list etc. formats are accepted. The following is to handle them.

def processDate(date):
	monthNames = dict(
		Jan=1, January=1,
		Feb=2, February=2,
		Mar=3, March=3,
		Apr=4, April=4,
		May=5, 
		Jun=6, June=6,
		Jul=7, July=7,
		Aug=8, August=8,
		Sep=9,September=9,
		Oct=10, October=10,
		Nov=11, November=11,
		Dec=12, December=12)

	# syntax 1: yyyy-mm-dd or yyyy/mm/dd
	exp1 = re.compile(r"""\s* #
	(?P<year>\d{4})     #
	(?:[-/]             # month and day can be absent
	(?P<month>\d{1,2})  #
	[-/]                #
	(?P<day>\d{1,2})    #
	)                   #
	""", re.VERBOSE)

	# syntax 2: mm-dd-yyyy or mm/dd/yyyy
	exp2 = re.compile(r"""\s* #
	(?P<month>\d{1,2})  #
	[-/]                #
	(?P<day>\d{1,2})    #
	[-/]                #
	(?P<year>\d{4})     #
	""", re.VERBOSE)

	# syntax 3: dd Mon yyyy
	exp3 = re.compile(r"""\s* #
	(?P<day>\d{1,2})\S*\s #
	(?P<month>\w+)\.?\s   #
	(?P<year>\d{4})       #
	""", re.VERBOSE)

	match = exp1.match(date)
	if not match: match = exp2.match(date)
	if not match: match = exp3.match(date)
	if not match: return None
	year, month, day = match.group('year', 'month', 'day')
	if not month: return [int(year)]
	if month in monthNames: return [int(year), monthNames[month], int(day)]
	else: return [int(year), int(month), int(day)]


def processAuthors(authors):
	# raise TypeError if authors is not iterable
	iter(authors)

	# syntax 1: von Reubald, H. W.
	exp1 = re.compile(r"""\s*   #
	(?P<lastName>.+?)\s*,       #
	\s*(?P<firstName>.+?)\s*$   #
	""", re.VERBOSE)
	# syntax 2: H. W. von Reubald
	exp2 = re.compile(r"""\s*          #
	(?P<firstName>(?:[^a-z\s]\S*\s*)*) #
	(?<!\s) \s+                        #
	(?P<lastName>.+?)\s*$              #
	""", re.VERBOSE)

	result = list()
	for author in authors:
		match = exp1.match(author)
		if not match: match = exp2.match(author)
		if not match: return None
		firstname = match.group('firstName')
		lastname = match.group('lastName')
		if lastname.isupper():
			# Sometimes last names are in capitals....
			lastname = lastname[0] + lastname[1:].lower()
		result.append((firstname, lastname))
	return result

def processDoi(doi):
	if '10.' not in doi: return None
	return str(doi[doi.index('10.'):].strip())

def processResult(result):
	try:
		if 'authors' in result:
			result['authors'] = processAuthors(result['authors'])
		if 'title' in result:
			result['title'] = unicode(result['title']).strip()
		if 'date' in result:
			result['date'] = processDate(result['date'])
		if 'volume' in result:
			result['volume'] = int(result['volume'])
		if 'issue' in result:
			result['issue'] = int(result['issue'])
		if 'firstPage' in result:
			result['firstPage'] = int(result['firstPage'])
		if 'lastPage' in result:
			result['lastPage'] = int(result['lastPage'])
		if 'publisher' in result:
			result['publisher'] = unicode(result['publisher'])
		if 'doi' in result:
			result['doi'] = processDoi(result['doi'])
		if 'type' in result:
			result['type'] = str(result['type'])
		else: result['type'] = 'article'
		return result
	except TypeError: 
		print 'Invalid data from plugin...'
		print result
		return None

