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
  // An example of using the menu for user dialogue
  sMenuItem formatConfirmation[] = {{0, 1, "Format drive C?", NULL}, {1, 2, "No", NULL}, {1, 3, "Yes", NULL}};
  if (lcd.showMenu(formatConfirmation, 3, 1) == 3)
    lcd.print("Yes selected");
  else
    lcd.print("No selected");
  delay(2000);
}
