class BitlockedPC:
	hostname = ""
	osname = ""
	model = ""
	serial = ""
	date = ""
	encType = ""
	campus = ""
	department = ""
	room = ""
	user = ""
	pcTech = ""
	
	def __init__(self, parameters):
			self.hostname = parameters[0]
			self.osname = parameters[1]
			self.model = parameters[2]
			self.serial = parameters[3]
			self.date = parameters[4]
			self.encType = parameters[5]
			self.campus = parameters[6]
			self.department = parameters[7]
			self.room = parameters[8]
			self.user = parameters[9]
			self.pcTech = parameters[10]

def make_PC(parameters):
	PC = BitlockedPC(parameters)
	return PC