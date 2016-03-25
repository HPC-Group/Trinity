import os
import argparse
import re
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

class TokenType:
	text = 1
	variable = 2
		
class Token:
	def __init__(self, type, value):
		self.type = type
		self.value = value

def tokenizeTemplate(template):
	regex = "({{\w+}})"
	tokens = re.split(regex, template)
	result = []
	for token in tokens:
		if token[0:2] == "{{":
			result.append(Token(TokenType.variable, token[2:-2]))
		else:
			result.append(Token(TokenType.text, token))
	return result

def expandVariable(variable, input):
	if variable == "VclType":
		return input.commandName
	elif variable == "CommandNameHdl":
		return input.commandName + "Hdl"
	elif variable == "CommandNameRequest":
		return input.commandName + "Request"	
	
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
	def __init__(self, destination, templateInfos, input):
		self.destination = destination
		self.templateInfos = templateInfos
		self.input = input
	
	def process(self):
		for templateInfo in self.templateInfos:
			filename = templateInfo["filename"]
			marker = templateInfo["marker"]
			template = readTemplateFile(filename)
			tokens = tokenizeTemplate(template)
			code = ""
			for token in tokens:
				if token.type == TokenType.text:
					code = code + token.value
				else:
					code = code + expandVariable(token.value, self.input)
			insertCodeInSource(self.destination, code, marker)

class VclH(ProcessorBase):
	def __init__(self, input):
		filename = "../src/commands/Vcl.h"
		templateInfo =  [
			{ "filename":"VclTemplate.h", "marker":"#undef PYTHON_ENUM" },
			{ "filename":"VclTemplate2.h", "marker":"#undef PYTHON_MAGIC_STRING" }]
		super(VclH, self).__init__(filename, templateInfo, input)

class CommandHandlerH(ProcessorBase):
	def __init__(self, input):
		if input.type == Types.proc:
			filename = "../src/processing-base/ProcessingCommandsHandler.h"
			templateInfo = [{ "filename":"ProcessingCommandsHandlerTemplate.h", "marker":"#undef PYTHON_MAGIC" }]
		else:
			filename = "../src/io-base/IOCommandsHandler.h"
			templateInfo = [{ "filename":"IOCommandsHandlerTemplate.h", "marker":"#undef PYTHON_MAGIC" }]
		super(CommandHandlerH, self).__init__(filename, templateInfo, input )
		
class CommandHandlerCPP(ProcessorBase):
	def __init__(self, input):
		if input.type == Types.proc:
			filename = "../src/processing-base/ProcessingCommandsHandler.cpp"
			templateInfo = [{ "filename":"ProcessingCommandsHandlerTemplate.cpp", "marker":"#undef PYTHON_MAGIC" }]
		else:
			filename = "../src/io-base/IOCommandsHandler.cpp"
			templateInfo = [{ "filename":"IOCommandsHandlerTemplate.cpp", "marker":"#undef PYTHON_MAGIC" }]
		super(CommandHandlerCPP, self).__init__(filename, templateInfo, input )	
		
def main():
	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--hasReply', help="shows if the command has a reply ('true' or 'false'")
	parser.add_argument('--returnType', help="return value of the command")
	parser.add_argument('--parameters', nargs='*', help="all parameters of the command ('please specify all parameters successive, e.g., int x float y double z')")
	args = parser.parse_args()

	input = Input(args.name, args.type, args.hasReply)
	
	objs = [ VclH(input), CommandHandlerH(input), CommandHandlerCPP(input) ] 
	for obj in objs:
		obj.process()
	
def test():
	str = "This is a {{Var}} Test"
	result = tokenizeTemplate(str)
	for t in result:
		print t.type
		print t.value
	
#test()
main()
