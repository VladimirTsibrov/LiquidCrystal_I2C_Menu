/* Пример использования меню для ввода параметров
 * и отображения их текущих значений в пунктах меню.
 * Обработка выбранных пунктов осуществляется при возврате из меню.
 */
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

int brightness = 50;
int _delay = 10;

// Объявим перечисление, используемое в качестве ключа пунктов меню
enum {mkBack, mkRoot, mkSetBrightness, mkSetDelay};

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
sMenuItem menu[] = {
  {mkBack, mkRoot, "Options"},
  {mkRoot, mkSetBrightness, NULL}, // Названия этих пунктов
  {mkRoot, mkSetDelay, NULL}      // сгенерируем позже
};

uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

// Функция поиска индекса пункта по его ключу
int getItemIndexByKey(uint8_t key) {
  for (uint8_t i = 0; i < menuLen; i++)
    if (menu[i].key == key)
      return i;
  return -1;
}

// Функция формирования названия пункта меню, содержащего значение параметра
void updateCaption(uint8_t key, char format[], int value){
  // key - ключ пункта меню, для которого обновляется навание
  // format - шаблон названия со значением
  // value - значение, добавляемое в название
  uint8_t index = getItemIndexByKey(key);
  char* buf = (char*) malloc(40);
  sprintf(buf, format, value);
  menu[index].caption = (char*) realloc(menu[index].caption, strlen(buf)+1);
  strcpy(menu[index].caption, buf);
  free(buf);
}

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  // Формируем названия пунктов меню
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
}

void loop() {
  // Показываем меню
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 1); 
  // и анализируем выбранный элемент
  if (selectedMenuItem == mkSetBrightness) {
    // Выбран пункт Brightness. Запрашиваем новое значение
    brightness = lcd.inputVal("Input brightness(%)", 0, 100, brightness, 5);
    // и обновляем название пункта
    updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  }
  else if (selectedMenuItem == mkSetDelay) {
    // Выбран пункт Delay. Запрашиваем новое значение
    _delay = lcd.inputVal("Input delay(ms)", 0, 20, _delay);
    // и обновляем название пункта
    updateCaption(mkSetDelay, "Delay (%dms)", _delay);
  }
}
