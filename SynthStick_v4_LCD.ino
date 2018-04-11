#include <SoftwareSerial.h>
#include <string.h>
#include <LiquidCrystal.h>

SoftwareSerial mySerial(2, 3); // RX, TX

// Enums
enum fret{OPEN, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15};
enum scale { majorEnum, majPentEnum, minorPentEnum };

// Scale Notes Starting from C
int major[] = {36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60};
int majorPent[] = {36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69};
int minorPent[] = {36, 39, 41, 42, 43, 46, 48, 51, 53, 54, 55, 58, 60, 63, 65};

// Current Scale being used
int scaleNotes[15];
int keyRoot = 36;

// Arduino Stuff
const int SOFT_POT_PIN = A0; // Pin connected to softpot wiper
byte resetMIDI = 4; //Tied to VS1053 Reset line
byte ledPin = 13; //MIDI traffic inidicator

// Used to make music!
int  instrument = 2;
enum fret currentFret;
enum scale currentScale;

// Instrument up button, instrument down button, scale change button
int iupPin = 7; // choose the input pin for inst up push button
int idownPin = 6; // choose the input pin for inst down push button
int scaleBPin = 5; // choose the input pin for change scale button
int keyBPin = 4; // choose the input pin for change key button
int iupVal = 0;
int idownVal = 0;
int scaleBVal = 0;
int keyBVal = 0;
boolean iupPressed = false;
boolean idownPressed = false;
boolean scaleBPressed = false;
boolean keyBPressed = false;

// Function Protoypes
void changeScale();
void instrumentUp();
void instrumentDown();
void changeKey();
void getMajorNotes();
void getMajorPentNotes();
void getMinorPentNotes();
void updateLCD();

//String Arrays for LCD
String key[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
String scales[] = {"Major", "MajorPent", "MinorPent"};

//LCD Pins
LiquidCrystal lcd(12, 11, 10, 8, 3, 2);

void setup() {
  Serial.begin(9600);
  //Setup soft serial for MIDI control
  mySerial.begin(31250);
  
  pinMode(SOFT_POT_PIN, INPUT);
  pinMode(iupPin, INPUT); //instrument up selector
  pinMode(idownPin, INPUT); // instrument down selector
  pinMode(scaleBPin, INPUT); // change scale selector
  pinMode(keyBPin, INPUT); // change pin selector
  
  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)
  memcpy(scaleNotes, major, sizeof(major));
  currentScale = majorEnum;

  //Setup LCD
  lcd.begin(16, 2);
  updateLCD();
}

void loop() {
  updateLCD();
  
  iupPressed = iupVal;
  iupVal = digitalRead(iupPin);
  
  idownPressed = idownVal;
  idownVal = digitalRead(idownPin);
  
  scaleBPressed = scaleBVal;
  scaleBVal = digitalRead(scaleBPin);
  
  keyBPressed = keyBVal;
  keyBVal = digitalRead(keyBPin);
  
  //Serial.println(iupVal);
  if (iupVal == HIGH && iupPressed == false) {
    Serial.println(instrument);
    instrumentUp();
  }  
  
  if (idownVal == HIGH && idownPressed == false) {
    Serial.println(instrument);
    instrumentDown();
  }
  
  if (scaleBVal == HIGH && scaleBPressed == false) {
    changeScale();
  } 
  
  if (keyBVal == HIGH && keyBPressed == false) {
    changeKey();
  }
  
  talkMIDI(0xB0, 0, 0x79); //Default bank GM1  
  
  // Read in the soft pot's ADC value
  int softPotADC = analogRead(SOFT_POT_PIN); 
  enum fret lastFret = currentFret;   // Hopefully this works in the first loop lol
  currentFret = findFret(softPotADC);
  
  // Figure out if a new note is being played
  boolean newNote = (currentFret != lastFret);

  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
  
  //if pot is pressed, play note
  if (softPotADC > 10 && newNote) {
    
    if (softPotADC <= 71) {
       //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[0], 60);
      delay(50);
    }
    else if (softPotADC <= 139) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[1], 60);
      delay(50);
    }
    else if (softPotADC <= 207) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[2], 60);
      delay(50);
    }
    else if (softPotADC <= 275) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[3], 60);
      delay(50);
    }
    else if (softPotADC <= 343) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[4], 60);
      delay(50);
    }
    else if (softPotADC <= 411) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[5], 60);
      delay(50);
    }
    else if (softPotADC <= 479) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[6], 60);
      delay(50);
    }
    else if (softPotADC <= 547) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[7], 60);
      delay(50);
    }
    else if (softPotADC <= 615) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[8], 60);
      delay(50);
    }
    else if (softPotADC <= 683) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[9], 60);
      delay(50);
    }
    else if (softPotADC <= 751) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[10], 60);
      delay(50);
    }
    else if (softPotADC <= 819) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[11], 60);
      delay(50);
    }
    else if (softPotADC <= 887) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[12], 60);
      delay(50);
    }
    else if (softPotADC <= 955) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[13], 60);
      delay(50);
    }
    else if (softPotADC <= 1023) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, scaleNotes[14], 60);
      delay(50);
    }
    else {
      //do nothing
    }    
    
    //Turn off the note with a given off/release velocity
