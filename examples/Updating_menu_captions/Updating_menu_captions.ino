#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

int brightness = 50;
int _delay = 10;

// Declaring the values used in the menu to define the parent-child relationship
enum {mkBack, mkRoot, mkSetBrightness, mkSetDelay};

// Menu definition
sMenuItem menu[] = {
  {mkBack, mkRoot, "Options"},
  {mkRoot, mkSetBrightness, NULL}, // Названия этих пунктов
  {mkRoot, mkSetDelay, NULL}      // сгенерируем позже
};

// Determine the number of items in the menu
uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

// This function returns the index of the menu item by its key value
int getItemIndexByKey(uint8_t key) {
  for (uint8_t i = 0; i < menuLen; i++)
    if (menu[i].key == key)
      return i;
  return -1;
}

// Function of forming the caption of the menu item with the value
void updateCaption(uint8_t key, const char format[], int value) {
  // key - the key of the updated menu item
  // format - template for assembling the caption of the menu item
  // value - value added to caption
  uint8_t index = getItemIndexByKey(key);
  char* buf = (char*) malloc(40);
  sprintf(buf, format, value);
  menu[index].caption = (char*) realloc(menu[index].caption, strlen(buf) + 1);
  strcpy(menu[index].caption, buf);
  free(buf);
}

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
}

void loop() {
  // Showing the menu
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 1);
  // and analyzing the selected item
  if (selectedMenuItem == mkSetBrightness) {
    // Changing the brightness value
    brightness = lcd.inputVal("Input brightness(%)", 0, 100, brightness, 5);
    updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  }
  else if (selectedMenuItem == mkSetDelay) {
    // Changing the _delay value
    _delay = lcd.inputVal("Input delay(ms)", 0, 20, _delay);
    updateCaption(mkSetDelay, "Delay (%dms)", _delay);
  }
}
