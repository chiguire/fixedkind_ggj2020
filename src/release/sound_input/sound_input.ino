#include "Adafruit_NeoTrellis.h"

Adafruit_NeoTrellis trellis;

#include <SoftwareSerial.h>

// define the pins used
#define VS1053_RX  2 // This is the pin that connects to the RX pin on VS1053

#define VS1053_RESET 9 // This is the pin that connects to the RESET pin on VS1053
// If you have the Music Maker shield, you don't need to connect the RESET pin!

// If you're using the VS1053 breakout:
// Don't forget to connect the GPIO #0 to GROUND and GPIO #1 pin to 3.3V
// If you're using the Music Maker shield:
// Don't forget to connect the GPIO #1 pin to 3.3V and the RX pin to digital #2

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
#define VS1053_GM1_OCARINA 82

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

#define FPS (1000/50)

#define NOTE_BEGIN 36
#define NOTES_LEN 16
int notes[] = { 0,   2,  4,  7,
                9,  12, 14, 16, 
                19, 21, 24, 26,
                28, 31, 33, 36, };

bool pressed[] = { 
  false, false, false, false, 
  false, false, false, false, 
  false, false, false, false, 
  false, false, false, false, 
};

bool prev_pressed[] = { 
  false, false, false, false, 
  false, false, false, false, 
  false, false, false, false, 
  false, false, false, false, 
};

bool just_pressed(int key) {
  return pressed[key] && !prev_pressed[key];
}

#define NOTE_DURATION (15)

int note_duration[] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
};

SoftwareSerial VS1053_MIDI(0, 2); // TX only, do not use the 'rx' side
// on a Mega/Leonardo you may have to change the pin to one that 
// software serial support uses OR use a hardware serial port!

//define a callback for key presses
TrellisCallback blink(keyEvent evt){
  // Check is the pad pressed?
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    pressed[evt.bit.NUM] = true;
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
  // or is the pad released?
    pressed[evt.bit.NUM] = false;
  }

  return 0;
}

void setup() {
  Serial.begin(9600);
  
  VS1053_MIDI.begin(31250); // MIDI uses a 'strange baud rate'
  
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
  delay(10);
  
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetInstrument(0, VS1053_GM1_OCARINA);
  midiSetChannelVolume(0, 127);

  if (!trellis.begin()) {
    Serial.println("Could not start trellis, check wiring?");
    while(1);
  } else {
    Serial.println("NeoPixel Trellis started");
  }

  //activate all keys and set callbacks
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, blink);
  }
  //do a little animation to show we're on
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(20);
  }
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, 0x000000);
    trellis.pixels.show();
    delay(20);
  }
}

void loop() {
  trellis.read();  // interrupt management does all the work! :)

  for (int i = 0; i != NOTES_LEN; i++) {
    if (just_pressed(i)) {
      note_duration[i] = NOTE_DURATION;
      midiNoteOn(0, NOTE_BEGIN+notes[i], 127);
      trellis.pixels.setPixelColor(i, get_cell_color(i, (NOTE_DURATION/2))); //on rising
      Serial.print(int_to_hex(i));
    }
  }

  trellis.pixels.show();
  
  for (int i = 0; i != NOTES_LEN; i++) {
    prev_pressed[i] = pressed[i];
  }

  delay(FPS);

  for (int i = 0; i != NOTES_LEN; i++) {
    if (note_duration[i] > 0) {
      note_duration[i]--;

      trellis.pixels.setPixelColor(i, get_cell_color(i, (note_duration[i]/2))); //on rising

      if (note_duration[i] == 0) {
        midiNoteOff(0, NOTE_BEGIN+notes[i], 127);
        trellis.pixels.setPixelColor(i, 0); //off falling
      }
    }
  }

}


void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);  
  VS1053_MIDI.write(inst);
}


void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write(MIDI_CHAN_VOLUME);
  VS1053_MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
  VS1053_MIDI.write(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_ON | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

int get_cell_color(byte note, char duration) {
  int r;
  int g;
  int b;
  
  switch (note) {
    case 0: r=7;g=0;b=2; break;
    case 1: r=7;g=4;b=1; break;
    case 2: r=7;g=7;b=0; break;
    case 3: r=6;g=7;b=1; break;
    case 4: r=1;g=5;b=2; break;
    case 5: r=2;g=7;b=7; break;
    case 6: r=0;g=4;b=6; break;
    case 7: r=4;g=0;b=5; break;
    case 8: r=7;g=1;b=7; break;
    case 9: r=4;g=4;b=4; break;
    case 10: r=7;g=4;b=0; break;
    case 11: r=5;g=3;b=1; break;
    case 12: r=4;g=4;b=0; break;
    case 13: r=0;g=4;b=4; break;
    case 14: r=0;g=0;b=4; break;
    case 15: r=7;g=6;b=5; break;
  }
  char inv_duration = (NOTE_DURATION/2)-duration;

  return trellis.pixels.Color(
    (r-inv_duration)>0?(r-inv_duration)*(256/8):0,
    (g-inv_duration)>0?(g-inv_duration)*(256/8):0,
    (b-inv_duration)>0?(b-inv_duration)*(256/8):0
  );
}

/******************************************/

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

char int_to_hex(int i) {
  switch (i)
  {
  case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: return '0' + i;
  case 10: case 11: case 12: case 13: case 14: case 15: return 'a' + (i-10);
  default: 'x';
  }
}