#define COL1 6   // 12 amarillo
#define COL2 7   // 13 verde
#define COL3 8   // 14 anaranjado
#define ROW1 9   // 15 azul
#define ROW2 10  // 16 rojo
#define ROW3 11  // 17 negro
#define ROW4 12  // 18 blanco
#define REDPIN 2 // 4 red LED +
#define GREENPIN 13  // 19 green LED +
#define BLUEPIN 5  // 11 blue LED +
#define NOISEPIN 3   // 5 noisemaker
#define SADTONE 200  // sad tone frequency
#define SADTIME 500  // sadtone time in milliseconds
#define HAPPYTONE 1000   // happy tone frequency
#define HAPPYTIME 500   // happy tone time in milliseconds
#define CLICKTONE 5000   // button press noise frequency
#define CLICKTIME 25   // button press noise time in milliseconds

// int one,two,three,four,five,six,seven,eight,nine,zero,star,pound;
int butt[13] = {
  0,0,0,0,0,0,0,0,0,0,0,0};
char chars[13] = {
  'X','1','2','3','4','5','6','7','8','9','0','*','#'};

// THIS PROGRAM IS FOR A TWELVE-BUTTON NUMERIC KEYPAD FOR SECURE OPENING
// OF THE GATE AT A HACKERSPACE OR ANY OTHER PLACE.
// IT SENDS THE CHARACTER PRESSED THROUGH THE SERIAL PORT,
// AND IT CAN RECIEVE COMMANDS TO TURN ON LEDS OR MAKE BEEPY NOISES.

unsigned long lastime = 0;
unsigned long redlastime = 0;
unsigned long greenlastime = 0;
unsigned long bluelastime = 0;
unsigned long noiselastime = 0;
unsigned long nowtime;
unsigned long debouncetime = 250;  // minimum milliseconds between buttonpresses
unsigned long redtime = 1000;
unsigned long greentime = 1000;
unsigned long bluetime = 1000;
unsigned long noisetime = 1000;
int lastbutt = 0;

void setup()
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
  Serial.begin(300);          // BAUD RATE IS SET HERE
  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);
  pinMode(NOISEPIN,OUTPUT);
  Serial.println("HELLO");
  digitalWrite(GREENPIN,HIGH);
  digitalWrite(REDPIN,HIGH);
  digitalWrite(BLUEPIN,HIGH);
  nowtime = millis();
  tone(3, HAPPYTONE, HAPPYTIME);
}

void loop()
{
  digitalWrite(COL1,LOW);
  digitalWrite(COL2,HIGH);
  digitalWrite(COL3,HIGH);
  butt[1] = !digitalRead(ROW1);
  butt[4] = !digitalRead(ROW2);
  butt[7] = !digitalRead(ROW3);
  butt[11] = !digitalRead(ROW4);

  digitalWrite(COL1,HIGH);
  digitalWrite(COL2,LOW);
  digitalWrite(COL3,HIGH);
  butt[2] = !digitalRead(ROW1);
  butt[5] = !digitalRead(ROW2);
  butt[8] = !digitalRead(ROW3);
  butt[10] = !digitalRead(ROW4);

  digitalWrite(COL1,HIGH);
  digitalWrite(COL2,HIGH);
  digitalWrite(COL3,LOW);
  butt[3] = !digitalRead(ROW1);
  butt[6] = !digitalRead(ROW2);
  butt[9] = !digitalRead(ROW3);
  butt[12] = !digitalRead(ROW4);

  int tempbutt = oneisdown();
  nowtime = millis();
  if (tempbutt != 0) {
    if (tempbutt != lastbutt)
      if  (nowtime - lastime > debouncetime) {
        Serial.print(chars[tempbutt]);
        lastbutt = tempbutt;
        lastime = nowtime;
              tone(3, CLICKTONE, CLICKTIME);
      }   
  } 
  else       
    lastbutt = tempbutt;
  handleserial();
}

int oneisdown() {
  int prest,sum = 0;
  for (int i = 1; i < 13; i++) {
    sum += butt[i];
    if (butt[i]) 
      prest = i;
  }
  if (sum == 1) 
    return prest;
  else return 0;
}

void handleserial() {
  if (Serial.available() > 0) {
    switch (Serial.read()) {
    case 'R':    // R is for Red LED
//      Serial.println("RED");
      digitalWrite(REDPIN,HIGH);
      redlastime = nowtime;
      break;
    case 'G':    // G is for Green LED
//      Serial.println("GREEN");
      digitalWrite(GREENPIN,HIGH);
      greenlastime = nowtime;
      break;
    case 'B':    // B is for Blue LED
//      Serial.println("BLUE");
      analogWrite(BLUEPIN,127);
      bluelastime = nowtime;
      break;
    case 'H': // make a happy tone
      tone(3, HAPPYTONE, HAPPYTIME);
//      Serial.println("HAPPY");
      break;
    case 'S': // make a sad tone
 :     tone(3, SADTONE, SADTIME);
//      Serial.println("SAD");
      break;
    case 'Q':  // mute the current tone and print QUIET
      noTone(3);
      Serial.println("QUIET");
      break;
    }
  }
  if (nowtime - redlastime > redtime) digitalWrite(REDPIN,LOW);
  if (nowtime - greenlastime > greentime) digitalWrite(GREENPIN,LOW);
  if (nowtime - bluelastime > bluetime) digitalWrite(BLUEPIN,LOW);
}










