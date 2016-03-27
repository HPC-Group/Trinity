import os
import argparse
import re
from inspect import isfunction
from shutil import copyfile

commonFiles = [
	"../src/commands/Vcl.h"
]

ioFiles = [
	"../src/io-base/IOCommandsHandler.h"
]

procFiles = [
	"../src/processing-base/ProcessingCommandsHandler.h"
]

templates = {
"CommandName" : lambda input : input.commandName,

"CommandNameCmd": r'{{CommandName}}Cmd',

"CommandNameHdl": r'{{CommandName}}Hdl',

"CommandNameRequest": r'{{CommandName}}Request',

"CommandNameReply": r'{{CommandName}}Reply',

"CommandHandlerHeader": 
r'''class {{CommandNameHdl}} : public ICommandHandler {
public:
    {{CommandNameHdl}}(const {{CommandNameRequest}}& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {{CommandNameRequest}} m_request;
	IOSession* m_session;
};''',

"VclType" : r'{{CommandName}}',

"VclEnumEntry" : r'{{VclType}},' ,

"VclMapEntry" : r'm_cmdMap.insert("{{VclType}}", VclType::{{VclType}});' 





}

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
			
def readSourceFile(filename):
	with open(filename, "r+") as sourceFile:
		source = sourceFile.read()
		return source

class TokenType:
	text = 1
	variable = 2
		
class Token:
	def __init__(self, type, value):
		self.type = type
		self.value = value

def replaceAutogen(source):
	return re.sub(r"/\* AUTOGEN (\w+) \*/", r"{{\1}}", source)
		
def tokenizeSource(source):
	regex = "({{\w+}})"
	tokens = re.split(regex, source)
	result = []
	for token in tokens:
		if token[0:2] == "{{":
			result.append(Token(TokenType.variable, token[2:-2]))
		else:
			result.append(Token(TokenType.text, token))
	return result

def member(name):
	return "m_" + name
	
def isIntType(type):
	return type in ["int", "unsigned int", "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t"]
	
def crQualify(type, name):
	if isIntType(type) or type in ["bool", "float", "double"]:
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
	if not params:
		return ""
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append("\t\t" + type + " " + member(name) + ";")
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
		items.append(type + " " + commandName + "Cmd" + "::get" + name.title() + "() const {\n\t return " + member(name) + ";\n}")
	return "\n\n".join(items)
	
def makeInitializerList(params):
	if not params:
		return ""
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(member(name) + "(" + name + ")")
	return "\t:"+ ", ".join(items)
	
