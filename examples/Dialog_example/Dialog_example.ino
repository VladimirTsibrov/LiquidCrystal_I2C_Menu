// Пример использования меню для построения диалога
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
  sMenuItem formatConfirmation[] = { // Опишем наше меню:
    {0, 1, "Format drive C?", NULL}, // Это будет заголовок
    {1, 2, "No", NULL}, // и два пункта для выбора
    {1, 3, "Yes", NULL}
  };
  // Показываем меню и анализируем результат
  if (lcd.showMenu(formatConfirmation, 3, 1) == 3)
    lcd.print("Yes selected");
  else
    lcd.print("No selected");
  while (lcd.getEncoderState() == eNone);
}
