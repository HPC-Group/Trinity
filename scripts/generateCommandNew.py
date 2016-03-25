import os
import argparse
import re
from shutil import copyfile

templateInfosAll = [
	{
		"source":"../src/commands/Vcl.h",
		"template":"VclTemplate.h",
		"marker":"#undef PYTHON_ENUM"
	},
	{
		"source":"../src/commands/Vcl.h",
		"template":"VclTemplate2.h",
		"marker":"#undef PYTHON_MAGIC_STRING"
	}
]

templateInfosProc = [
	{
		"source":"../src/processing-base/ProcessingCommandsHandler.h",
		"template":"ProcessingCommandsHandlerTemplate.h",
		"marker":"#undef PYTHON_MAGIC"
	},
	{
		"source":"../src/processing-base/ProcessingCommandsHandler.cpp",
		"template":"ProcessingCommandsHandlerTemplate.cpp",
		"marker":"#undef PYTHON_MAGIC"
	}
]

templateInfosIO = [
	{
		"source":"../src/io-base/IOCommandsHandler.h",
		"template":"IOCommandsHandlerTemplate.h",
		"marker":"#undef PYTHON_MAGIC"
	},
	{
		"source":"../src/io-base/IOCommandsHandler.cpp",
		"template":"IOCommandsHandlerTemplate.cpp",
		"marker":"#undef PYTHON_MAGIC"
	}
]

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
		
def process(templateInfos, input):
	for templateInfo in templateInfos:
		template = readTemplateFile(templateInfo["template"])
		marker = templateInfo["marker"]
		tokens = tokenizeTemplate(template)
		code = ""
		for token in tokens:
			if token.type == TokenType.text:
				code = code + token.value
			else:
				code = code + expandVariable(token.value, input)
		insertCodeInSource(templateInfo["source"], code, marker)

class Input:
	def __init__(self, commandName, hasReply):
		self.commandName = commandName
		self.hasReply = hasReply
		
def main():
	global templateInfosAll
	global templateInfosProc
	global templateInfosIO

	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--hasReply', help="shows if the command has a reply ('true' or 'false'")
	parser.add_argument('--returnType', help="return value of the command")
	parser.add_argument('--parameters', nargs='*', help="all parameters of the command ('please specify all parameters successive, e.g., int x float y double z')")
	args = parser.parse_args()

	input = Input(args.name, args.hasReply)
	if args.type == "proc":
		templateInfos = templateInfosAll + templateInfosProc
	elif args.type == "io":
		templateInfos = templateInfosAll + templateInfosIO
	else:
		raise Exception("There's something wrong with the specified parameters!")
	process(templateInfos, input)
	
main()
