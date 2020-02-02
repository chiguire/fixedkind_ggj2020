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

#define CELL_R (0)
#define CELL_G (1)
#define CELL_B (2)
#define CELL_STARTING_R (3)
#define CELL_STARTING_G (4)
#define CELL_STARTING_B (5)
#define CELL_INFO_LAST (6)

byte main_screen_cell_positions[NUM_CELLS][2] = { {0,0},{8,0},{16,0},{24,0}, {0,8},{8,8},{16,8},{24,8}, {0,16},{8,16},{16,16},{24,16}, {0,24},{8,24},{16,24},{24,24} };
byte main_screen_state[NUM_CELLS][CELL_INFO_LAST];

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

void setup() {

  for (int i = 0; i != NUM_CELLS; i++) {
    main_screen_state[i][CELL_R] = 0;
    main_screen_state[i][CELL_G] = 0;
    main_screen_state[i][CELL_B] = 0;
  }
  main_screen_state[0][CELL_STARTING_R]  = 7; main_screen_state[0][CELL_STARTING_G]  = 0; main_screen_state[0][CELL_STARTING_B]  = 2;
  main_screen_state[1][CELL_STARTING_R]  = 7; main_screen_state[1][CELL_STARTING_G]  = 4; main_screen_state[1][CELL_STARTING_B]  = 1;
  main_screen_state[2][CELL_STARTING_R]  = 7; main_screen_state[2][CELL_STARTING_G]  = 7; main_screen_state[2][CELL_STARTING_B]  = 0;
  main_screen_state[3][CELL_STARTING_R]  = 6; main_screen_state[3][CELL_STARTING_G]  = 7; main_screen_state[3][CELL_STARTING_B]  = 1;
  main_screen_state[4][CELL_STARTING_R]  = 1; main_screen_state[4][CELL_STARTING_G]  = 5; main_screen_state[4][CELL_STARTING_B]  = 2;
  main_screen_state[5][CELL_STARTING_R]  = 2; main_screen_state[5][CELL_STARTING_G]  = 7; main_screen_state[5][CELL_STARTING_B]  = 7;
  main_screen_state[6][CELL_STARTING_R]  = 0; main_screen_state[6][CELL_STARTING_G]  = 4; main_screen_state[6][CELL_STARTING_B]  = 6;
  main_screen_state[7][CELL_STARTING_R]  = 4; main_screen_state[7][CELL_STARTING_G]  = 0; main_screen_state[7][CELL_STARTING_B]  = 5;
  main_screen_state[8][CELL_STARTING_R]  = 7; main_screen_state[8][CELL_STARTING_G]  = 1; main_screen_state[8][CELL_STARTING_B]  = 7;
  main_screen_state[9][CELL_STARTING_R]  = 4; main_screen_state[9][CELL_STARTING_G]  = 4; main_screen_state[9][CELL_STARTING_B]  = 4;
  main_screen_state[10][CELL_STARTING_R] = 4; main_screen_state[10][CELL_STARTING_G] = 0; main_screen_state[10][CELL_STARTING_B] = 0;
  main_screen_state[11][CELL_STARTING_R] = 5; main_screen_state[11][CELL_STARTING_G] = 3; main_screen_state[11][CELL_STARTING_B] = 1;
  main_screen_state[12][CELL_STARTING_R] = 4; main_screen_state[12][CELL_STARTING_G] = 4; main_screen_state[12][CELL_STARTING_B] = 0;
  main_screen_state[13][CELL_STARTING_R] = 0; main_screen_state[13][CELL_STARTING_G] = 4; main_screen_state[13][CELL_STARTING_B] = 4;
  main_screen_state[14][CELL_STARTING_R] = 0; main_screen_state[14][CELL_STARTING_G] = 0; main_screen_state[14][CELL_STARTING_B] = 4;
  main_screen_state[15][CELL_STARTING_R] = 7; main_screen_state[15][CELL_STARTING_G] = 6; main_screen_state[15][CELL_STARTING_B] = 5;
 
  Serial.begin(9600);
  matrix.begin();
  Serial.println("Setup finished");
}

void loop() {
  for (int i = 0; i != NUM_CELLS; i++) {
    matrix.fillRect(
      main_screen_cell_positions[i][0], 
      main_screen_cell_positions[i][1], 
      CELL_SIZE, 
      CELL_SIZE, 
      matrix.Color333(
        main_screen_state[i][CELL_STARTING_R], 
        main_screen_state[i][CELL_STARTING_G], 
        main_screen_state[i][CELL_STARTING_B]
      )
    );
  }
}

void serialEvent() {
  
}
