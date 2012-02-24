import os


modules = []

for filename in os.listdir(__path__[0]):
	if filename[0] is not '.' and filename[-3:] == '.py' and '__' not in filename:
		modulename = filename.split('.')[0]
		exec('import ' + modulename)
		exec('modules.append('+modulename+')')
