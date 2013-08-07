const int col1 = 6;  // 12 amarillo
const int col2 = 7;  // 13 verde
const int col3 = 8;  // 14 anaranjado
const int row1 = 9;  // 15 azul
const int row2 = 10; // 16 rojo
const int row3 = 11; // 17 negro
const int row4 = 12; // 18 blanco
const int redpin = 2;// 4 red LED +
const int greenpin = 13; // 19 green LED +
const int bluepin = 5; // 11 blue LED +
const int noisepin = 3;  // 5 noisemaker
const int sadtone = 200; // sad tone frequency
const int sadtime = 500; // sadtone time in milliseconds
const int happytone = 1000;  // happy tone frequency
const int happytime = 500;  // happy tone time in milliseconds
const int clicktone = 5000;  // button press noise frequency
const int clicktime = 25;  // button press noise time in milliseconds

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
  pinMode(col1,OUTPUT);
  pinMode(col2,OUTPUT);
  pinMode(col3,OUTPUT);
  pinMode(row1,INPUT);
  pinMode(row2,INPUT);
  pinMode(row3,INPUT);
  pinMode(row4,INPUT);
  digitalWrite(row1,HIGH);
  digitalWrite(row2,HIGH);
  digitalWrite(row3,HIGH);
  digitalWrite(row4,HIGH);
  Serial.begin(300);          // BAUD RATE IS SET HERE
  pinMode(redpin,OUTPUT);
  pinMode(greenpin,OUTPUT);
  pinMode(bluepin,OUTPUT);
  pinMode(noisepin,OUTPUT);
  Serial.println("HELLO");
  digitalWrite(greenpin,HIGH);
  digitalWrite(redpin,HIGH);
  digitalWrite(bluepin,HIGH);
  nowtime = millis();
  tone(3, happytone, happytime);
}

void loop()
{
  digitalWrite(col1,LOW);
  digitalWrite(col2,HIGH);
  digitalWrite(col3,HIGH);
  butt[1] = !digitalRead(row1);
  butt[4] = !digitalRead(row2);
  butt[7] = !digitalRead(row3);
  butt[11] = !digitalRead(row4);

  digitalWrite(col1,HIGH);
  digitalWrite(col2,LOW);
  digitalWrite(col3,HIGH);
  butt[2] = !digitalRead(row1);
  butt[5] = !digitalRead(row2);
  butt[8] = !digitalRead(row3);
  butt[10] = !digitalRead(row4);

  digitalWrite(col1,HIGH);
  digitalWrite(col2,HIGH);
  digitalWrite(col3,LOW);
  butt[3] = !digitalRead(row1);
  butt[6] = !digitalRead(row2);
  butt[9] = !digitalRead(row3);
  butt[12] = !digitalRead(row4);

  int tempbutt = oneisdown();
  nowtime = millis();
  if (tempbutt != 0) {
    if (tempbutt != lastbutt)
      if  (nowtime - lastime > debouncetime) {
        Serial.print(chars[tempbutt]);
        lastbutt = tempbutt;
        lastime = nowtime;
              tone(3, clicktone, clicktime);
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
      digitalWrite(redpin,HIGH);
      redlastime = nowtime;
      break;
    case 'G':    // G is for Green LED
//      Serial.println("GREEN");
      digitalWrite(greenpin,HIGH);
      greenlastime = nowtime;
      break;
    case 'B':    // B is for Blue LED
//      Serial.println("BLUE");
      analogWrite(bluepin,127);
      bluelastime = nowtime;
      break;
    case 'H': // make a happy tone
      tone(3, happytone, happytime);
//      Serial.println("HAPPY");
      break;
    case 'S': // make a sad tone
      tone(3, sadtone, sadtime);
//      Serial.println("SAD");
      break;
    case 'Q':  // mute the current tone and print QUIET
      noTone(3);
      Serial.println("QUIET");
      break;
    }
  }
  if (nowtime - redlastime > redtime) digitalWrite(redpin,LOW);
  if (nowtime - greenlastime > greentime) digitalWrite(greenpin,LOW);
  if (nowtime - bluelastime > bluetime) digitalWrite(bluepin,LOW);
}










