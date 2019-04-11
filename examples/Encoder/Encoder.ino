#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

int x = 0;

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  lcd.printfAt(0, 0, "%d ", x);
}

void loop() {
  // Polling the encoder status
  eEncoderState EncoderState = lcd.getEncoderState();
  switch (EncoderState) {
    case eLeft:   // Rotating left
      x--;
      break;
    case eRight:  // Rotating right
      x++;
      break;
    case eButton: // Button pressed
      x = 0;
      break;
    case eNone:   // No action. Exit the function
      return;
  }
  lcd.printfAt(0, 0, "%d ", x); // Print x
}
