#from PyQt4.QtCore import *
from PySide.QtCore import *
import traceback

# An AsyncJob represents a running asyncronous function.
# When the function has finished, the finished signal is emitted
# with a possible return value (tuple)
class AsyncJob(QObject):
	
	#finished = pyqtSignal(tuple)
	finished = Signal(tuple)
	
	def isFinished(self): return self.returnValue != None
	def returnValue(): return self.returnValue
	
	# The rest is for internal use only...
	
	instances = set()
	
	def __init__(self, generator):
		QObject.__init__(self)
		self.returnValue = None
		self.generator = generator
		QTimer.singleShot(0, self.run)
		self.instances.add(self) # Avoid garbage collection
		
	def run(self):
		self.signal = self.generator.next()
		self.signal.connect(self.slot)

	def slot(self, *args):
		try:
			# Execute the next code segment
			sig = self.generator.send(args)
			if not sig: return # Keep on listening to the same signal
			self.signal.disconnect(self.slot)
			self.signal = sig
			self.signal.connect(self.slot)
			
		except StopIteration as i:
			# Function finished
			self.finish(*i.args)
			
		except Exception as e:
			traceback.print_exc()
			self.finish(e)
			
	def finish(self, *returnValue):
		self.returnValue = returnValue
		self.signal.disconnect(self.slot)
		self.finished.emit(self.returnValue)
		self.instances.remove(self)
		del self.generator


# Function decorator (@async)
def async(function):
	def newfunc(*args, **kwargs):
		return AsyncJob(function(*args, **kwargs))
	return newfunc

	
# Use this instead of return in @async functions
def asyncReturn(*values): raise StopIteration(*values)
