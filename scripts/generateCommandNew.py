import os
import argparse
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


class Types:
	proc = 1
	io = 2
		
class Input:
	def __init__(self, commandName, type, hasReply):
		self.commandName = commandName
		self.hasReply = hasReply
		if (type == "proc"):
			self.type = Types.proc
		elif (type == "io"):
			self.type = Types.io
		else:
			raise Exception("Invalid type: must be 'proc' or 'io'")
		
	
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
	def __init__(self, input):
		super(VclH, self).__init__("../src/commands/Vcl.h", [
			{ "filename":"VclTemplate.h", "marker":"#undef PYTHON_ENUM" },
			{ "filename":"VclTemplate2.h", "marker":"#undef PYTHON_MAGIC_STRING" }])
		self.input = input

	def processTemplate(self, filename, template):
		return template.replace("{VclType}", self.input.commandName)
	

class CommandFactoryH(ProcessorBase):
	def __init__(self, input):
		if input.type == Types.proc:
			filename = "../src/processing-base/ProcessingCommandsHandler.h"
			templateInfo = [{ "filename":"ProcessingCommandsHandlerTemplate.h", "marker":"#undef PYTHON_MAGIC" }]
		else:
			filename = "../src/io-base/IOCommandsHandler.h"
			templateInfo = [{ "filename":"IOCommandsHandlerTemplate.h", "marker":"#undef PYTHON_MAGIC" }]
		super(CommandFactoryH, self).__init__(filename, templateInfo )
		self.input = input

	def processTemplate(self, filename, template):
		return template.replace("{CommandName}", self.input.commandName)


def main():
	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--hasReply', help="shows if the command has a reply ('true' or 'false'")
	args = parser.parse_args()

	input = Input(args.name, args.type, args.hasReply)
	
	objs = [ VclH(input), CommandFactoryH(input) ] 
	for obj in objs:
		obj.process()
	
main()
