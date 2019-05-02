#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#define cols 20
#define rows 4
LiquidCrystal_I2C_Menu lcd(0x27, cols, rows);

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
  if (lcd.inputValBitwise("Input value", val, 5, 2, 1)) { // Asking for [-]XXX.XX value
    lcd.print("You entered: ");
    lcd.print(val);
  }
  else
    lcd.print("Input canceled");
  if (rows == 4) {
    lcd.printAt(0, 2, "Press button");
    lcd.printAt(0, 3, "to continue");
  }
  while (lcd.getEncoderState() != eButton);
}
