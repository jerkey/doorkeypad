./makeEEPROMfromcodelist.py codesfile EEPROMcodes.bin
read -p "press enter to continue or control-C to abort upload of new EEPROM image"
SERIALPORT=`ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | tail -n 1` || exit 1
/home/user/arduino-1.8.8/hardware/tools/avr/bin/avrdude -C/home/user/arduino-1.8.8/hardware/tools/avr/etc/avrdude.conf -v -V -patmega328p -carduino -P$SERIALPORT -b57600 $@ -Ueeprom:w:EEPROMcodes.bin:r
