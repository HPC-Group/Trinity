import os
import argparse
import re
import subprocess
from inspect import isfunction
from shutil import copyfile

commonFiles = [
	#"../src/commands/Vcl.h"
]

ioFiles = [
	"../src/commands/IOCommands.h"
	#"../src/io-base/IOCommandsHandler.h",
	#"../src/io-base/IOCommandsHandler.cpp"
]

procFiles = [
	#"../src/commands/ProcessingCommands.h",
	#"../src/processing-base/ProcessingCommandsHandler.h",
	#"../src/processing-base/ProcessingCommandsHandler.cpp"
]

templates = {
## TOPLEVEL VARIABLES
"CommandHeader": 
r'''struct {{CommandNameCmd}} {
    static VclType Type;

	{{CommandRequestParams}}
	
	{{CommandReplyParamsIfNotVoid}}
};''',

"CommandHandlerHeader": 
r'''class {{CommandNameHdl}} : public ICommandHandler {
public:
    {{CommandNameHdl}}(const {{CommandNameRequest}}& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {{CommandNameRequest}} m_request;
	IOSession* m_session;
};''',

"CommandHandlerImpl":
r'''{{CommandNameHdl}}::{{CommandNameHdl}}(const {{CommandNameRequest}}& request, RenderSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> {{CommandNameHdl}}::execute() {
	// TODO
}''',

"VclEnumEntry" : r'{{VclType}},' ,

"VclMapEntry" : r'm_cmdMap.insert("{{VclType}}", VclType::{{VclType}});',


## LOWER LEVEL COMMON VARIABLES

"CommandName" : lambda input : input.commandName,

"CommandNameCmd": r'{{CommandName}}Cmd',

"CommandNameHdl": r'{{CommandName}}Hdl',

"CommandNameRequest": r'{{CommandName}}Request',

"CommandNameReply": r'{{CommandName}}Reply',


## LOWER LEVEL COMMAND HEADER VARIABLES

"CommandRequestParams":
r'''class RequestParams : public SerializableTemplate<RequestParams> {
public:
	RequestParams() = default;
	{{RequestCtorDeclaration}}
	
	void serialize(ISerialWriter& writer) const override;
	void deserialize(const ISerialReader& reader) override;

	std::string toString() const;
	bool equals(const RequestParams& other) const;
	
	{{RequestGetterDeclarations}}
	{{RequestMemberList}}
};''',

"RequestCtorDeclaration": lambda input : makeCtorDeclaration(input.params, "RequestParams"),

"RequestGetterDeclarations": lambda input : makeGetterDeclarations(input.params),

"RequestMemberList": lambda input : makeMemberList(input.params),

"CommandReplyParamsIfNotVoid": lambda input : "" if "void" in input.ret else "{{CommandReplyParams}}",

"CommandReplyParams": 
r'''class ReplyParams : public SerializableTemplate<ReplyParams> {
public:
	ReplyParams() = default;
	{{ReplyCtorDeclaration}}
	
	void serialize(ISerialWriter& writer) const override;
	void deserialize(const ISerialReader& reader) override;

	std::string toString() const;
	bool equals(const ReplyParams& other) const;

	{{RequestGetterDeclarations}}
	{{RequestMemberList}}
};''',

"ReplyCtorDeclaration": lambda input : makeCtorDeclaration(input.ret, "ReplyParams"),

"ReplyGetterDeclarations": lambda input : makeGetterDeclarations(input.ret),

"ReplyMemberList": lambda input : makeMemberList(input.ret),

## LOWER LEVEL VCL HEADER VARIABLES

"VclType" : r'{{CommandName}}'
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
		items.append(type + " " + member(name) + ";")
	return "private:\n" + "\n".join(items)

def makeGetterDeclarations(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(type + " get" + name.title() + "() const;")
	return "\n".join(items)
	
def makeGetterDefinitions(commandName, params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(type + " " + commandName + "Cmd" + "::get" + name.title() + "() const {\n return " + member(name) + ";\n}")
	return "\n\n".join(items)
	
def makeInitializerList(params):
	if not params:
		return ""
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(member(name) + "(" + name + ")")
	return ":"+ ", ".join(items)
	
def makeSerialization(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		if isIntType(type):
			items.append('writer.appendInt("' + name + '", ' + member(name) + ');')
		elif type == "float":
			items.append('writer.appendFloat("' + name + '", ' + member(name) + ');')
		elif type == "double":
			items.append('writer.appendDouble("' + name + '", ' + member(name) + ');')
		elif type == "bool":
			items.append('writer.appendBool("' + name + '", ' + member(name) + ');')
		elif type == "string":
			items.append('writer.appendString("' + name + '", ' + member(name) + ');')
		elif "vector<float>" in type:
			items.append('writer.appendFloatVec("' + name + '", ' + member(name) + ');')
		elif "vector<bool>" in type:
			items.append('writer.appendBoolVec("' + name + '", ' + member(name) + ');')
		elif "vector<std::string>" in type:
			items.append('writer.appendStringVec("' + name + '", ' + member(name) + ');')
		elif "vector<int" in type or "vector<uint" in type or "vector<unsigned int" in type:
			items.append('writer.appendIntVec("' + name + '", ' + member(name) + ');')
		elif "vector<" in type:
			items.append("// TODO: this is getting to complicated, you need to do this yourself... (" + name + ")")
		else:
			items.append('writer.appendObject("' + name + '", ' + member(name) + ');')
	return "\n".join(items)
	
def makeDeserialization(params):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		if type == "uint8_t":
			items.append(member(name) + ' = reader.getUInt8("' + name + '");')
		elif type == "int32_t":
			items.append(member(name) + ' = reader.getInt32("' + name + '");')
		elif type == "uint32_t":
			items.append(member(name) + ' = reader.getUInt32("' + name + '");')
		elif type == "int64_t":
			items.append(member(name) + ' = reader.getInt64("' + name + '");')
		elif type == "uint64_t":
			items.append(member(name) + ' = reader.getUInt64("' + name + '");')
		elif type == "float":
			items.append(member(name) + ' = reader.getFloat("' + name + '");')
		elif type == "double":
			items.append(member(name) + ' = reader.getDouble("' + name + '");')
		elif type == "bool":
			items.append(member(name) + ' = reader.getBool("' + name + '");')
		elif "vector<float>" in type:
			items.append(member(name) + ' = reader.getFloatVec("' + name + '");')
		elif "vector<uint8_t>" in type:
			items.append(member(name) + ' = reader.getUInt8Vec("' + name + '");')
		elif "vector<int32_t>" in type:
			items.append(member(name) + ' = reader.getInt32Vec("' + name + '");')
		elif "vector<uint64_t>" in type:
			items.append(member(name) + ' = reader.getUInt64Vec("' + name + '");')
		elif "vector<bool>" in type:
			items.append(member(name) + ' = reader.getBoolVec("' + name + '");')
		elif "vector<std::string>" in type:
			items.append(member(name) + ' = reader.getStringVec("' + name + '");')
		elif "vector<" in type:
			items.append("// TODO: this is getting to complicated, you need to do this yourself... (" + name + ")")
		else:
			items.append(member(name) + ' = reader.getSerializable<' + type + '>("' + name + '");')
	return "\n".join(items)
	
def makeEquals(params):
	if not params:
		return "return true;"
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(member(name) + " == other." + member(name))
	return "return " + " && ".join(items) + ";"
	
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
	return "stream << " + " << ".join(items) + ";"
	
def makeCtorDeclaration(params, ctorName):
	if not params:
		return ""
	else:
		return ctorName + "(" + makeArgumentList(params) + ");"		

def expandVariable(variable, input):
	global templates
	t = templates[variable]
	if isfunction(t):
		return t(input)
	else:
		return t
		
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

def clangFormat(source):
	proc = subprocess.Popen(
		'clang-format',stdout=subprocess.PIPE,
		stdin=subprocess.PIPE)
	proc.stdin.write(source)
	proc.stdin.close()
	result = proc.stdout.read()
	proc.wait()
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
		print clangFormat(result)
		
class Input:
	def __init__(self, commandName, params, ret):
		self.commandName = commandName
		self.params = params
		self.ret = ret
		
def main():
	global commonFiles
	global ioFiles
	global procFiles

	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--returnType', help="return value of the command")
	parser.add_argument('--parameters', nargs='*', help="all parameters of the command ('please specify all parameters successive, e.g., int x float y double z')")
	args = parser.parse_args()

	input = Input(args.name, args.parameters, [args.returnType, "result"])
	files = []
	if args.type == "proc":
			files = commonFiles + procFiles
	elif args.type == "io":
		files = commonFiles + ioFiles
	else:
		raise Exception("There's something wrong with the specified parameters!")
	process(files, input)
	
main()
