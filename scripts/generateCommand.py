import os
import argparse
import re
import subprocess
from inspect import isfunction
from shutil import copyfile

commonFiles = [
	"../src/commands/Vcl.h",
	"../src/commands/Request.cpp",
	"../src/commands/Reply.cpp"
]

ioFiles = [
	"../src/commands/IOCommands.h",
	"../src/commands/IOCommands.cpp",
	"../src/io-base/IOCommandsHandler.h",
	"../src/io-base/IOCommandsHandler.cpp",
	"../src/io-base/IOCommandFactory.cpp",
	"../src/common/IOSessionProxy.h",
	"../src/common/IOSessionProxy.cpp"
]

procFiles = [
	"../src/commands/ProcessingCommands.h",
	"../src/commands/ProcessingCommands.cpp",
	"../src/processing-base/ProcessingCommandsHandler.h",
	"../src/processing-base/ProcessingCommandsHandler.cpp",
	"../src/processing-base/ProcessingCommandFactory.cpp",
	"../src/frontend-base/RendererProxy.h",
	"../src/frontend-base/RendererProxy.cpp"
]

templates = {
## TOPLEVEL VARIABLES
"CommandHeader": 
r'''struct {{CommandNameCmd}} {
    static VclType Type;

	{{CommandRequestParams}}
	
	{{CommandReplyParamsIfNotVoid}}
};

{{CommandRequestOperators}}

{{CommandReplyOperatorsIfNotVoid}}

/* AUTOGEN CommandHeader */''',

"CommandImpl":
r'''////////////// {{CommandNameCmd}} //////////////

VclType {{CommandNameCmd}}::Type = VclType::{{VclType}};

{{CommandRequestImpl}}

{{CommandReplyImplIfNotVoid}}

/* AUTOGEN CommandImpl */''',

"CommandImplOperators":
r'''{{CommandImplRequestOperators}}
{{CommandImplReplyOperatorsIfNotVoid}}

/* AUTOGEN CommandImplOperators */''',

"ProcCommandHandlerHeader": 
r'''class {{CommandNameHdl}} : public ICommandHandler {
public:
    {{CommandNameHdl}}(const {{CommandNameRequest}}& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {{CommandNameRequest}} m_request;
	RenderSession* m_session;
};

/* AUTOGEN ProcCommandHandlerHeader */''',

"IOCommandHandlerHeader": 
r'''class {{CommandNameHdl}} : public ICommandHandler {
public:
    {{CommandNameHdl}}(const {{CommandNameRequest}}& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {{CommandNameRequest}} m_request;
	IOSession* m_session;
};

/* AUTOGEN IOCommandHandlerHeader */''',

"ProcCommandHandlerImpl":  
r'''{{CommandNameHdl}}::{{CommandNameHdl}}(const {{CommandNameRequest}}& request, RenderSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> {{CommandNameHdl}}::execute() {
    {{CommandNameCmd}}::ReplyParams params(m_session->getRenderer().{{InterfaceMethod}}({{ParamsFromRequest}}));
    return mocca::make_unique<{{CommandNameReply}}>(params, m_request.getRid(), m_session->getSid());
}

/* AUTOGEN ProcCommandHandlerImpl */''',

"IOCommandHandlerImpl": 
r'''{{CommandNameHdl}}::{{CommandNameHdl}}(const {{CommandNameRequest}}& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> {{CommandNameHdl}}::execute() {
    {{CommandNameCmd}}::ReplyParams params(m_session->getIO().{{InterfaceMethod}}({{ParamsFromRequest}}));
    return mocca::make_unique<{{CommandNameReply}}>(params, m_request.getRid(), m_session->getSid());
}

/* AUTOGEN IOCommandHandlerImpl */''',

"IOCommandFactoryEntry":  lambda input: "/* AUTOGEN IOCommandFactoryEntry */" if input.type == "proc" else "{{CommandFactoryEntry}}\n/* AUTOGEN IOCommandFactoryEntry */",

"ProcCommandFactoryEntry":  lambda input: "/* AUTOGEN ProcCommandFactoryEntry */" if input.type == "io" else "{{CommandFactoryEntry}}\n/* AUTOGEN ProcCommandFactoryEntry */",

"ProcRequestFactoryEntry": lambda input: "/* AUTOGEN ProcRequestFactoryEntry */" if input.type == "io" else "{{RequestFactoryEntry}}\n/* AUTOGEN ProcRequestFactoryEntry */",

"IORequestFactoryEntry": lambda input: "/* AUTOGEN IORequestFactoryEntry */" if input.type == "proc" else "{{RequestFactoryEntry}}\n/* AUTOGEN IORequestFactoryEntry */",

"ProcReplyFactoryEntry": lambda input: "/* AUTOGEN ProcReplyFactoryEntry */" if input.type == "io" or "void" in input.ret else "{{ReplyFactoryEntry}}\n/* AUTOGEN ProcReplyFactoryEntry */",

"IOReplyFactoryEntry": lambda input: "/* AUTOGEN IOReplyFactoryEntry */" if input.type == "proc" or "void" in input.ret else "{{ReplyFactoryEntry}}\n/* AUTOGEN IOReplyFactoryEntry */",

"IOInterfaceOverride": lambda input: "/* AUTOGEN IOInterfaceOverride */" if input.type == "proc" else "{{InterfaceOverride}}\n/* AUTOGEN IOInterfaceOverride */",

"RendererInterfaceOverride": lambda input: "/* AUTOGEN RendererInterfaceOverride */" if input.type == "io" else "{{InterfaceOverride}}\n/* AUTOGEN RendererInterfaceOverride */",

"IOSessionProxyImpl":
r'''{{ReturnType}} IOSessionProxy::{{InterfaceMethod}}({{RequestArgumentList}}) const {
{{CommandNameCmd}}::RequestParams params({{RequestArguments}});
{{CommandNameRequest}} request(params, IDGenerator::nextID(), m_remoteSid);
auto reply = sendRequestChecked(m_inputChannel, request);
return reply->getParams().getResult();
}

/* AUTOGEN IOSessionProxyImpl */''',

"RendererProxyImpl":
r'''{{ReturnType}} RendererProxy::{{InterfaceMethod}}({{RequestArgumentList}}) const {
{{CommandNameCmd}}::RequestParams params({{RequestArguments}});
{{CommandNameRequest}} request(params, IDGenerator::nextID(), m_remoteSid);
auto reply = sendRequestChecked(m_inputChannel, request);
return reply->getParams().getResult();
}

/* AUTOGEN RendererProxyImpl */''',

"VclEnumEntry" : "{{VclType}},\n/* AUTOGEN VclEnumEntry */",

"VclMapEntry" : 'm_cmdMap.insert("{{VclType}}", VclType::{{VclType}});\n/* AUTOGEN VclMapEntry */',


## LOWER LEVEL COMMON VARIABLES

"CommandName" : lambda input : input.commandName,

"CommandNameCmd": r'{{CommandName}}Cmd',

"CommandNameHdl": r'{{CommandName}}Hdl',

"CommandNameRequest": r'{{CommandName}}Request',

"CommandNameReply": r'{{CommandName}}Reply',

"InterfaceMethod" : lambda input: untitle(input.methodname),

"ReturnType": lambda input: input.ret[0],

"RequestArgumentList": lambda input: makeArgumentList(input.params),

"ReplyArgumentList": lambda input: makeArgumentList(input.ret),

"RequestArguments": lambda input: makeArguments(input.params),

"ReplyArguments": lambda input: makeArguments(input.ret),


## LOWER LEVEL COMMAND HEADER VARIABLES

"CommandRequestParams":
r'''class RequestParams : public SerializableTemplate<RequestParams> {
public:
	RequestParams() = default;
	{{RequestCtorDeclarationSemicolon}}
	
	void serialize(ISerialWriter& writer) const override;
	void deserialize(const ISerialReader& reader) override;

	std::string toString() const;
	bool equals(const RequestParams& other) const;
	
	{{RequestGetterDeclarations}}
	{{RequestMemberList}}
};''',

"RequestCtorDeclaration": lambda input : makeCtorDeclaration(input.params, "RequestParams"),

"RequestCtorDeclarationSemicolon": lambda input : "" if not input.params else makeCtorDeclaration(input.params, "RequestParams") + ";",

"RequestGetterDeclarations": lambda input : makeGetterDeclarations(input.params),

"RequestMemberList": lambda input : makeMemberList(input.params),

"CommandRequestOperators" :
r'''bool operator==(const {{CommandNameCmd}}::RequestParams& lhs, const {{CommandNameCmd}}::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::RequestParams& obj);
using {{CommandNameRequest}} = RequestTemplate<{{CommandNameCmd}}>;''',

"CommandReplyParamsIfNotVoid": lambda input : "" if "void" in input.ret else "{{CommandReplyParams}}",

"CommandReplyParams": 
r'''class ReplyParams : public SerializableTemplate<ReplyParams> {
public:
	ReplyParams() = default;
	{{ReplyCtorDeclaration}};
	
	void serialize(ISerialWriter& writer) const override;
	void deserialize(const ISerialReader& reader) override;

	std::string toString() const;
	bool equals(const ReplyParams& other) const;

	{{ReplyGetterDeclarations}}
	{{ReplyMemberList}}
};''',

"ReplyCtorDeclaration": lambda input : makeCtorDeclaration(input.ret, "ReplyParams"),

"ReplyCtorDeclarationSemicolon": lambda input : "" if not inpur.ret else makeCtorDeclaration(input.ret, "Reply") + ";",

"ReplyGetterDeclarations": lambda input : makeGetterDeclarations(input.ret),

"ReplyMemberList": lambda input : makeMemberList(input.ret),

"CommandReplyOperatorsIfNotVoid": lambda input : "" if "void" in input.ret else "{{CommandReplyOperators}}",

"CommandReplyOperators" :
r'''bool operator==(const {{CommandNameCmd}}::ReplyParams& lhs, const {{CommandNameCmd}}::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::ReplyParams& obj);
using {{CommandNameReply}} = ReplyTemplate<{{CommandNameCmd}}>;''',

## LOWER LEVEL COMMAND IMPL VARIABLES

"CommandRequestImpl":
r'''
{{CommandRequestCtorImplIfNotEmpty}}

void {{CommandNameCmd}}::RequestParams::serialize(ISerialWriter& writer) const {
{{RequestParamSerialization}}
}

void {{CommandNameCmd}}::RequestParams::deserialize(const ISerialReader& reader) {
{{RequestParamDeserialization}}
}

bool {{CommandNameCmd}}::RequestParams::equals(const {{CommandNameCmd}}::RequestParams& other) const {
{{RequestParamEquals}}
}

{{RequestGetterDefinitions}}

std::string {{CommandNameCmd}}::RequestParams::toString() const {
    std::stringstream stream;
{{RequestParamStreaming}}
    return stream.str();
}''',

"CommandRequestCtorImplIfNotEmpty": lambda input : "" if not input.params else "{{CommandRequestCtorImpl}}",

"CommandRequestCtorImpl":
r'''{{CommandNameCmd}}::RequestParams::{{RequestCtorDeclaration}}
{{RequestInitializerList}} {}''',

"RequestInitializerList": lambda input : makeInitializerList(input.params),

"RequestParamSerialization": lambda input : makeSerialization(input.params),

"RequestParamDeserialization": lambda input : makeDeserialization(input.params),

"RequestParamEquals": lambda input : makeEquals(input.params),

"RequestGetterDefinitions": lambda input : makeGetterDefinitions(input.commandName, input.params, "RequestParams"),

"RequestParamStreaming": lambda input : makeStreaming(input.params),

"CommandReplyImplIfNotVoid": lambda input : "" if "void" in input.ret else "{{CommandReplyImpl}}",

"CommandImplRequestOperators":
r'''bool operator==(const {{CommandNameCmd}}::RequestParams& lhs, const {{CommandNameCmd}}::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::RequestParams& obj) {
    return os << obj.toString();
}''',

"CommandReplyImpl":
r'''
{{CommandReplyCtorImpl}}

void {{CommandNameCmd}}::ReplyParams::serialize(ISerialWriter& writer) const {
{{ReplyParamSerialization}}
}

void {{CommandNameCmd}}::ReplyParams::deserialize(const ISerialReader& reader) {
{{ReplyParamDeserialization}}
}

bool {{CommandNameCmd}}::ReplyParams::equals(const {{CommandNameCmd}}::ReplyParams& other) const {
{{ReplyParamEquals}}
}

{{ReplyGetterDefinitions}}

std::string {{CommandNameCmd}}::ReplyParams::toString() const {
    std::stringstream stream;
{{ReplyParamStreaming}}
    return stream.str();
}''',

"CommandReplyCtorImpl":
r'''{{CommandNameCmd}}::ReplyParams::{{ReplyCtorDeclaration}}
{{ReplyInitializerList}} {}''',

"ReplyInitializerList": lambda input : makeInitializerList(input.ret),

"ReplyParamSerialization": lambda input : makeSerialization(input.ret),

"ReplyParamDeserialization": lambda input : makeDeserialization(input.ret),

"ReplyParamEquals": lambda input : makeEquals(input.ret),

"ReplyGetterDefinitions": lambda input : makeGetterDefinitions(input.commandName, input.ret, "ReplyParams"),

"ReplyParamStreaming": lambda input : makeStreaming(input.ret),

"CommandImplReplyOperatorsIfNotVoid": lambda input : "" if "void" in input.ret else "{{CommandImplReplyOperators}}",

"CommandImplReplyOperators":
r'''bool operator==(const {{CommandNameCmd}}::ReplyParams& lhs, const {{CommandNameCmd}}::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::ReplyParams& obj) {
    return os << obj.toString();
}''',

## LOWER LEVEL COMMAND FACTORY VARIABLES

"CommandFactoryEntry":
r'''case VclType::{{VclType}}:
return mocca::make_unique<{{CommandNameHdl}}>(static_cast<const {{CommandNameRequest}}&>(request), session);
break;''',

## LOWER LEVEL REQUEST IMPL VARIABLES

"RequestFactoryEntry":
r''' else if (type == {{CommandNameRequest}}::Ifc::Type) {
	return reader->getSerializablePtr<{{CommandNameRequest}}>("req");
}''',

## LOWER LEVEL REPLY IMPL VARIABLES

"ReplyFactoryEntry":
r''' else if (type == {{CommandNameReply}}::Ifc::Type) {
	return reader->getSerializablePtr<{{CommandNameReply}}>("rep");
}''',

## LOWER LEVEL COMMAND HANDLER IMPL VARIABLES

"ParamsFromRequest": lambda input: makeParamsFromRequest(input.params),

## LOWER LEVEL PROXY HEADER VARIABLES

"InterfaceOverride": lambda input : makeInterfaceOverride(input),

## LOWER LEVEL VCL HEADER VARIABLES

"VclType" : r'{{CommandName}}'
}
			
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

