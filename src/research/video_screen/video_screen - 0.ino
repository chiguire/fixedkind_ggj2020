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

# define CELL_HUE (0)
# define CELL_SATURATION (1)
# define CELL_VALUE (2)
# define CELL_STARTING_HUE (0)

#define FPS (1000/60)
#define NUM_CELLS (16)
#define SIDE_CELLS (4)
#define CELL_SIZE (8)
#define SLIT_SIZE (CELL_SIZE/2)

#define NONE_SCREEN (0)
#define MAIN_SCREEN (1)
#define FIX_SCREEN (2)
#define GOOD_SCREEN (3)

#define NONE_NOTE (0)

byte cell_position[NUM_CELLS][2];
long main_screen_state[NUM_CELLS][4];
int game_state = MAIN_SCREEN;
byte inputbuffer[2];

int screen_requested = NONE_SCREEN;
int note_played = NONE_NOTE;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i != SIDE_CELLS; i++) {
    for (int j = 0; j != SIDE_CELLS; j++) {
      Serial.println(i*4+j);
      cell_position[i*4+j][0] = 8*i;
      cell_position[i*4+j][1] = 8*j;
      main_screen_state[i*4+j][CELL_HUE] = 0;
      main_screen_state[i*4+j][CELL_SATURATION] = 255;
      main_screen_state[i*4+j][CELL_VALUE] = 0;
      Serial.println(i*4+j);
    }
  }
  // These hues are calculated from hue = (int)((atan2(-dy, dx) + PI) * 1536.0 / (PI * 2.0))
  // This comes from the colorwheel_32x32 example
  main_screen_state[0][CELL_STARTING_HUE] = 3072;
  main_screen_state[1][CELL_STARTING_HUE] = 3362;
  main_screen_state[2][CELL_STARTING_HUE] = 3652;
  main_screen_state[3][CELL_STARTING_HUE] = 3942;
  main_screen_state[4][CELL_STARTING_HUE] = 4233;
  main_screen_state[5][CELL_STARTING_HUE] = 4523;
  main_screen_state[6][CELL_STARTING_HUE] = 4813;
  main_screen_state[7][CELL_STARTING_HUE] = 5103;
  main_screen_state[8][CELL_STARTING_HUE] = 5393;
  main_screen_state[9][CELL_STARTING_HUE] = 5683;
  main_screen_state[10][CELL_STARTING_HUE] = 5973;
  main_screen_state[11][CELL_STARTING_HUE] = 6263;
  main_screen_state[12][CELL_STARTING_HUE] = 6554;
  main_screen_state[13][CELL_STARTING_HUE] = 6844;
  main_screen_state[14][CELL_STARTING_HUE] = 7134;
  main_screen_state[15][CELL_STARTING_HUE] = 7424;
  inputbuffer[0] = 0;
  inputbuffer[1] = 0;
  
  matrix.begin();
  
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(7, 0, 0));

  Serial.println("Finished setup");
}

void loop() {
  switch (game_state) {
    case MAIN_SCREEN: main_screen(); break;
  }
}

void serialEvent()
{
  while (Serial.available() > 0) {
    
    char inChar = (char)Serial.read();
    Serial.print("Got char "); Serial.println(inChar);
    
    if (inChar == '0') { note_played = 1; }
    else if (inChar == '1') { note_played = 2; }
    else if (inChar == '2') { note_played = 3; }
    else if (inChar == '3') { note_played = 4; }
    else if (inChar == '4') { note_played = 5; }
    else if (inChar == '5') { note_played = 6; }
    else if (inChar == '6') { note_played = 7; }
    else if (inChar == '7') { note_played = 8; }
    else if (inChar == '8') { note_played = 9; }
    else if (inChar == '9') { note_played = 10; }
    else if (inChar == 'a') { note_played = 11; }
    else if (inChar == 'b') { note_played = 12; }
    else if (inChar == 'c') { note_played = 13; }
    else if (inChar == 'd') { note_played = 14; }
    else if (inChar == 'e') { note_played = 15; }
  }
  /*
  while (Serial.available() >= 2) {
    Serial.readBytes(inputbuffer, 2);
    screen_requested = inputbuffer[0];
    note_played = inputbuffer[1];
    Serial.print("Screen requested: "); Serial.print(screen_requested); Serial.print(" / Note played: "); Serial.println(note_played);
  }*/
}

void main_screen() {

  if (note_played != NONE_NOTE) {
    main_screen_state[(note_played - 1)][CELL_HUE] = main_screen_state[(note_played-1)][CELL_STARTING_HUE];
    main_screen_state[(note_played - 1)][CELL_VALUE] = 255;
    note_played = NONE_NOTE;
  }
  
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  for (int i = 0; i != NUM_CELLS; i++) {
    matrix.fillRect(
      cell_position[i][0], 
      cell_position[i][1], 
      CELL_SIZE, 
      CELL_SIZE, 
      matrix.ColorHSV(
        main_screen_state[i][CELL_HUE], 
        main_screen_state[i][CELL_SATURATION], 
        main_screen_state[i][CELL_VALUE], 
        true
      )
    );

    // update cell state
    if (main_screen_state[i][CELL_VALUE] > 0) {
      main_screen_state[i][CELL_VALUE] = (int)(main_screen_state[i][CELL_VALUE]*0.5f);
    }
  }
  
  delay(100);
}
