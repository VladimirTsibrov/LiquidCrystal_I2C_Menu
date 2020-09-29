/* Пример использования функции selectVal для выбора значения. Синтаксис:
 * selectVal(title, list, count, show_selected, preselected), где
 * title - заголовок
 * list - массив значений для выбора
 * count - количество элементов в массиве
 * show_selected - флаг отображения указателя на выбранном элементе
 * preselected - индекс выбранного по умолчанию элемента
 * Функция возвращает индекс выбранного значения
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
  int index;
  String list[] = {"Off", "On"};
  index = lcd.selectVal("Turn backlight", list, 2, false);
  lcd.setBacklight(index);
  lcd.printf("Backlight turned %s", list[index].c_str());
  while (lcd.getEncoderState() == eNone);
}
