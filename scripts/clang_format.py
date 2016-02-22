import os
import subprocess
import sys

rootDir = sys.argv[1]
clangFormatBinary = ".\clang-format.exe"

def checkFile(path):
	if "thirdparty" in path or "gmock" in path or "gtest" in path or "silverbullet" in path or "tri-watch" in path or "tri-android" in path or "processing-gridleaper" in path:
		return False
	ext = os.path.splitext(name)[1]
	if ext <> ".cpp" and ext <> ".h":
		return False
	return True

for root, dirs, files in os.walk(rootDir, topdown=False):
    for name in files:
				sourceFile = os.path.join(root, name)
				if checkFile(sourceFile):
					print "Formatting: " + sourceFile
					subprocess.call([clangFormatBinary, "-i", "-style=file", "-sort-includes", sourceFile])