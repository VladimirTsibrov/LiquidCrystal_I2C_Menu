/* Пример использования функции printMultiline, а также
 * способы размещения длинных строк в памяти программ
 */
#include <avr/pgmspace.h>
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

// Объявим две строки в памяти программ. Так они не будут занимать оперативную память
const char text_1[] PROGMEM = "Using PROGMEM example";
const char text_2[] PROGMEM = "This text is stored in FLASH";

const char* const text[] PROGMEM = {text_1, text_2};

void setup() {
  lcd.begin();
    lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  char *buffer;
  buffer = (char*) malloc(30); // Буфер для временного хранения строки
  strcpy_P(buffer, (char*)pgm_read_word(&(text[0]))); // Копируем строку в буфер
  lcd.printMultiline(buffer); // Выводим содержимое буфера на экран
  strcpy_P(buffer, (char*)pgm_read_word(&(text[1]))); // Аналогично со второй строкой
  lcd.printMultiline(buffer);
  free(buffer); // Освобождаем буфер
  
  // Другой пример хранения строк в памяти программ - использование макроса F().
  lcd.printMultiline(F("Using F() macro example. Press button to continue."));
}
