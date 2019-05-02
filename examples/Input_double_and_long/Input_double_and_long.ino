#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  // Examples of using input functions. Syntax:
  // inputVal(title, min, max, default, [step = 1])
  // inputValBitwise(title, value, precision, [scale = 0], [signed = 0])
  double x = 0;
  long y = 0;

  x = lcd.inputVal("Input value", 0.0, 10.0, x, 0.25); // input double from 0 to 10 with step 0.25
  x = lcd.inputVal<double>("Input value", 0, 10, x, 0.25); // similar to the previous one, but the type of the variables is specified explicitly - <double>
  lcd.inputValBitwise("Input value", x, 4, 2, 0); // input unsigned value XX.XX
  lcd.inputValBitwise("Input value", x, 4, 2, 1); // input signed value [-]XX.XX

  y = lcd.inputVal <long> ("Input value", 0, 10000, y, 100); // input long value. The type of the variable is specified explicitly to avoid compile errors
  lcd.inputValBitwise("Input value", y, 4); // input 4-digit value (scale and signed by default = 0)
  lcd.inputValBitwise("Input value", y, 4, 0, 1); // input 4-digit signed value
}
