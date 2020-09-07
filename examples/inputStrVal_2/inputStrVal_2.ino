/* Пример использования функции inputStrVal для маскированного ввода. Синтаксис:
 * inputStrVal(title, buffer, len, availSymbols), где
 * title - заголовок
 * buffer - указатель на массив char для вводимых символов
 * len - длина вводимого значения
 * availSymbols - массив символов, доступных для ввода и редактирования
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

char ip[] = "192.168.001.001"; // Массив символов с начальным значением/маской

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  if (lcd.inputStrVal("Input IP", ip, 15, "0123456789")){
    lcd.print("You entered:");
    lcd.printAt(0, 1, ip);
  }
  else
    lcd.print("Input canceled");
  while (lcd.getEncoderState() == eNone);
}
