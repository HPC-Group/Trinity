#

import os, sys
import argparse
from shutil import copyfile

from string import Template

class Types:
	proc = 1
	io = 2

srcPath = "../src"

templateDir = "./template/"

procDir = "./processingFiles/"
ioDir = "./ioFiles/"
commDir = "./commFiles/"
commonDir = "./commonFiles/"
frontendDir = "./frontendFiles/"

processingFiles = {0 : "ProcessingCommands.h",
		   1 : "ProcessingCommands.cpp",
		   2 : "",
		   3 : "ProcessingCommandsHandler.h",
		   4 : "ProcessingCommandsHandler.cpp",
		   5 : "ProcessingCommandFactory.cpp"}

ioFiles = {0 : "IOCommands.h",
	   1 : "IOCommands.cpp",
	   2 : "",
	   3 : "IOCommandsHandler.h",
	   4 : "IOCommandsHandler.cpp",
	   5 : "IOCommandFactory.cpp"}

commFiles = {0 : "Request.cpp",
	     1 : "Reply.cpp",
	     2 : "Vcl.h"}

commonFiles = {0 : "IOSessionProxy.h",
							 1 : "IOSessionProxy.cpp"}

frontendFiles = {0 : "RendererProxy.h",
								 1 : "RendererProxy.cpp"}

processingTemplates = {0 : "ProcessingCommandsTemplate.h",
		       1 : "ProcessingCommandsTemplate.cpp",
		       2 : "ProcessingCommandsTemplate2.cpp",
		       3 : "ProcessingCommandsHandlerTemplate.h",
		       4 : "ProcessingCommandsHandlerTemplate.cpp",
		       5 : "ProcessingCommandFactoryTemplate.cpp"}

ioTemplates = {0 : "IOCommandsTemplate.h",
	       1 : "IOCommandsTemplate.cpp",
	       2 : "IOCommandsTemplate2.cpp",
	       3 : "IOCommandsHandlerTemplate.h",
	       4 : "IOCommandsHandlerTemplate.cpp",
	       5 : "IOCommandFactoryTemplate.cpp"}

commonTemplates = {0 : "IOSessionProxyTemplate.h",
									 1 : "IOSessionProxyTemplate.cpp"}

frontendTemplates = {0 : "RendererProxyTemplate.h",
										 1 : "RendererProxyTemplate.cpp"}


def openFile(filename, templateName, lookup):
  with open(filename, "r+") as datafile:
    content = datafile.readlines()
    determineStartStop(content, lookup)
    
    template = openTemplate(templateName)
    
    content.insert(stop, template + "\n")
    datafile.truncate()
    datafile.seek(0)
    datafile.writelines(content)
    
    
def openTemplate(templateName):
  with open(templateName, "r+") as templateFile:
    template = templateFile.read()
    template = template.replace("{CommandName}", name).replace("{VclType}", vclType).replace("{return}", returnType).replace("{CommandName_small}", nameSmall)

    for i in range(len(parameterVariables)):
      if i == len(parameterVariables) - 1:
        template = template.replace("{Parameter_Variables}", parameterVariables[i])
      template = template.replace("{Parameter_Variables}", parameterVariables[i] + " " + ",{Parameter_Variables}")
    for i in range(len(parameters)):
      if i % 2 == 1:
        continue
      if i == len(parameters) - 2:
        template = template.replace("{Parameters}", parameters[i] + " " + parameters[i + 1])
        return template
      template = template.replace("{Parameters}", parameters[i] + " " + parameters[i + 1] + ",{Parameters}")
    if len(parameters) == 0:
      template.replace("{Parameters}", "")
    return template


def generateCommandsHdr():
  refreshSrc()
  generateVcl()

  if type == Types.proc:
    for i in processingFiles:
      if (i == 2):
        openFile(procDir + processingFiles[i - 1], templateDir + processingTemplates[i], "#undef PYTHON_MAGIC_DEFINITION")
      else:
        openFile(procDir + processingFiles[i], templateDir + processingTemplates[i], "#undef PYTHON_MAGIC")

    for i in frontendFiles:
      openFile(frontendDir + frontendFiles[i], templateDir + frontendTemplates[i], "#undef PYTHON_MAGIC")
    
    generateRequest("#undef PYTHON_MAGIC_PROC")
    if hasReply == "true":
      generateReply("#undef PYTHON_MAGIC_PROC")
    
    copyFiles(Types.proc)
  else:  
    for i in ioFiles:
      if i == 2:
        openFile(ioDir + ioFiles[i - 1], templateDir + ioTemplates[i], "#undef PYTHON_MAGIC_DEFINITION")
      else:
        openFile(ioDir + ioFiles[i], templateDir + ioTemplates[i], "#undef PYTHON_MAGIC")

    for i in commonFiles:
      openFile(commonDir + commonFiles[i], templateDir + commonTemplates[i], "#undef PYTHON_MAGIC")
    
    generateRequest("#undef PYTHON_MAGIC_IO")
    if hasReply == "true":
      generateReply("#undef PYTHON_MAGIC_IO")
        
    copyFiles(Types.io)
    
  
def generateRequest(lookup):
  openFile(commDir + commFiles[0], templateDir + "RequestTemplate.cpp", lookup)
  
  
