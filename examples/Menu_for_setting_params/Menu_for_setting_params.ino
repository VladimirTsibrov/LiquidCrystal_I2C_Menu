/* An example of using the menu to set parameters
 * with saving them in the EEPROM, 
 * displaying in menu items
 * and using handlers
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);
#include <EEPROM.h>

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

uint8_t brightness;
uint8_t _delay;

// Declaring the values used in the menu to define the parent-child relationship
enum {mkBack, mkRoot, mkSetBrightness, mkSetDelay, mkDefaults};

// Handlers are implemented after the menu, so their prototypes are needed:
void SetBrightness();
void SetDelay();
void SetDefaults();

// Menu definition
sMenuItem menu[] = {
/*{ParentKey, Key, Caption, [Handler]} */
  {mkBack, mkRoot, "Options", NULL},
    {mkRoot, mkSetBrightness, NULL, SetBrightness}, // Captions of these items
    {mkRoot, mkSetDelay, NULL, SetDelay},           // will be generated later
    {mkRoot, mkDefaults, "Defaults", SetDefaults},
    {mkRoot, mkBack, "Back", NULL}
};

// Determine the number of items in the menu
uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

// This function returns the index of the menu item by its key value
int getItemIndexByKey(uint8_t key){
  for (uint8_t i = 0; i < menuLen; i++)
    if (menu[i].key == key)
      return i;
  return -1;
}

// Function of forming the caption of the menu item with the value
void updateCaption(uint8_t key, char format[], int value){
  // key - the key of the updated menu item
  // format - template for assembling the caption of the menu item
  // value - value added to caption
  uint8_t index = getItemIndexByKey(key);
  char* buf = (char*) malloc(40);
  sprintf(buf, format, value);
  menu[index].caption = (char*) realloc(menu[index].caption, strlen(buf)+1);
  strcpy(menu[index].caption, buf);
  free(buf);
}

// Handler for changing the brightness value
void SetBrightness(){
  brightness = lcd.inputVal<uint8_t>("Input brightness(%)", 0, 100, brightness, 5);
  EEPROM.update(0, brightness);
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
}

// Handler for changing the _delay value
void SetDelay(){
  _delay = lcd.inputVal<uint8_t>("Input delay(ms)", 0, 20, _delay);
  EEPROM.update(1, _delay);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
}

// Handler for setting default values
void SetDefaults(){
  brightness = 50;
  _delay = 10;
  EEPROM.update(0, brightness);
  EEPROM.update(1, _delay);
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
}

// Redraw the information on the screen
void LCDRepaint(){
  lcd.clear();
  lcd.printfAt(0, 0, "Brightness (%d%%)", brightness);
  lcd.printfAt(0, 1, "Delay (%dms)", _delay);
  lcd.printAt(0, 3, "Press btn for menu");
}

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);

  // Get the brightness and _delay values from the EEPROM
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
  
  // Update the menu item captions
  updateCaption(mkSetBrightness, "Brightness (%d%%)", brightness);
  updateCaption(mkSetDelay, "Delay (%dms)", _delay);
  
  LCDRepaint();
}
  
void loop() {
  if (lcd.getEncoderState() == eButton) { // By pressing the button
    lcd.showMenu(menu, menuLen, 1); // show the menu
    LCDRepaint(); // then redraw the screen
  }
  // And the rest of the time we perform some actions.
}
