#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

enum fret{OPEN, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15};
const int SOFT_POT_PIN = A0; // Pin connected to softpot wiper
int  instrument = 2;
int note = 40;
byte resetMIDI = 4; //Tied to VS1053 Reset line
byte ledPin = 13; //MIDI traffic inidicator
enum fret currentFret;

void setup() {
  Serial.begin(9600);
  //Setup soft serial for MIDI control
  mySerial.begin(31250);
  
  pinMode(SOFT_POT_PIN, INPUT);
  
  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)
}

void loop() {
  talkMIDI(0xB0, 0, 0x79); //Default bank GM1    
  
  // Read in the soft pot's ADC value
  int softPotADC = analogRead(SOFT_POT_PIN); 
  enum fret lastFret = currentFret;   // Hopefully this works in the first loop lol
  currentFret = findFret(softPotADC);
  
  // Figure out if a new note is being played
  boolean newNote = currentFret != lastFret;

  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
  
  //if pot is pressed, play note
  if (softPotADC > 10 && newNote) {
    
    if (softPotADC <= 71) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 40, 60);
      delay(50);
    }
    else if (softPotADC <= 139) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 42, 60);
      delay(50);
    }
    else if (softPotADC <= 207) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 44, 60);
      delay(50);
    }
    else if (softPotADC <= 275) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 45, 60);
      delay(50);
    }
    else if (softPotADC <= 343) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 47, 60);
      delay(50);
    }
    else if (softPotADC <= 411) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 49, 60);
      delay(50);
    }
    else if (softPotADC <= 479) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 51, 60);
      delay(50);
    }
    else if (softPotADC <= 547) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 52, 60);
      delay(50);
    }
    else if (softPotADC <= 615) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 54, 60);
      delay(50);
    }
    else if (softPotADC <= 683) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 56, 60);
      delay(50);
    }
    else if (softPotADC <= 751) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 57, 60);
      delay(50);
    }
    else if (softPotADC <= 819) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 59, 60);
      delay(50);
    }
    else if (softPotADC <= 887) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 61, 60);
      delay(50);
    }
    else if (softPotADC <= 955) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 63, 60);
      delay(50);
    }
    else if (softPotADC <= 1023) {
      //Note on channel 0, some note value (note), middle velocity (0x45):
      noteOn(0, 64, 60);
      delay(50);
    }
    else {
      //do nothing
    }    
  } 
}

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
