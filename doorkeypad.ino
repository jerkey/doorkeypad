#define BAUDRATE        300 // serial port baud rate
#define COL1 2    // 2       0,2,5,7
#define ROW1 3    // 3       9,0,#
#define COL2 4    // 4       #,3,*,8
#define ROW2 5    // 5       3,2,1
#define COL3 6    // 6       9,1,4,6
#define ROW3 7    // 7       4,5,*
#define ROW4 8    // 8       6,7,8
#define REDPIN 9  // 
#define GREENPIN 11  // 
#define BLUEPIN  10  // 
#define NOISEPIN 12  // 
#define SADTONE 200  // sad tone frequency
#define SADTIME 500  // sadtone time in milliseconds
#define HAPPYTONE 1000  // happy tone frequency
#define HAPPYTIME 500   // happy tone time in milliseconds
#define CLICKTONE 5000  // button press noise frequency
#define CLICKTIME 25    // button press noise time in milliseconds
#define STAR    11
#define POUND   12
#define ENTRYCODETIMEOUT 5000 // after no button pressed, clear entry code attempt

static uint32_t validcodes[256] = {0}; // RAM array to store valid codes

// int one,two,three,four,five,six,seven,eight,nine,zero,star,pound;
int butt[13] = {
  0,0,0,0,0,0,0,0,0,0,0,0};
char chars[13] = {
  'X','1','2','3','4','5','6','7','8','9','0','*','#'};
String entrycode = ""; // string gets built up to try an access code

// THIS PROGRAM IS FOR A TWELVE-BUTTON NUMERIC KEYPAD FOR SECURE OPENING
// OF THE GATE AT A HACKERSPACE OR ANY OTHER PLACE.
// IT SENDS THE CHARACTER PRESSED THROUGH THE SERIAL PORT,
// AND IT CAN RECIEVE COMMANDS TO TURN ON LEDS OR MAKE BEEPY NOISES.

unsigned long lastime = 0;
unsigned long redlastime = 0;
unsigned long greenlastime = 0;
unsigned long bluelastime = 0;
unsigned long noiselastime = 0;
#define DEBOUNCETIME 250   // minimum milliseconds between buttonpresses
#define REDTIME 1000 
#define GREENTIME 1000 
#define BLUETIME 1000 
int lastbutt = 0;

#include <EEPROM.h>

void loadFromEEPROM() {
  for (int i=0; i < 256; i++) EEPROM.get(i * 4, validcodes[i]); // load codes from EEPROM
}

void tryentrycode(char latestchar) {
  if (latestchar == '*') {
    entrycode = ""; // clear the code
    return;
  }
  if (latestchar != '#') entrycode += latestchar; // tack on digit unless #
  if ((entrycode.length() > 8) || (latestchar == '#')) { // if enough digits or #
    char arr[12];
    entrycode.toCharArray(arr, sizeof(arr));
    uint32_t attemptedcode = atol(arr);
    for (int i=0; i < 256; i++) {
      if (attemptedcode == validcodes[i]) {
        Serial.print("\n"+String(attemptedcode)+" found in record "+String(i));
      }
    }
    entrycode = "";
  }
}


void initPins()
{
  pinMode(COL1,OUTPUT);
  pinMode(COL2,OUTPUT);
  pinMode(COL3,OUTPUT);
  pinMode(ROW1,INPUT);
  pinMode(ROW2,INPUT);
  pinMode(ROW3,INPUT);
  pinMode(ROW4,INPUT);
  digitalWrite(ROW1,HIGH);
  digitalWrite(ROW2,HIGH);
  digitalWrite(ROW3,HIGH);
  digitalWrite(ROW4,HIGH);
  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);
  pinMode(NOISEPIN,OUTPUT);
  digitalWrite(GREENPIN,HIGH);
  digitalWrite(REDPIN,HIGH);
  digitalWrite(BLUEPIN,HIGH);
}

void scanButts()
{
  digitalWrite(COL1,LOW); // 2       0,2,5,7
  digitalWrite(COL2,HIGH);
  digitalWrite(COL3,HIGH);
  butt[10] = !digitalRead(ROW1);
  butt[2] = !digitalRead(ROW2);
  butt[5] = !digitalRead(ROW3);
  butt[7] = !digitalRead(ROW4);

  digitalWrite(COL1,HIGH);
  digitalWrite(COL2,LOW); // 4       #,3,*,8
  digitalWrite(COL3,HIGH);
  butt[POUND] = !digitalRead(ROW1);
  butt[3]     = !digitalRead(ROW2);
  butt[STAR]  = !digitalRead(ROW3);
  butt[8]     = !digitalRead(ROW4);

  digitalWrite(COL1,HIGH);
  digitalWrite(COL2,HIGH);
  digitalWrite(COL3,LOW); // 6       9,1,4,6
  butt[9] = !digitalRead(ROW1);
  butt[1] = !digitalRead(ROW2);
  butt[4] = !digitalRead(ROW3);
  butt[6] = !digitalRead(ROW4);
}

void setup()
{
  initPins();
  Serial.begin(BAUDRATE);
  Serial.print("github.com/jerkey/doorkeypad");
  loadFromEEPROM(); // load access codes from EEPROM into RAM
  // for (int l = 0; l<8; l++) Serial.println(String(l)+"	"+validcodes[l]); // for diagnosing EEPROM
  tone(NOISEPIN, HAPPYTONE, HAPPYTIME);
}

void loop()
{
  scanButts(); // update array of which buttons are pressed
  int tempbutt = oneisdown(); // returns 0 unless exactly one button is pressed
  if (tempbutt != 0) {
    if (tempbutt != lastbutt) // only do this once per button press
      if  (millis() - lastime > DEBOUNCETIME) {
        Serial.print(chars[tempbutt]);
        tryentrycode(chars[tempbutt]); // see if we've entered a code
        lastbutt = tempbutt;
        lastime = millis();
        tone(NOISEPIN, CLICKTONE, CLICKTIME);
      }   
  } 
  else       
    lastbutt = tempbutt;
  handleserial();
  if (millis() - lastime > ENTRYCODETIMEOUT) {
    entrycode = ""; // clear the code, time went by
  }
}

int oneisdown() {
  int prest,sum = 0;
  for (int i = 1; i < 13; i++) {
    sum += butt[i];
    if (butt[i]) prest = i;
  }
  if (sum == 1) {
    return prest;
  } else {
    return 0;
  }
}

void handleserial() {
  if (Serial.available() > 0) {
    switch (Serial.read()) {
    case 'R':    // R is for Red LED
      digitalWrite(REDPIN,HIGH);
      redlastime = millis();
      break;
    case 'G':    // G is for Green LED
      digitalWrite(GREENPIN,HIGH);
      greenlastime = millis();
      break;
    case 'B':    // B is for Blue LED
      digitalWrite(BLUEPIN,HIGH);
      bluelastime = millis();
      break;
    case 'H': // make a happy tone
      tone(NOISEPIN, HAPPYTONE, HAPPYTIME);
      break;
    case 'S': // make a sad tone
      tone(NOISEPIN, SADTONE, SADTIME);
      break;
    case 'Q':  // mute the current tone and print QUIET
      noTone(NOISEPIN);
      Serial.println("QUIET");
      break;
    }
  }
  if (millis() - redlastime > REDTIME) digitalWrite(REDPIN,LOW);
  if (millis() - greenlastime > GREENTIME) digitalWrite(GREENPIN,LOW);
  if (millis() - bluelastime > BLUETIME) digitalWrite(BLUEPIN,LOW);
}
