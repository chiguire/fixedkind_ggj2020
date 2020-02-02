#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define GAME_STATE_MAIN (0)

int game_state = GAME_STATE_MAIN;

#define NUM_CELLS (16)
#define CELL_SIZE (8)

#define NONE_NOTE (-1)
#define NOTE_DURATION (7)
char notes_played_len[NUM_CELLS] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
};

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

void setup() {
  Serial.begin(9600);
  matrix.begin();

  matrix.fillScreen(matrix.Color333(0,0,0));
  
  matrix.fillRect(0, 0, 8, 8, 63497);
  delay(100);
  matrix.fillRect(8, 0, 8, 8, 64644);
  delay(100);
  matrix.fillRect(16, 0, 8, 8, 65504);
  delay(100);
  matrix.fillRect(24, 0, 8, 8, 57316);
  delay(100);
  matrix.fillRect(0, 8, 8, 8, 9641);
  delay(100);
  matrix.fillRect(8, 8, 8, 8, 20479);
  delay(100);
  matrix.fillRect(16, 8, 8, 8, 1179);
  delay(100);
  matrix.fillRect(24, 8, 8, 8, 36886);
  delay(100);
  matrix.fillRect(0, 16, 8, 8, 63807);
  delay(100);
  matrix.fillRect(8, 16, 8, 8, 38034);
  delay(100);
  matrix.fillRect(16, 16, 8, 8, 36864);
  delay(100);
  matrix.fillRect(24, 16, 8, 8, 45924);
  delay(100);
  matrix.fillRect(0, 24, 8, 8, 38016);
  delay(100);
  matrix.fillRect(8, 24, 8, 8, 1170);
  delay(100);
  matrix.fillRect(16, 24, 8, 8, 18);
  delay(100);
  matrix.fillRect(24, 24, 8, 8, 65238);
  delay(100);
  
  matrix.fillScreen(matrix.Color333(0,0,0));
  Serial.println("Setup finished");
}

void loop() {
}

byte get_cell_x(int note) {
  switch (note)
  {
  case 0: case 4: case 8: case 12: return 0;
  case 1: case 5: case 9: case 13: return 8;
  case 2: case 6: case 10: case 14: return 16;
  case 3: case 7: case 11: case 15: return 24;
  default: return -1;
  }
}

byte get_cell_y(int note) {
  switch (note)
  {
  case 0: case 1: case 2: case 3: return 0;
  case 4: case 5: case 6: case 7: return 8;
  case 8: case 9: case 10: case 11: return 16;
  case 12: case 13: case 14: case 15: return 24;
  default: return -1;
  }
}

uint16_t get_cell_color(byte note, char duration) {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  
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
    case 10: r=4;g=0;b=0; break;
    case 11: r=5;g=3;b=1; break;
    case 12: r=4;g=4;b=0; break;
    case 13: r=0;g=4;b=4; break;
    case 14: r=0;g=0;b=4; break;
    case 15: r=7;g=6;b=5; break;
  }

  return matrix.Color333(
    (r-(NOTE_DURATION-duration))>0?(r-(NOTE_DURATION-duration)):0,
    (g-(NOTE_DURATION-duration))>0?(g-(NOTE_DURATION-duration)):0,
    (b-(NOTE_DURATION-duration))>0?(b-(NOTE_DURATION-duration)):0
  );
}

void serialEvent() {
}
