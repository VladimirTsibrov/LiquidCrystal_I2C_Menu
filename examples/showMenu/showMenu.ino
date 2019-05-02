#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

// Declaring the values used in the menu to define the parent-child relationship
enum {mkBack, mkRoot, mkRun, mkOptions, mkMode, mkSpeed, mkLog, mkSelftest, mkHelp, mkFAQ, mkIndex, mkAbout};

// Menu definition
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
    {mkRoot, mkBack, "Exit menu"}
};

// Determine the number of items in the menu
uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 1);
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
  else if (selectedMenuItem == mkBack)
    lcd.print("Exit selected");
delay(2000);
}
