#!/usr/bin/python

import sys

if len(sys.argv) != 2:
	print "usage: <program-name> <input-file>"
	exit(-1);
firstVal = True
prevTime = 0.0
largestDiffTime = -sys.maxsize - 1
largestDiffString = ""
prevLargestDiffString = ""
smallestDiffTime = sys.maxsize
smallestDiffString = ""
prevSmallestDiffString = ""
prevLine = ""
inFile = open(sys.argv[1], 'r')
for line in inFile :
	if "Bi:1:018:1 0 19" in line :
		currentTime = int(line.split(" ")[1])
		if firstVal :
			firstVal = False
		else :
			diffTime = currentTime - prevTime;
			print diffTime
			if diffTime > largestDiffTime :
				largestDiffTime = diffTime
				largestDiffString = line
				prevLargestDiffString = prevLine
			elif diffTime < smallestDiffTime :
				smallestDiffTime = diffTime
				smallestDiffString = line
				prevSmallestDiffString = prevLine
		prevTime = currentTime
		prevLine = line

print "largest diff time: " + repr(largestDiffTime)
print "smallest diff time: " + repr(smallestDiffTime)
print largestDiffString
print prevLargestDiffString
print smallestDiffString
print prevSmallestDiffString
