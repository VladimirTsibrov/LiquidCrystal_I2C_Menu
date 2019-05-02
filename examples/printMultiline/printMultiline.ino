#include <avr/pgmspace.h>
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

const char text_1[] PROGMEM = "Using PROGMEM example";
const char text_2[] PROGMEM = "This text is stored in FLASH";

const char* const text[] PROGMEM = {text_1, text_2};

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  char *buffer;
  lcd.printMultiline("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");
  buffer = (char*) malloc(30); // Allocate memory to read a string from program memory.
  strcpy_P(buffer, (char*)pgm_read_word(&(text[0]))); // Read a string from program memory
  lcd.printMultiline(buffer); // Print buffer contents
  strcpy_P(buffer, (char*)pgm_read_word(&(text[1])));
  lcd.printMultiline(buffer);
  free(buffer);
  lcd.printMultiline(F("Using F() macro example. Press button to continue."));
}
