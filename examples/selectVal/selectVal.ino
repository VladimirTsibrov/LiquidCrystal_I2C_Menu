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
  int index;
  String list[] = {"Off", "On"};
  index = lcd.selectVal("Turn backlight", list, 2, lcd.getBacklight());
  lcd.setBacklight(index);
  lcd.printf("Backlight turned %s", list[index].c_str());
  delay(2000);
}
