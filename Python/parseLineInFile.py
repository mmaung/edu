#!/usr/bin/python

import sys

if len(sys.argv) != 2:
	print "usage: <program-name> <input-file>"
	exit(-1);
firstVal = True
prevTime = 0.0
largestDiffTime = -sys.maxsize - 1;
smallestDiffTime = sys.maxsize;
inFile = open(sys.argv[1], 'r')
for line in inFile :
	if "Zi:1:062:1" in line :
		currentTime = int(line.split(" ")[1])
		if firstVal :
			firstVal = False
		else :
			diffTime = currentTime - prevTime;
			print diffTime
			if diffTime > largestDiffTime :
				largestDiffTime = diffTime
			elif diffTime < smallestDiffTime :
				smallestDiffTime = diffTime
		prevTime = currentTime

print "largest diff time: " + repr(largestDiffTime)
print "smallest diff time: " + repr(smallestDiffTime)

