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
    template = template.replace("{CommandName}", name).replace("{VclType}", vclType)
    return template


def generateCommandsHdr():
  refreshSrc()

  if type == Types.proc:
    for i in processingFiles:
      if (i == 2):
        openFile(procDir + processingFiles[i - 1], templateDir + processingTemplates[i], "#undef PYTHON_MAGIC_DEFINITION")
      else:
        openFile(procDir + processingFiles[i], templateDir + processingTemplates[i], "#undef PYTHON_MAGIC")

    generateRequest("#undef PYTHON_MAGIC_PROC")
    if hasReply != "false":
      generateReply("#undef PYTHON_MAGIC_PROC")
    
    copyFiles(Types.proc)
  else:  
    for i in ioFiles:
      if i == 2:
        openFile(ioDir + ioFiles[i - 1], templateDir + ioTemplates[i], "#undef PYTHON_MAGIC_DEFINITION")
      else:
        openFile(ioDir + ioFiles[i], templateDir + ioTemplates[i], "#undef PYTHON_MAGIC")

    generateRequest("#undef PYTHON_MAGIC_IO")
    if hasReply != "false":
      generateReply("#undef PYTHON_MAGIC_IO")
    
    generateVcl()
    
    copyFiles(Types.io)

  
def generateRequest(lookup):
  openFile(commDir + commFiles[0], templateDir + "RequestTemplate.cpp", lookup)
  
  
def generateReply(lookup):
  openFile(commDir + commFiles[1], templateDir + "ReplyTemplate.cpp", lookup)
  

def generateVcl():
  openFile(commDir + commFiles[2], templateDir + "VclTemplate.h", "#undef PYTHON_MAGIC")
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
	global hasReply
	global vclType
	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	parser.add_argument('--hasReply', help="shows if the command has a reply ('true' or 'false'")
	parser.add_argument('--vclType', help="VclType of the command")
	
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
	      
	if not args.vclType:
		raise("Invalid VclType: specify something!")
		
	name = args.name
	hasReply = args.hasReply
	vclType = args.vclType
	
	generateCommandsHdr()
	
	
main()