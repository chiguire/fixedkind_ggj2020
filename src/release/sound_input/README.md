# sound_input

This sketch controls the neotrellis button array, the scale change buttons, and the music maker shield, which outputs sound either through the speaker or headphones.

The serial port is used to communicate buttons pressed from the neotrellis or scale change buttons, and to receive notes pressed from the director (i.e. when receiving a message).

You can enable or disable the input through serial commands. When you send a hexadecimal digit, the corresponding button is pressed and a note sounds.

The notes belong to the pentatonic scale in 3 octaves. I did a small investigation whether to use major scales or other modes, and concluded that it was best to use the pentatonic scale for aesthetic purposes.

## Requirements

Following instructions from https://learn.adafruit.com/adafruit-music-maker-shield-vs1053-mp3-wav-wave-ogg-vorbis-player/installing-software

For this sketch, you need the Adafruit Music Maker shield for Arduino, and a Adafruit Neotrellis. For this jam, the shield was installed in an Arduino Mega, and the Neotrellis was connected using the remaining free pins, for Vin, Ground, SDA and SCL.

In the Arduino UI, go to Manage Libraries, and install Adafruit_VS1053 library, and the Adafruit seesaw Library.