/* Пример использования функции getEncoderState для работы с энкодером. 
 * Функция вовращает одно из следующих значений:
 * eNone, eLeft, eRight, eButton.
 * Для привязки используемых энкодером пинов вызывается функция attachEncoder.
 */
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

int x = 0;

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  lcd.printfAt(0, 0, "%d ", x);
}

void loop() {
  // Опрашиваем энкодер
  eEncoderState EncoderState = lcd.getEncoderState();
  switch (EncoderState) {
    case eLeft:   // При вращении влево уменьшаем значение переменной
      x--;
      break;
    case eRight:  // При вращении вправо увеличиваем значение переменной
      x++;
      break;
    case eButton: // При нажатии кнопки энкодера обнуляем значение переменной
      x = 0;
      break;
    case eNone:   // Энкодер не вращается, кнопка не нажата. Выходим из функции
      return;
  }
  lcd.printfAt(0, 0, "%d ", x); // Покажем новое значение x
}
