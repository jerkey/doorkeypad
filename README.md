codesfile is a text file with lines starting with numbers no more than 9 digits
doorkeypad.ino is the program that goes on an atmega328p wired to a codepad
makeEEPROMfromcodelist.py makes a binary EEPROM image from codesfile
uploadcodes.sh runs makeEEPROMfromcodelist.py and then uploads it to the atmega328p
eepromtest.txt is notes from figuring out byte order after uploading doorkeypad.ino to the EEPROM