//    noteOff(0, note, 60);
//    delay(50);
  } 
}

// Find the "fret" where the note was played
enum fret findFret(int softPotADC) {
    if (softPotADC == 0) {
  return OPEN;
    }
    else if (softPotADC <= 71) {
        return F1;
    }
    else if (softPotADC <= 139) {
        return F2;
    }
    else if (softPotADC <= 207) {
        return F3;
    }
    else if (softPotADC <= 275) {
        return F4;
    }
    else if (softPotADC <= 343) {
        return F5;
    }
    else if (softPotADC <= 411) {
        return F6;
    }
    else if (softPotADC <= 479) {
        return F7;
    }
    else if (softPotADC <= 547) {
        return F8;
    }
    else if (softPotADC <= 615) {
        return F9;
    }
    else if (softPotADC <= 683) {
        return F10;
    }
    else if (softPotADC <= 751) {
        return F11;
    }
    else if (softPotADC <= 819) {
        return F12;
    }
    else if (softPotADC <= 887) {
        return F13;
    }
    else if (softPotADC <= 955) {
        return F14;
    }
    else { // if (softPotADC <= 1023) {
        return F15;
    }
}

// Update LCD
void updateLCD() {
  //Instrument
  lcd.setCursor(0,0);
  lcd.print("Ins: ");
  lcd.setCursor(5,0);
  lcd.print(instrument);

  //Key
  lcd.setCursor(9,0);
  lcd.print("Key: ");
  lcd.setCursor(14,0);
  lcd.print(key[keyRoot-36]);

  //Scale
  lcd.setCursor(0,1);
  lcd.print("S: ");
  lcd.setCursor(4,1);
  lcd.print(scales[currentScale]);

//Just in case, but probably won't need it since volume will be analog
//  //Volume
//  lcd.setCursor(8,1);
//  lcd.print("vol: ");
//  lcd.setCursor(14,1);
//  lcd.print("11");
}

// Change the current scale of notes being used
void changeScale() {
  if (currentScale == majorEnum) {
    memcpy(scaleNotes, majorPent, sizeof(majorPent));
    currentScale = majPentEnum;
  }
  else if (currentScale == majPentEnum) {
    memcpy(scaleNotes, minorPent, sizeof(majorPent));
    currentScale = minorPentEnum;
  }
  else {  // if currentScale == minorPentEnum
    memcpy(scaleNotes, major, sizeof(majorPent));
    currentScale = majorEnum;
  }
}

// Toggle instrument up
void instrumentUp() {
  if (instrument == 128) {
      instrument = 1;
  }
  else {
      instrument += 1;
  }
}

// Toggle instrument down
void instrumentDown() {
  if (instrument == 1) {
      instrument = 128;
  } 
  else {
      instrument -= 1;
  }
}

// Updates the array of Major notes
void getMajorNotes() {
  major[0] = keyRoot;
  int step = 0;
  
  for (int i = 1; i < 15; i++) {
    step++;
    if (step == 3 || step == 7) {
      major[i] = (major[i-1] + 1);
    }
    else {
      major[i] = (major[i-1] + 2);
    }
    
    if (step == 7) step = 0;
  }
}

// Updates the array of Major Pentatonic Notes
void getMajorPentNotes() {
  majorPent[0] = keyRoot;
  int step = 0;
  
  for (int i = 1; i < 15; i++) {
    step++;
    if (step == 3 || step == 5) {
      majorPent[i] = majorPent[i-1] + 3;
    }
    else {
      majorPent[i] = majorPent[i-1] + 2;
    }
    
    if (step == 5) step = 0;
  }
}

// Updates the array of Minor Pentatonic Notes
void getMinorPentNotes() {
  minorPent[0] = keyRoot;
  int step= 0;
  
  for (int i = 1; i < 15; i++) {
    step++;
    if (step == 1 || step == 5) {
      minorPent[i] = minorPent[i-1] + 3;
    }
    else if (step == 2 || step == 6) {
      minorPent[i] = minorPent[i-1] + 2;
    }
    else {
      minorPent[i] = minorPent[i-1] + 2;
    }
    
    if (step == 6) step = 0;
  }
}

// Changes the key
void changeKey() {
  // Update root note
  if (keyRoot == 47) {
    keyRoot = 36;
  }
  else {
    keyRoot++;
  }
  
  // Change all scale's keys
  getMajorNotes();
  getMajorPentNotes();
  getMinorPentNotes();
  
  // Update current scale to be copy of current scale in new key
  if (currentScale == majorEnum) {
    memcpy(scaleNotes, major, sizeof(major));
  }
  else if (currentScale == majPentEnum) {
    memcpy(scaleNotes, majorPent, sizeof(majorPent));
  }
  else { // currentScale == minorPentEnum
    memcpy(scaleNotes, minorPent, sizeof(minorPent));
  }
}

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

  digitalWrite(ledPin, LOW);
}