def generateReply(lookup):
  openFile(commDir + commFiles[1], templateDir + "ReplyTemplate.cpp", lookup)
  

def generateVcl():
  openFile(commDir + commFiles[2], templateDir + "VclTemplate.h", "#undef PYTHON_ENUM")
  openFile(commDir + commFiles[2], templateDir + "VclTemplate2.h", "#undef PYTHON_MAGIC_STRING")
  
  
def refreshSrc():
  copyfile("../src/commands/" + processingFiles[0], procDir + processingFiles[0])
  copyfile("../src/commands/" + processingFiles[1], procDir + processingFiles[1])
  copyfile("../src/processing-base/" + processingFiles[3], procDir + processingFiles[3])
  copyfile("../src/processing-base/" + processingFiles[4], procDir + processingFiles[4])
  copyfile("../src/processing-base/" + processingFiles[5], procDir + processingFiles[5])
  
  copyfile("../src/commands/" + ioFiles[0], ioDir + ioFiles[0])
  copyfile("../src/commands/" + ioFiles[1], ioDir + ioFiles[1])
  copyfile("../src/io-base/" + ioFiles[3], ioDir + ioFiles[3])
  copyfile("../src/io-base/" + ioFiles[4], ioDir + ioFiles[4])
  copyfile("../src/io-base/" + ioFiles[5], ioDir + ioFiles[5])
      
  copyfile("../src/commands/" + commFiles[0], commDir + commFiles[0])
  copyfile("../src/commands/" + commFiles[1], commDir + commFiles[1])
  copyfile("../src/commands/" + commFiles[2], commDir + commFiles[2])
  
  copyfile("../src/common/" + commonFiles[0], commonDir + commonFiles[0])
  copyfile("../src/common/" + commonFiles[1], commonDir + commonFiles[1])
  #copyfile("../src/common/" + commonFiles[2], commonDir + commonFiles[2])
  
  copyfile("../src/frontend-base/" + frontendFiles[0], frontendDir + frontendFiles[0])
  copyfile("../src/frontend-base/" + frontendFiles[1], frontendDir + frontendFiles[1])


def copyFiles(type):
    if type == Types.proc:
      copyfile(procDir + processingFiles[0], "../src/commands/" + processingFiles[0])
      copyfile(procDir + processingFiles[1], "../src/commands/" + processingFiles[1])
      copyfile(procDir + processingFiles[3], "../src/processing-base/" + processingFiles[3])
      copyfile(procDir + processingFiles[4], "../src/processing-base/" + processingFiles[4])
      copyfile(procDir + processingFiles[5], "../src/processing-base/" + processingFiles[5])
    else:
      copyfile(ioDir + ioFiles[0], "../src/commands/" + ioFiles[0])
      copyfile(ioDir + ioFiles[1], "../src/commands/" + ioFiles[1])
      copyfile(ioDir + ioFiles[3], "../src/io-base/" + ioFiles[3])
      copyfile(ioDir + ioFiles[4], "../src/io-base/" + ioFiles[4])
      copyfile(ioDir + ioFiles[5], "../src/io-base/" + ioFiles[5])
      
    copyfile(commDir + commFiles[0], "../src/commands/" + commFiles[0])
    copyfile(commDir + commFiles[1], "../src/commands/" + commFiles[1])
    copyfile(commDir + commFiles[2], "../src/commands/" + commFiles[2])


def readFile(datafile):
	return datafile.readlines()


def determineStartStop(content, lookup):
  global start
  global stop
  start, stop = 0, 0
  tmpStop = 0
  for line in content:
    tmpStop = tmpStop + 1
    if lookup in line:
      stop = tmpStop - 1
      start = tmpStop - 2
      
      
def main():
	global type
	global name
	global nameSmall
	global hasReply
	global vclType
	global returnType
	global parameters
	global parameterVariables
	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--hasReply', help="shows if the command has a reply ('true' or 'false'")
	#parser.add_argument('--vclType', help="VclType of the command")
	parser.add_argument('--returnType', help="return value of the command")
	parser.add_argument('--parameters', nargs='*', help="all parameters of the command ('please specify all parameters successive, e.g., int x float y double z')")
	
	args = parser.parse_args()
	if (args.type == "proc"):
		type = Types.proc
	elif (args.type == "io"):
		type = Types.io
	else:
		raise("Invalid type: must be 'proc' or 'io'")
	      
	if not args.name:
		raise("Invalid name: give it a name!")
	
	if (args.hasReply != "true" and args.hasReply != "false"):
		raise("Invalid reply: must be 'true' or 'false'")
	      
	if not args.returnType:
		raise("Invalid return type: specify something!")
	
	if not args.parameters or not isinstance(args.parameters, list) or len(args.parameters) % 2 != 0:
		raise("There's something wrong with the specified parameters!")
	
	#if not args.vclType:
	#	raise("Invalid VclType: specify something!")
		
	name = args.name
	nameSmall = str(name[0].lower() + name[1:])
	hasReply = args.hasReply
	vclType = args.name
	returnType = args.returnType
	parameters = args.parameters
	parameterVariables = parameters[1::2]
	
	generateCommandsHdr()
	
	
main()
