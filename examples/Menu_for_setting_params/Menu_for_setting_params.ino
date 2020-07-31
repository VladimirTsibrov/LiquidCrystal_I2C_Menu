/* Пример использования меню для ввода параметров
 * с отображением их текущих значений в пунктах меню
 * и сохранением в EEPROM. 
 * Обработка выбранных пунктов осуществляется в обработчиках.
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);
#include <EEPROM.h>

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

uint8_t brightness; // Параметры, которые мы будем именять
uint8_t _delay;

// Объявим перечисление, используемое в качестве ключа пунктов меню
enum {mkBack, mkRoot, mkSetBrightness, mkSetDelay, mkDefaults};

// Прототипы обработчиков пунктов меню:
void SetBrightness();
void SetDelay();
void SetDefaults();

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
sMenuItem menu[] = {
// {ParentKey, Key, Caption, [Handler]} - структура пункта меню
  {mkBack, mkRoot, "Options", NULL},
    {mkRoot, mkSetBrightness, NULL, SetBrightness}, // Названия этих пунктов
    {mkRoot, mkSetDelay, NULL, SetDelay},           // будут сгенерированы позже
    {mkRoot, mkDefaults, "Defaults", SetDefaults},
    {mkRoot, mkBack, "Back", NULL}
};

uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

// Функция поиска индекса пункта по его ключу
int getItemIndexByKey(uint8_t key){
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

// Обработчик для пункта меню Brightness
void SetBrightness(){
  // Запрашиваем новое значение
  brightness = lcd.inputVal<uint8_t>("Input brightness(%)", 0, 100, brightness, 5);
  // Сохраняем его в EEPROM
  EEPROM.update(0, brightness);
  // Обновляем название пункта меню
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  // Далее может распологаться код - реакция на изменение значения Brightness
}

// Обработчик для пункта меню Delay
void SetDelay(){
  // Запрашиваем новое значение
  _delay = lcd.inputVal<uint8_t>("Input delay(ms)", 0, 20, _delay);
  // Сохраняем его в EEPROM
  EEPROM.update(1, _delay);
  // Обновляем название пункта меню
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
  // Далее может распологаться код - реакция на изменение значения Delay
}

// Обработчик для пункта меню Defaults
void SetDefaults(){
  brightness = 50;
  _delay = 10;
  EEPROM.update(0, brightness);
  EEPROM.update(1, _delay);
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
  // Далее может распологаться код - реакция на изменение значений Delay и Brightness
}

// Перерисовка информации на экране
void LCDRepaint(){
  lcd.clear();
  lcd.printfAt(0, 0, "Brightness (%d%%)", brightness);
  lcd.printfAt(0, 1, "Delay (%dms)", _delay);
  lcd.printAt(0, 3, "Press enter for menu");
}

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);

  // Считываем значения brightness и _delay из EEPROM
  brightness = EEPROM.read(0);
  if (brightness > 100) {
    brightness = 50;
    EEPROM.write(0, brightness);
  }
  _delay = EEPROM.read(1);
  if(_delay > 20) {
    _delay = 10;
    EEPROM.write(1, _delay);
  }
  
  // Формируем названия пунктов меню
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
  
  LCDRepaint();
}
  
void loop() {
  // Для изменения параметров необходимо нажать кнопку
  if (lcd.getEncoderState() == eButton) { // При нажатии
    lcd.showMenu(menu, menuLen, 1); // показываем меню
    LCDRepaint(); // после чего обновляем информацию на дисплее
  }
  // Далее может располагаться основной функционал программы
}
