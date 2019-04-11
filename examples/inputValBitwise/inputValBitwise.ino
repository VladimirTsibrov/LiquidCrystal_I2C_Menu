#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

double val = 0;

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  if (lcd.inputValBitwise("Input value", val, 5, 2, 1)) // Asking for [-]XXX.XX value
    lcd.printf("You entered %Lf", val);
  else
    lcd.print("Input canceled");
  lcd.printAt(0, 2, "Press button");
  lcd.printAt(0, 3, "to continue");
  while (lcd.getEncoderState() != eButton);
}
