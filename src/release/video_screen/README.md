# video_screen

This Arduino sketch is in charge of displaying the notes/colors on a LED screen. When the sketch begins, a small test is done and you should see all colors displaying in sequence. After this, the sketch sends an 'r' character on the serial port to signal the director the screen is ready.

The screen has 3 states: main, correct and incorrect. During the main state, the sketch reads the serial port for a hexadecimal character ([0-9a-f]), and plays the corresponding cell on-screen. The correct and incorrect states show a green and a red screen for a short time, and then come back to the main state. The director sends a correct command ('z') or incorrect command ('x') through the serial port.

The screen is divided in 16 cells, 4 by 4, and each cell has a starting color. When a note is played, the color is set to that cell, and then each individual component (R,G,B) is decreased to 0 until black.

## Obstacles

An important obstacle in developing this sketch was running into problems when using println statements through the serial port. I used this to debug the sketch, but after adding a few arrays, the sketch stopped and printed weird characters on the serial port. A small number of cells (4 or 8) and the sketch would work. A larger number of cells (12 or more) and the sketch would stop working.

It was eventually found that removing the println statements would make the sketch work. In conclusion, when working with a device with minimal memory, such as the Arduino Uno, the serial port must be used with great care.

## Requirements

Following instructions from https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/test-example-code

For this sketch, you need a 32x32 RGB LED Matrix Panel, and an Arduino Uno. You can also use the Adafruit RGB Matrix Shield to make your life easier, or connect it directly to the pins. Follow the instructions from the URL above.

In the Arduino UI, go to Manage Libraries, and install RGB Matrix Panel library, and Adafruit GFX Library