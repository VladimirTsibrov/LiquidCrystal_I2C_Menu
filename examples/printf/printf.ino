/* Пример использования функций вывода на дисплей:
 * printf(format, ...) - форматированный вывод
 * printAt(x, y, value, [param]) - вывод с указанной позиции
 * printfAt(x, y, format, ...) - форматированный вывод с указанной позиции
 * Описание спецификаторов форматированного вывода можно найти по ссылке: 
 * https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaa3b98c0d17b35642c0f3e4649092b9f1
 */
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

void setup() {
  char hello[] = "Hello, world!";
  String s = "String example";
  lcd.begin();
  lcd.printf("millis=%lu", millis());
  lcd.printAt(3, 1, hello);
  lcd.printAt(0, 2, s);
  lcd.printfAt(0, 3, "%s", s.c_str());
}

void loop() {

}
