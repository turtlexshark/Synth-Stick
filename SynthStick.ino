#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

const int SOFT_POT_PIN = A0; // Pin connected to softpot wiper
int  instrument = 2;
int note = 40;
byte resetMIDI = 4; //Tied to VS1053 Reset line
byte ledPin = 13; //MIDI traffic inidicator

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

  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command

  //if (softPotADC < 10 && softPotADC > 0) {
    Serial.println(softPotADC);
  //}
  
  //if pot is pressed, play note
  if (softPotADC > 10) {
    
    if (softPotADC <= 101) {
       //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 40, 60);
      delay(50);
    }
    else if (softPotADC <= 204) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 43, 60);
      delay(50);
    }
    else if (softPotADC <= 306) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 45, 60);
      delay(50);
    }
    else if (softPotADC <= 408) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 47, 60);
      delay(50);
    }
    else if (softPotADC <= 510) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 50, 60);
      delay(50);
    }
    else if (softPotADC <= 612) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 53, 60);
      delay(50);
    }
    else if (softPotADC <= 714) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 55, 60);
      delay(50);
    }
    else if (softPotADC <= 816) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 57, 60);
      delay(50);
    }
    else if (softPotADC <= 918) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 60, 60);
      delay(50);
    }
    else if (softPotADC <= 1023) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, 70, 60);
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
