#!/usr/bin/env python3
import sys

if len(sys.argv) < 3:
    print("usage: {} <codesfile> <EEPROMbinfile>".format(sys.argv[0]))
    exit()

codesloaded = 0

codesfile = open(sys.argv[1],'r') # read input file
EEPROMbinfile = open(sys.argv[2],'wb') # write BYTES to output file

for line in codesfile:
    linecodenumber = line[:-1].split(' ')[0] # the [:-1] chops off the newline at the end of the line
    if len(linecodenumber) > 9:
        print("skipping input file line with more than 9 digits: "+line[:-1])
        continue
    try:  
        EEPROMbinfile.write(int(linecodenumber, 10).to_bytes(4,'little'))
        print(line[:-1])
    except:
        print("skipping input file line not starting with a number: "+line[:-1])
        continue
    codesloaded += 1 # we loaded another code
    if codesloaded > 255:
        print("255 codes loaded, EEPROM file full")
        EEPROMbinfile.close()
        exit()

print("loaded {} codes from {}".format(codesloaded, sys.argv[1]))

while codesloaded < 256:
    EEPROMbinfile.write(b'\xFF\xFF\xFF\xFF')
    codesloaded += 1

EEPROMbinfile.close()
exit()
