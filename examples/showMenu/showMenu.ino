/* Пример использования функции showMenu для отображения меню. Синтаксис:
 * showMenu(menu, menuLen, showTitle), где
 * menu - массив пунктов меню. Тип элементов - sMenuItem
 * menuLen - количество элементов в меню
 * showTitle - флаг отображения заголовка в меню (название родительского пункта)
 * Функция возвращает ключ выбранного пункта меню
 */
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

// Объявим перечисление, используемое в качестве ключа пунктов меню
enum {mkBack, mkRoot, mkRun, mkOptions, mkMode, mkSpeed, mkLog, mkSelftest, mkHelp, mkFAQ, mkIndex, mkAbout, mkLongCaption};

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
sMenuItem menu[] = {
  {mkBack, mkRoot, "Menu demo"},
    {mkRoot, mkRun, "Run"},
    {mkRoot, mkOptions, "Options"},
      {mkOptions, mkMode, "Mode"},
      {mkOptions, mkSpeed, "Speed"},
      {mkOptions, mkLog, "Print log"},
      {mkOptions, mkSelftest, "Selftest"},
      {mkOptions, mkBack, "Back"},
    {mkRoot, mkHelp, "Help"},
      {mkHelp, mkFAQ, "FAQ"},
      {mkHelp, mkIndex, "Index"},
      {mkHelp, mkAbout, "About"},
      {mkHelp, mkBack, "Back"},
    {mkRoot, mkLongCaption, "Long caption scrolling example"},
    {mkRoot, mkBack, "Exit menu"}
};

uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  // Показываем меню
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 1);
  // И выполняем действия в соответствии с выбранным пунктом
  if (selectedMenuItem == mkRun)
    lcd.print("Run selected");
  else if (selectedMenuItem == mkMode)
    lcd.print("Mode selected");
  else if (selectedMenuItem == mkSpeed)
    lcd.print("Speed selected");
  else if (selectedMenuItem == mkLog)
    lcd.print("Print log selected");
  else if (selectedMenuItem == mkSelftest)
    lcd.print("Selftest selected");
  else if (selectedMenuItem == mkFAQ)
    lcd.print("FAQ selected");
  else if (selectedMenuItem == mkIndex)
    lcd.print("Index selected");
  else if (selectedMenuItem == mkAbout)
    lcd.print("About selected");
  else if (selectedMenuItem == mkLongCaption)
    lcd.print("Scrolling selected");
  else if (selectedMenuItem == mkBack)
    lcd.print("Exit selected");
  while (lcd.getEncoderState() == eNone);
}