def makeSerialization(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		if isIntType(type):
			items.append('\twriter.appendInt("' + name + '", ' + member(name) + ');')
		elif type == "float":
			items.append('\twriter.appendFloat("' + name + '", ' + member(name) + ');')
		elif type == "double":
			items.append('\twriter.appendDouble("' + name + '", ' + member(name) + ');')
		elif type == "bool":
			items.append('\twriter.appendBool("' + name + '", ' + member(name) + ');')
		elif type == "string":
			items.append('\twriter.appendString("' + name + '", ' + member(name) + ');')
		elif "vector<float>" in type:
			items.append('\twriter.appendFloatVec("' + name + '", ' + member(name) + ');')
		elif "vector<bool>" in type:
			items.append('\twriter.appendBoolVec("' + name + '", ' + member(name) + ');')
		elif "vector<std::string>" in type:
			items.append('\twriter.appendStringVec("' + name + '", ' + member(name) + ');')
		elif "vector<int" in type or "vector<uint" in type or "vector<unsigned int" in type:
			items.append('\twriter.appendIntVec("' + name + '", ' + member(name) + ');')
		elif "vector<" in type:
			items.append("\t// TODO: this is getting to complicated, you need to do this yourself... (" + name + ")")
		else:
			items.append('\twriter.appendObject("' + name + '", ' + member(name) + ');')
	return "\n".join(items)
	
def makeDeserialization(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		if type == "uint8_t":
			items.append('\t' + member(name) + ' = reader.getUInt8("' + name + '");')
		elif type == "int32_t":
			items.append('\t' + member(name) + ' = reader.getInt32("' + name + '");')
		elif type == "uint32_t":
			items.append('\t' + member(name) + ' = reader.getUInt32("' + name + '");')
		elif type == "int64_t":
			items.append('\t' + member(name) + ' = reader.getInt64("' + name + '");')
		elif type == "uint64_t":
			items.append('\t' + member(name) + ' = reader.getUInt64("' + name + '");')
		elif type == "float":
			items.append('\t' + member(name) + ' = reader.getFloat("' + name + '");')
		elif type == "double":
			items.append('\t' + member(name) + ' = reader.getDouble("' + name + '");')
		elif type == "bool":
			items.append('\t' + member(name) + ' = reader.getBool("' + name + '");')
		elif "vector<float>" in type:
			items.append('\t' + member(name) + ' = reader.getFloatVec("' + name + '");')
		elif "vector<uint8_t>" in type:
			items.append('\t' + member(name) + ' = reader.getUInt8Vec("' + name + '");')
		elif "vector<int32_t>" in type:
			items.append('\t' + member(name) + ' = reader.getInt32Vec("' + name + '");')
		elif "vector<uint64_t>" in type:
			items.append('\t' + member(name) + ' = reader.getUInt64Vec("' + name + '");')
		elif "vector<bool>" in type:
			items.append('\t' + member(name) + ' = reader.getBoolVec("' + name + '");')
		elif "vector<std::string>" in type:
			items.append('\t' + member(name) + ' = reader.getStringVec("' + name + '");')
		elif "vector<" in type:
			items.append("\t// TODO: this is getting to complicated, you need to do this yourself... (" + name + ")")
		else:
			items.append('\t' + member(name) + ' = reader.getSerializable<' + type + '>("' + name + '");')
	return "\n".join(items)
	
def makeEquals(params):
	if not params:
		return "\treturn true;"
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(member(name) + " == other." + member(name))
	return "\treturn " + " && ".join(items) + ";"
	
def makeStreaming(params):
	if not params:
		return ""
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		if i == 0:
			items.append('"' + name + ': " << ' + member(name))
		else:
			items.append('"; ' + name + ': " << ' + member(name))
	return "\tstream << " + " << ".join(items) + ";"
	
def makeCtorDeclaration(params, ctorType):
	if not params:
		return ""
	else:
		return ctorType + "(" + makeArgumentList(params) + ");"		

def expandVariable(variable, input):
	global templates
	t = templates[variable]
	if isfunction(t):
		return t(input)
	else:
		return t
	

	if variable == "RequestCtorDeclaration":
		return makeCtorDeclaration(input.params, "RequestParams")
	elif variable == "ReplyCtorDeclaration":
		return makeCtorDeclaration(input.ret, "ReplyParams")
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
	elif variable == "RequestParamSerialization":
		return makeSerialization(input.params)
	elif variable == "ReplyParamSerialization":
		return makeDeserialization(input.ret)
	elif variable == "RequestParamDeserialization":
		return makeDeserialization(input.params)
	elif variable == "ReplyParamDeserialization":
		return makeSerialization(input.ret)
	elif variable == "RequestParamEquals":
		return makeEquals(input.params)
	elif variable == "ReplyParamEquals":
		return makeEquals(input.ret)
	elif variable == "RequestParamStreaming":
		return makeStreaming(input.params)
	elif variable == "ReplyParamStreaming":
		return makeStreaming(input.ret)
	else:
		raise Exception("Unknown variable " + variable)
		
def replaceRecursive(source, input):
	tokens = tokenizeSource(source)
	result = []
	for idx, token in enumerate(tokens):
		if token.type == TokenType.variable:
			expanded = expandVariable(token.value, input)
			result = result + replaceRecursive(expanded, input)
		else:
			result.append(token)
	return result
	
def process(files, input):
	print files
	for file in files:
		source = readSourceFile(file)
		source = replaceAutogen(source)
		tokens = replaceRecursive(source, input)
		result = ""
		for token in tokens:
			result = result + token.value
		print result

class Input:
	def __init__(self, commandName, hasReply, params, ret):
		self.commandName = commandName
		self.hasReply = hasReply
		self.params = params
		self.ret = ret
		
def main():
	global commonFiles
	global ioFiles

	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--hasReply', help="shows if the command has a reply ('true' or 'false'")
	parser.add_argument('--returnType', help="return value of the command")
	parser.add_argument('--parameters', nargs='*', help="all parameters of the command ('please specify all parameters successive, e.g., int x float y double z')")
	args = parser.parse_args()

	input = Input(args.name, args.hasReply, args.parameters, [args.returnType, "result"])
	files = []
	if args.type == "proc":
			files = commonFiles + procFiles
	elif args.type == "io":
		files = commonFiles + ioFiles
	else:
		raise Exception("There's something wrong with the specified parameters!")
	process(files, input)
	
main()
