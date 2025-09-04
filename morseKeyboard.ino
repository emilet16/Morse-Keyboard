#include <Keyboard.h>

const int deletePin = 2;
const int enterPin = 3;
const int spacePin = 4;
const int shiftPin = 5;
const int morsePin = 6;

const int NUM_CHARS = 57;
const int MIN_PRESS_DURATION = 50;
const int LONG_PRESS_DURATION = 300;
const int MAX_PRESS_DELAY = 300; //Wait the equivalent of a dash between words

const static String morse[NUM_CHARS] = {
  "..__..", "._.._.", "___.", ".._..", ".____.", //!"#%'
  "_.__.", "_.__._", ".__._", "._._", "__..__", "_...._", "._._._", //()*+,-./
  "_.._.", "_____", ".____", "..___", "...__", "...._", ".....", "_....", "__...", "___..", "____.", //0123456789
  "___...", "_.___", "__.__", "___._", //:<=>
  "._", "_...", "_._.", "_..", ".", ".._.", "__.", //abcdefg
  "....", "..", ".___", "_._", "._..", "__", "_.", "___", ".__.", //hijklmnop
  "__._", "._.", "...", "_", ".._", "..._", ".__", "_.._", "_.__", "__..", //qrstuvwxyz
  "._.._", "._..__", "..__.", "..__._" //[]{}
}; //All characters required to code in python in a modified version of morse code

const static int ASCII[NUM_CHARS] = {
  33, 34, 35, 37, 39, //!"#%'
  40, 41, 42, 43, 44, 45, 46, 47, //()*+,-./
  48, 49, 50, 51, 52, 53, 54, 55, 56, 57, //0123456789
  58, 60, 61, 62, //:<=>
  97, 98, 99, 100, 101, 102, 103, //abcdefg
  104, 105, 106, 107, 108, 109, 110, 111, 112, //hijklmnop
  113, 114, 115, 116, 117, 118, 119, 120, 121, 122, //qrstuvwxyz
  91, 93, 123, 125 //[]{}
}; //The equivalent string

void setup() {
  pinMode(deletePin, INPUT_PULLUP);
  pinMode(enterPin, INPUT_PULLUP);
  pinMode(spacePin, INPUT_PULLUP);
  pinMode(shiftPin, INPUT_PULLUP);
  pinMode(morsePin, INPUT_PULLUP);

  Keyboard.begin();
}

unsigned long d,t1,t2;
String code = "";

void loop() {
  // put your main code here, to run repeatedly:
  whileNoMorse();
  
NextInput:
  pressDuration();

  if(d>MIN_PRESS_DURATION) { //Ignore very small delays (when the button doesn't connect fully)
    code += readIO();
  }

  while((millis() - t2) < MAX_PRESS_DELAY) { //If the button is pressed again within the delay, add to the input
    if(digitalRead(morsePin) == LOW) {
      goto NextInput;
    }
  }

  writeChar();
}

void whileNoMorse() {
  while(digitalRead(morsePin) == HIGH) {
    if(digitalRead(spacePin) == LOW) {
      Keyboard.write(' ');
      delay(200);
    }

    if(digitalRead(enterPin) == LOW) {
      Keyboard.write(KEY_RETURN);
      delay(200);
    }

    if(digitalRead(deletePin) == LOW) {
      Keyboard.write(KEY_BACKSPACE);
      delay(200);
    }
  }
}

void pressDuration() {
  t1 = millis();
  while(digitalRead(morsePin) == LOW) {}
  t2 = millis();

  d = t2-t1;
}

char readIO() {
  if(d<LONG_PRESS_DURATION) { //If the delay is short
    return '.';
  } else { //If the delay is long
    return '_';
  }
}

void writeChar() {
  int i = 0;
  int caps = (digitalRead(shiftPin) == LOW);
  for(int i=0; i<NUM_CHARS; i++) { //Search through the array for the morse value
    if(morse[i] == code) {
      Keyboard.write(ASCII[i]-32*caps); //If the caps button is pressed, convert the ascii value to the equivalent capital letter
    }
  }

  code = "";
}