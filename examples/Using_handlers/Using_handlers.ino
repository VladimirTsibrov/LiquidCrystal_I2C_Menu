/* An example of using menu item handlers.
   Handlers allow you to perform actions when
   selecting menu items without leaving the menu.
*/
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

int brightness = 50;
int _delay = 10;

void SetBrightness() {
  brightness = lcd.inputVal("Input brightness(%)", 0, 100, brightness, 5);
}

void SetDelay() {
  _delay = lcd.inputVal("Input delay(ms)", 0, 20, _delay);
}

// Declaring the values used in the menu to define the parent-child relationship
enum {mkBack, mkRoot, mkOptions, mkSetBrightness, mkSetDelay};

// Menu definition
sMenuItem menu[] = {
  {mkBack, mkRoot, "Main menu", NULL},
  {mkRoot, mkOptions, "Options", NULL},
  {mkOptions, mkSetBrightness, "SetBrightness", SetBrightness},
  {mkOptions, mkSetDelay, "SetDelay", SetDelay},
  {mkOptions, mkBack, "Back", NULL},
  {mkRoot, mkBack, "Back", NULL}
};

// Determine the number of items in the menu
uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 1); // Вызываем меню
  /* The reaction for SetBrightness and SetDelay menu items is already
     described in the handlers. Therefore, it remains to describe the
     reaction only for those menu items that do not have handlers:
     if (selectedMenuItem == ...) {...}
  */
}
