import os

files = os.listdir(__path__[0])
modulenames = []
modules = []

for filename in files:
	if filename[0] is not '.' and filename[-3:] == '.py' and '__' not in filename:
		modulenames.append(filename.split('.')[0])

for modulename in modulenames:
	exec 'import ' + modulename
	exec 'modules.append('+modulename+')'