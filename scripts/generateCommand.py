import os
import argparse
from string import Template

class Types:
	proc = 1
	io = 2

srcPath = "../src"

def commandsHdrPath():
	if (type == Types.proc):
		return os.path.join(srcPath, "commands", "ProcessingCommands.h")
	else:
		return os.path.join(srcPath, "commands", "IOCommands.h")


def generateCommandsHdr():

def main():
	global type
	global name
	parser = argparse.ArgumentParser(description="Generate Trinity Command.")
	parser.add_argument('--type', help="type of the command to generate ('proc' or 'io'" )
	parser.add_argument('--name', help="name of the command to generate")
	
	args = parser.parse_args()
	if (args.type == "proc"):
		type = Types.proc
	elif (args.type == "io"):
		type = Types.io
	else:
		raise("Invalid type: must be 'proc' or 'io'")
		
	name = args.name
	
	generateCommandsHdr()
	
	
main()