def title(s):
	return s[0].upper() + s[1:]
	
def untitle(s):
	return s[0].lower() + s[1:]
	
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

def makeArguments(params):
	items = []
	for i in xrange(0, len(params), 2):
		items.append(params[i + 1])
	return ", ".join(items)	

def makeParamsFromRequest(params):
	items = []
	for i in xrange(0, len(params), 2):
		name = params[i + 1]
		items.append("m_request.getParams().get" + title(name) + "()")
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
		items.append(type + " get" + title(name) + "() const;")
	return "\n".join(items)
	
def makeGetterDefinitions(commandName, params, className):
	items = []
	for i in xrange(0, len(params), 2):
		type = params[i]
		name = params[i + 1]
		items.append(type + "  {{CommandNameCmd}}::" + className + "::get" + title(name) + "() const {\n return " + member(name) + ";\n}")
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
		return ctorName + "(" + makeArgumentList(params) + ")"	

def makeInterfaceOverride(input):
	return "{{ReturnType}} {{InterfaceMethod}} ({{RequestArgumentList}}) const override;"
		
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

def replaceFile(filename, source):
	with open(filename, "w") as destfile:
		destfile.write(source)
		
def process(files, input):
	print files
	for file in files:
		source = readSourceFile(file)
		source = replaceAutogen(source)
		tokens = replaceRecursive(source, input)
		result = ""
		for token in tokens:
			result = result + token.value
		formatted = clangFormat(result)
		replaceFile(file, formatted)
		
class Input:
	def __init__(self, commandName, type, methodname, params, ret):
		self.commandName = commandName
		self.type = type
		self.methodname = methodname
		self.params = params
		self.ret = ret
		
def main():
	global commonFiles
	global ioFiles
	global procFiles

	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--methodname', help="name of the corresponding interface method")
	parser.add_argument('--returntype', help="return value of the command")
	parser.add_argument('--parameters', nargs='*', help="all parameters of the command ('please specify all parameters successive, e.g., int x float y double z')")
	args = parser.parse_args()

	input = Input(args.name, args.type, args.methodname, args.parameters, [args.returntype, "result"])
	files = []
	if args.type == "proc":
			files = commonFiles + procFiles
	elif args.type == "io":
		files = commonFiles + ioFiles
	else:
		raise Exception("There's something wrong with the specified parameters!")
	process(files, input)
	
main()
