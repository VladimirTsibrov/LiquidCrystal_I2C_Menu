/* Пример использования функции printMultiline
 */
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4


void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  lcd.printMultiline("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");
}
