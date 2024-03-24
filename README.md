# iGuessr
A basic tool to guess iDevice passwords after they have been exploited to remove the passcode limit. Pretty much like a free alternative to the MFC dongle.
## How does this thing work?
Well its simple, we turn your Arduino into a virtual keyboard that will automatically type in codes from 0000 to 9999, there should also be a display connected to the Arduino that will display what codes it is trying and when the photoresistor detects a change in brightness (this usually means the device unlocked) the screen will display the last attempted code which should be the iDevice's lock code.
## Prerequisites
- Exploited iDevice
- Arduino UNO
- 2.4″ LCD Shield (Support for 16x2 lcd is coming soon)
- Photoresistor
- Usb A to B data cable
- Apple lightning to usb camera adapter or the 30pin version
## Build Guide
Let's build shall we?

1. First you need to have the Arduino IDE downloaded. [Link to the Arduino site](https://www.arduino.cc/en/software)
2. In this github you will find a file named iGuessr_sketch.ino, upload this sketch to the ardunio (If you get any errors
it likely means you're missing a few library's like Adafruit_GFX or Adafruit_TFTLCD, just download and retry)
3. Once the sketch is uploaded you need to download Atmel Flip [Link to Atmel Flip page](https://www.microchip.com/en-us/development-tool/flip),
This tool will turn the Arduino into a keyboard and back to stock if you want to reuse it later.
4.
