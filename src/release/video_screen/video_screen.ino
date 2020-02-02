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

#define FPS (1000/50)

#define GAME_STATE_MAIN (0)
#define GAME_STATE_CORRECT (1)
#define GAME_STATE_INCORRECT (2)

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

#define CORRECT_COMMAND ('z')
#define INCORRECT_COMMAND ('x')
#define STATE_TIME (18)
byte time_played = 0;
uint16_t color_r_played = 0;
uint16_t color_g_played = 0;
uint16_t color_b_played = 0;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

void setup() {
  Serial.begin(9600);
  matrix.begin();

  matrix.fillScreen(matrix.Color333(0,0,0));
  
  for (int i = 0; i != NUM_CELLS; i++) {
    for (int j = NOTE_DURATION; j != 0; j--) {
      matrix.fillScreen(matrix.Color333(0,0,0));
      matrix.fillRect(get_cell_x(i), get_cell_y(i), CELL_SIZE, CELL_SIZE, get_cell_color(i, j));
      delay(FPS);
    }
  }
  matrix.fillScreen(matrix.Color333(0,0,0));
  // Do not ever uncomment Serial.print* lines or this won't work
  //Serial.println("Setup finished");
  Serial.print('r');
}

void loop() {
  switch (game_state)
  {
  case GAME_STATE_MAIN:
    game_state_main();
    break;
  case GAME_STATE_CORRECT:
    game_state_correct();
    break;
  case GAME_STATE_INCORRECT:
    game_state_incorrect();
    break;
  default:
    break;
  }
}

void game_state_main() {
  matrix.fillScreen(matrix.Color333(0,0,0));
  for (int i = 0; i != NUM_CELLS; i++) {
    if (notes_played_len[i] > 0) {
      //Serial.print(i); Serial.print(" "); Serial.println((int)notes_played_len[i]);
      matrix.fillRect(get_cell_x(i), get_cell_y(i), CELL_SIZE, CELL_SIZE, get_cell_color(i, notes_played_len[i]));
      notes_played_len[i]--;
    }
  }
  delay(FPS);
}

void game_state_correct() {
  for (int i = 0; i != 32; i++)
  {
    float factor = (sin(((time_played+i)/16.0f)*3.14f/50.0f)+1)/2.0f;
    matrix.drawLine(0, i, 31, i, matrix.Color888(
      (uint16_t)(factor*color_r_played),
      (uint16_t)(factor*color_g_played),
      (uint16_t)(factor*color_b_played)
    ));
  }

  time_played--;
  if (time_played == 0)
  {
    game_state = GAME_STATE_MAIN;
  }
  delay(FPS);
}

void game_state_incorrect() {
  for (int i = 0; i != 32; i++)
  {
    float factor = (sin(((time_played+i)/16.0f)*3.14f/50.0f)+1)/2.0f;
    matrix.drawLine(i, 0, i, 31, matrix.Color888(
      (uint16_t)(factor*color_r_played),
      (uint16_t)(factor*color_g_played),
      (uint16_t)(factor*color_b_played)
    ));
  }

  time_played--;
  if (time_played == 0)
  {
    game_state = GAME_STATE_MAIN;
  }
  delay(FPS);
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
    case 10: r=7;g=4;b=0; break;
    case 11: r=5;g=3;b=1; break;
    case 12: r=4;g=4;b=0; break;
    case 13: r=0;g=4;b=4; break;
    case 14: r=0;g=0;b=4; break;
    case 15: r=7;g=6;b=5; break;
  }
  char inv_duration = NOTE_DURATION-duration;

  return matrix.Color333(
    (r-inv_duration)>0?(r-inv_duration):0,
    (g-inv_duration)>0?(g-inv_duration):0,
    (b-inv_duration)>0?(b-inv_duration):0
  );
}

void serialEvent() {
    while (Serial.available() > 0) {
    byte note_played = NONE_NOTE;
    char inChar = (char)Serial.read();
    //Serial.print("Got char "); Serial.println(inChar);
    
    if (game_state != GAME_STATE_MAIN) {
      // ignore input if we're not in the main state
      return;
    }

    if (inChar == '0') { note_played = 0; }
    else if (inChar == '1') { note_played = 1; }
    else if (inChar == '2') { note_played = 2; }
    else if (inChar == '3') { note_played = 3; }
    else if (inChar == '4') { note_played = 4; }
    else if (inChar == '5') { note_played = 5; }
    else if (inChar == '6') { note_played = 6; }
    else if (inChar == '7') { note_played = 7; }
    else if (inChar == '8') { note_played = 8; }
    else if (inChar == '9') { note_played = 9; }
    else if (inChar == 'a') { note_played = 10; }
    else if (inChar == 'b') { note_played = 11; }
    else if (inChar == 'c') { note_played = 12; }
    else if (inChar == 'd') { note_played = 13; }
    else if (inChar == 'e') { note_played = 14; }
    else if (inChar == 'f') { note_played = 15; }

    if (note_played != NONE_NOTE) {
      notes_played_len[note_played] = NOTE_DURATION;
    }

    if (inChar == CORRECT_COMMAND) { 
      game_state = GAME_STATE_CORRECT;
      time_played = STATE_TIME;
      color_r_played = 64;
      color_g_played = 224;
      color_b_played = 64;
    }
    else if (inChar == INCORRECT_COMMAND) {
      game_state = GAME_STATE_INCORRECT;
      time_played = STATE_TIME;
      color_r_played = 224;
      color_g_played = 64;
      color_b_played = 64;
    }
  }
}
