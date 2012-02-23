#from PyQt4.QtCore import *
from PySide.QtCore import QObject, QTimer, Signal
import traceback

# An AsyncJob represents a running asyncronous function.
# When the function has finished, the finished signal is emitted.
# The signal arguments carry the return value(s) 
class AsyncJob(QObject):
	
	#finished = pyqtSignal(tuple)
	finished = Signal(tuple)
	
	def isFinished(self): return self.returnValues != None
	def returnValues(): return self.returnValues
	
	# The rest is for internal use only...
	
	instances = set()
	
	def __init__(self, generator):
		QObject.__init__(self)
		self.returnValues = None
		self.signal = None
		self.generator = generator
		self.instances.add(self) # Avoid garbage collection
		QTimer.singleShot(0, self.slot)
		
	def slot(self, *args):
		try:
			if self.signal:
				self.signal.disconnect(self.slot)
			
			# Execute the next code segment
			self.signal = self.generator.send(self.unwrapSingletons(args))
			if not isinstance(self.signal, Signal):
				self.generator.close()
				raise Exception('Async function cannot be resumed: no valid signal.')
			
			self.signal.connect(self.slot)
			
		except StopIteration as i:
			# Function finished
			self.finish(*i.args)
			return
			
		except Exception as e:
			traceback.print_exc()
			self.finish(e)
			
	def finish(self, *values):
		self.returnValues = values
		#self.signal.disconnect(self.slot)
		if len(values) == 0: self.finished.emit(None) 
		else: self.finished.emit(*values)
		self.instances.remove(self)
	
	def unwrapSingletons(self, values):
		if len(values) == 0: return None
		elif len(values) == 1: return values[0]
		else: return values


# Function decorator (@async)
def async(function):
	def newfunc(*args, **kwargs):
		return AsyncJob(function(*args, **kwargs))
	return newfunc

	
# Use this instead of return in @async functions
def asyncReturn(value): raise StopIteration(value)
