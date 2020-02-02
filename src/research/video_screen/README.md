I had a serious problem, in which I couldn't make video_screen work correctly. Apparently the bug was in that executing any Serial.printX statement would drive the Arduino mad. So I had to remove all print statements so it could work.

These were previous attempts at making it work, which are probably correct except they print to the serial port.