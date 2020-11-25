/* Пример использования функции onChange при вызове inputVal. Синтаксис:
 * inputVal(title, min, max, default, [step = 1], [*onChangeFunc = NULL]), где 
 * title - заголовок
 * min и max - минимальное и максимальное значения для задания диапазона ввода
 * default - начальное значение
 * step - шаг приращения, по умолчанию = 1
 * *onChangeFunc - указатель на void функцию, которая будет вызываться при изменении значения
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

int v = 0;
long l = 0;
  
void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

// Функция, которая будет вызываться при изменении переменной v
void volumeOnChange(int newValue) {
  // Тип переменной newValue должен совпадать с типом переменной v - int
  // Функция может использоваться для применения нового значения v непосредственно при редактировании,
  // а не при завершении редактирования.
  // Кроме того можно добавить свои эффекты на экран редактирования. Например, шкалу:
  for (uint8_t i = 0; i < 15; i++) {
    if (i < newValue)
      lcd.printAt(3 + i, 1, "\xFF");
    else
      lcd.printAt(3 + i, 1, "\xDB");
  }
}

// Функция, которая будет вызываться при изменении переменной l
void longOnChange(long newValue) {
  // Тип переменной newValue должен совпадать с типом переменной l - long
  // Пример вывода шкалы для переменной, диапазон которой не совпадает с размером шкалы.
  // Решение - привести newValue к нужному диапазону функцией map
  long pos = map(newValue, 0, 1000, 0, 15); // в шкале 15 делений
  for (uint8_t i = 0; i < 15; i++) {
    if (i < pos)
      lcd.printAt(4 + i, 1, "\xFF");
    else
      lcd.printAt(4 + i, 1, "\xDB");
  }
}

void loop() {
  v = lcd.inputVal("Input volume", 0, 15, v, 1, volumeOnChange); 
  l = lcd.inputVal <long> ("Input value", 0, 1000, l, 10, longOnChange);
}
