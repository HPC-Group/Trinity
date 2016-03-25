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
	},
	{
		"source":"../src/commands/ProcessingCommands.h",
		"template":"ProcessingCommandsTemplate.h",
		"marker":"#undef PYTHON_MAGIC"
	},
	{
		"source":"../src/commands/ProcessingCommands.cpp",
		"template":"ProcessingCommandsTemplate.cpp",
		"marker":"#undef PYTHON_MAGIC"
	},
	{
		"source":"../src/commands/ProcessingCommands.cpp",
		"template":"ProcessingCommandsTemplate2.cpp",
		"marker":"#undef PYTHON_MAGIC_DEFINITION"
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
	},
	{
		"source":"../src/commands/IOCommands.h",
		"template":"IOCommandsTemplate.h",
		"marker":"#undef PYTHON_MAGIC"
	},
	{
		"source":"../src/commands/IOCommands.cpp",
		"template":"IOCommandsTemplate.cpp",
		"marker":"#undef PYTHON_MAGIC"
	},
	{
		"source":"../src/commands/IOCommands.cpp",
		"template":"IOCommandsTemplate2.cpp",
		"marker":"#undef PYTHON_MAGIC_DEFINITION"
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
		if line.strip() == marker:
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

def crQualify(type, name):
	if type in ["int", "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t",
				"uint32_t", "int64_t", "uint64_t", "bool", "float", "double"]:
		return type + " " + name
	else:
		return "const " + type + "& " + name
	
def makeArgumentList(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(crQualify(type, name))
	return ", ".join(items)

def makeMemberList(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append("\t\t" + type + " m_" + name + ";")
	return "\tprivate:\n" + "\n".join(items)

def makeGetterDeclarations(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append("\t\t" + type + " get" + name.title() + "() const;")
	return "\n".join(items)
	
def makeGetterDefinitions(commandName, params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(type + " " + commandName + "Cmd" + "::get" + name.title() + "() const {\n\t return " + "m_" + name + ";\n}")
	return "\n\n".join(items)
	
def makeInitializerList(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append("m_" + name + "(" + name + ")")
	return ", ".join(items)
	
def expandVariable(variable, input):
	if variable == "VclType":
		return input.commandName
	elif variable == "CommandNameCmd":
		return input.commandName + "Cmd"
	elif variable == "CommandNameHdl":
		return input.commandName + "Hdl"
	elif variable == "CommandNameRequest":
		return input.commandName + "Request"	
	elif variable == "CommandNameReply":
		return input.commandName + "Reply"
	elif variable == "RequestCtorDeclaration":
		return "Request(" + makeArgumentList(input.params) + ");"
	elif variable == "ReplyCtorDeclaration":
		return "Request(" + makeArgumentList(input.ret) + ");"
	elif variable == "RequestMembers":
		return makeMemberList(input.params)
	elif variable == "ReplyMembers":
		return makeMemberList(input.ret)
	elif variable == "RequestGetterDeclarations":
		return makeGetterDeclarations(input.params)
	elif variable == "ReplyGetterDeclarations":
		return makeGetterDeclarations(input.ret)
	elif variable == "RequestInitializerList":
		return makeInitializerList(input.params)
	elif variable == "RequestGetterDefinitions":
		return makeGetterDefinitions(input.commandName, input.params)
	elif variable == "ReplyGetterDefinitions":
		return makeGetterDefinitions(input.commandName, input.ret)
	else:
		raise Exception("Unknown variable " + variable)
		
		
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
	def __init__(self, commandName, hasReply, params, ret):
		self.commandName = commandName
		self.hasReply = hasReply
		self.params = params
		self.ret = ret
		
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

	input = Input(args.name, args.hasReply, args.parameters, [args.returnType, "result"])
	if args.type == "proc":
		templateInfos = templateInfosAll + templateInfosProc
	elif args.type == "io":
		templateInfos = templateInfosAll + templateInfosIO
	else:
		raise Exception("There's something wrong with the specified parameters!")
	process(templateInfos, input)
	
main()
