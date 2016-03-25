import os
from shutil import copyfile

def insertCodeInSource(destination, code, marker):
	with open(destination, "r+") as destfile:
		content = destfile.readlines()
		pos = determineInsertPosition(content, marker)
		content.insert(pos, code + "\n")
		destfile.truncate()
		destfile.seek(0)
		destfile.writelines(content)
			
def determineInsertPosition(code, marker):
	pos = 0
	for line in code:
		pos = pos + 1
		if marker in line:
			return pos - 1
			
def readTemplateFile(filename):
	with open(os.path.join("template", filename), "r+") as templateFile:
		template = templateFile.read()
		return template


class Input:
	commandName = "MyCommand"
		
class ProcessorBase(object):
	def __init__(self, destination, templateInfos):
		self.destination = destination
		self.templateInfos = templateInfos
	
	def process(self):
		for templateInfo in self.templateInfos:
			filename = templateInfo["filename"]
			marker = templateInfo["marker"]
			template = readTemplateFile(filename)
			code = self.processTemplate(filename, template)
			insertCodeInSource(self.destination, code, marker)
			
	def processTemplate(self, filename, template):
		raise NotImplementedError("Subclass must implement abstract method")
	
	

class VclH(ProcessorBase):
	def __init__(self):
		super(VclH, self).__init__("../src/commands/Vcl.h", [
			{ "filename":"VclTemplate.h", "marker":"#undef PYTHON_ENUM" },
			{ "filename":"VclTemplate2.h", "marker":"#undef PYTHON_MAGIC_STRING" }])

	def processTemplate(self, filename, template):
		return template.replace("{VclType}", Input.commandName)
	


def main():
	objs = [ VclH() ] 
	for obj in objs:
		obj.process()
	
main()
