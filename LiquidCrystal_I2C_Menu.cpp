#include "LiquidCrystal_I2C_Menu.h"
#include <inttypes.h>
#include <Arduino.h>
#include <Wire.h>

uint8_t scrollUp[8]  = {0x4, 0xa, 0x11, 0x1f};
uint8_t scrollDown[8]  = {0x0, 0x0, 0x0, 0x0, 0x1f, 0x11, 0xa, 0x4};

uint8_t iconOk[8]  = {0x0, 0x1, 0x1, 0x2, 0x12, 0xc, 0x4};
uint8_t iconCancel[8]  = {0x0, 0x0, 0x11, 0xa, 0x4, 0xa, 0x11};


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

LiquidCrystal_I2C_Menu::LiquidCrystal_I2C_Menu(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
  _addr = lcd_addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _charsize = charsize;
  _backlightval = LCD_BACKLIGHT;
}

void LiquidCrystal_I2C_Menu::begin() {
  Wire.begin();
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

  if (_rows > 1) {
    _displayfunction |= LCD_2LINE;
  }

  // for some 1 line displays you can select a 10 pixel high font
  if ((_charsize != 0) && (_rows == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delay(50);

  // Now we pull both RS and R/W low to begin commands
  expanderWrite(_backlightval); // reset expanderand turn backlight off (Bit 8 =1)
  delay(1000);

  //put the LCD into 4 bit mode
  // this is according to the hitachi HD44780 datasheet
  // figure 24, pg 46

  // we start in 8bit mode, try to set 4 bit mode
  write4bits(0x03 << 4);
  delayMicroseconds(4500); // wait min 4.1ms

  // second try
  write4bits(0x03 << 4);
  delayMicroseconds(4500); // wait min 4.1ms

  // third go!
  write4bits(0x03 << 4);
  delayMicroseconds(150);

  // finally, set to 4-bit interface
  write4bits(0x02 << 4);

  // set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for roman languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

  home();
}

/********** high level commands, for the user! */
void LiquidCrystal_I2C_Menu::clear() {
  command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_Menu::home() {
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_Menu::setCursor(uint8_t col, uint8_t row) {
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if (row > _rows) {
    row = _rows - 1;  // we count rows starting w/0
  }
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_I2C_Menu::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_Menu::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C_Menu::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_Menu::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_I2C_Menu::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_Menu::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C_Menu::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_I2C_Menu::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_I2C_Menu::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C_Menu::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_I2C_Menu::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_I2C_Menu::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_I2C_Menu::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(charmap[i]);
  }
}

// Turn the (optional) backlight off/on
void LiquidCrystal_I2C_Menu::noBacklight(void) {
  _backlightval = LCD_NOBACKLIGHT;
  expanderWrite(0);
}

void LiquidCrystal_I2C_Menu::backlight(void) {
  _backlightval = LCD_BACKLIGHT;
  expanderWrite(0);
}
bool LiquidCrystal_I2C_Menu::getBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}

/*********** extended functions ***********/

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, const String &s) {
  setCursor(col, row);
  print(s);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, const char str[]) {
  setCursor(col, row);
  print(str);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, char c) {
  setCursor(col, row);
  print(c);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, unsigned char b, int base) {
  setCursor(col, row);
  print(b, base);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, int n, int base) {
  setCursor(col, row);
  print(n, base);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, unsigned int n, int base) {
  setCursor(col, row);
  print(n, base);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, long n, int base) {
  setCursor(col, row);
  print(n, base);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, unsigned long n, int base) {
  setCursor(col, row);
  print(n, base);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, double d, int digits) {
  setCursor(col, row);
  print(d, digits);
}

void LiquidCrystal_I2C_Menu::printAt(uint8_t col, uint8_t row, const Printable& x) {
  setCursor(col, row);
  print(x);
}

void LiquidCrystal_I2C_Menu::printf(const char * format, ...) {
  char buf[80];
  va_list args;
  va_start (args, format);
  vsnprintf(buf, 80, format, args);
  va_end (args);
  print(buf);
}

void LiquidCrystal_I2C_Menu::printfAt(uint8_t col, uint8_t row, const char * format, ...) {
  char buf[80];
  va_list args;
  va_start (args, format);
  vsnprintf(buf, 80, format, args);
  va_end (args);
  setCursor(col, row);
  print(buf);
}

void LiquidCrystal_I2C_Menu::attachEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinBtn) {
  _pinA = pinA;
  _pinB = pinB;
  _pinBtn = pinBtn;
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);
  pinMode(_pinBtn, INPUT_PULLUP);
  _prevPoolTime = 0;
}

eEncoderState LiquidCrystal_I2C_Menu::getEncoderState() {
  bool encoderA, encoderB;
  eEncoderState Result = eNone;
  if (millis() - _prevPoolTime > ENCODER_POOL_DELAY) {
    _prevPoolTime = millis();
    if (digitalRead(_pinBtn) == LOW ) {
      if (_buttonPrev) {
        _buttonPrev = 0;
        Result = eButton;
      }
    }
    else {
      _buttonPrev = 1;
      encoderA = digitalRead(_pinA);
      encoderB = digitalRead(_pinB);
      if ((!encoderA) && (_APrev)) {
        if (encoderB) Result = eRight;
        else          Result = eLeft;
      }
      _APrev = encoderA;
    }
  }
  return Result;
}

bool  LiquidCrystal_I2C_Menu::printTitle(const char title[]) {
  uint8_t l = strlen(title);
  char * buffer;
  clear();
  if (l > 0) {
    l = min(l, _cols);
    buffer = (char*) malloc(l + 1);
    memcpy(buffer, title, l);
    buffer[l] = '\0';
    printAt(0, 0, buffer);
    free(buffer);
    return 1;
  }
  else return 0;
}

void LiquidCrystal_I2C_Menu::printMultiline(const String &s) {
  printMultiline(s.c_str());
}

void LiquidCrystal_I2C_Menu::printMultiline(const char str[]) {
  uint8_t offset = 0;
  uint8_t lineLength = _cols - 1;
  bool needRepaint = 1;
  char buf[_cols];
  eEncoderState encoderState = eNone;
  createChar(0, scrollUp);
  createChar(1, scrollDown);
  while (encoderState != eButton) {
    if (needRepaint) {
      needRepaint = 0;
      clear();
      // print text
      for (uint8_t i = 0; i < min(_rows, (strlen(str) + lineLength - 1) / lineLength); i++) {
        memcpy(buf, &str[offset + i * lineLength], lineLength);
        buf[lineLength] = '\0';
        printAt(0, i, buf);
      }
      // show scrollbar
      if (offset > 0) {
        setCursor(lineLength, 0);
        write(0); // arrow up
      }
      if (strlen(str) > (unsigned)(offset + _rows * lineLength)) {
        setCursor(lineLength, _rows - 1);
        write(1); // arrow down
      }
    }

    encoderState = getEncoderState();
    if (encoderState == eNone) {
      encoderIdle(); // no action
    }
    else if (encoderState == eLeft) {
      // scroll up
      if (offset > 0) {
        offset -= lineLength;
        needRepaint = 1;
      }
    }
    else if (encoderState == eRight) {
      // scroll down
      if (strlen(str) > (unsigned)(offset + _rows * lineLength)) {
        offset += lineLength;
        needRepaint = 1;
      }
    }
  }
  clear();
}

bool LiquidCrystal_I2C_Menu::isEditable(char ch, const char availSymbols[]) {
  if (ch == '\0') return 1;
  byte l = strlen(availSymbols);
  for (byte i = 0; i < l; i++) {
    if (availSymbols[i] == ch) return 1;
  }
  return 0;
}

bool LiquidCrystal_I2C_Menu::getNextEditable(char S[], uint8_t lenS, const char availSymbols[], uint8_t &currentPos, bool direction) {
  int i;
  if (currentPos == 254) i = lenS;
  else i = currentPos;
  while (1) {
    if (direction) i++; else i--;
    if (i < 0) return 0;
    if (i == lenS) {
      currentPos = 254;
      return 0;
    }
    if (isEditable(S[i], availSymbols)) {
      currentPos = i;
      return 1;
    }
  }
}

bool LiquidCrystal_I2C_Menu::_inputStrVal(const char title[], char buffer[], uint8_t len, const char availSymbols[], bool _signed) {
  eEncoderState encoderState = eNone;
  uint8_t pos = 0;
  uint8_t l = len;
  if (l > _cols - 4) l = _cols - 4;
  char tmpBuffer[_cols + 1];
  uint8_t hasTitle = printTitle(title);

  memcpy(tmpBuffer, buffer, l);
  tmpBuffer[l] = '\0';
  printAt(0, hasTitle, tmpBuffer);

  createChar(0, iconOk);
  createChar(1, iconCancel);
  setCursor(_cols - 3, hasTitle); // Ok
  write(0);
  setCursor(_cols - 1, hasTitle); // Cancel
  write(1);

  // Jump to the first editable symbol
  if (!_signed & (!isEditable(tmpBuffer[pos], availSymbols))) {
    if (!getNextEditable(tmpBuffer, len, availSymbols, pos, 1))
      setCursor(_cols - 3, hasTitle);
    else
      setCursor(pos, hasTitle);
  }
  else setCursor(pos, hasTitle);

  cursor();

  // Main loop - select the symbol for editing or OK/Cancel
  while (1) {
    encoderState = getEncoderState();
    switch (encoderState) {
      case eNone: {
          encoderIdle();
          continue;
        }
      case eLeft: { // Moving left
          if (pos == 0) continue; // Cant move left any more
          if ((pos == 1) & (_signed)) {
            setCursor(--pos, hasTitle);
            continue;
          }
          if (pos == 255) { // Cancel selected, jump to OK
            setCursor(_cols - 3, hasTitle);
            pos--;
            continue;
          }
          if (getNextEditable(tmpBuffer, len, availSymbols, pos, 0))
            setCursor(pos, hasTitle);
          continue;
        }
      case eRight: { // Moving right
          if (pos == 255) continue; // Cant move right any more
          if (pos == 254) { // Ok selected, jump to Cancel
            setCursor(_cols - 1, hasTitle);
            pos++;
            continue;
          }
          if (pos == l - 1) { // Last symbol selected, jump to OK
            setCursor(_cols - 3, hasTitle);
            pos = 254;
            continue;
          }
          if (getNextEditable(tmpBuffer, len, availSymbols, pos, 1))
            setCursor(pos, hasTitle);
          else {
            pos = 254;
            setCursor(_cols - 3, hasTitle);
          }
          continue;
        }
      case eButton: { // Button pressed
          if (pos == 255) {
            noCursor();
            clear();
            return 0; // Cancel
          }
          if (pos == 254) { // OK
            noCursor();
            clear();
            memcpy(buffer, tmpBuffer, l);
            return 1;
          }
          // Editing the selected symbol
          encoderState = eNone;
          //setCursor(pos, hasTitle);
          blink();
          while (encoderState != eButton)
          {
            encoderState = getEncoderState();
            switch (encoderState) {
              case eNone: {
                  encoderIdle();
                  continue;
                }
              case eLeft: {
                  if ((_signed) & (pos == 0)) {
                    if (!getNextSymbol(tmpBuffer[pos], 0, NUMERIC_SIGNS, 1)) continue;
                  }
                  else {
                    if (!getNextSymbol(tmpBuffer[pos], 0, availSymbols)) continue;
                  }
                  printAt(pos, hasTitle, tmpBuffer[pos]);
                  setCursor(pos, hasTitle);
                  continue;
                }
              case eRight: {
                  if ((_signed) & (pos == 0)) {
                    if (!getNextSymbol(tmpBuffer[pos], 1, NUMERIC_SIGNS, 1)) continue;
                  }
                  else {
                    if (!getNextSymbol(tmpBuffer[pos], 1, availSymbols)) continue;
                  }
                  printAt(pos, hasTitle, tmpBuffer[pos]);
                  setCursor(pos, hasTitle);
                  continue;
                }
              case eButton: break;
            }
          }
          noBlink();
          continue;
        }
    }
  }
}

bool LiquidCrystal_I2C_Menu::getNextSymbol(char &ch, bool direction, const char availSymbols[], bool looped) {
  if (ch == '\0') {
    ch = availSymbols[0];
    return 1;
  }
  for (uint8_t i = 0; i < strlen(availSymbols); i++) {
    if (availSymbols[i] == ch) {
      if (direction) {
        if (++i < strlen(availSymbols)) ch = availSymbols[i];
        else if (looped) ch = availSymbols[0];
        return 1;
      }
      else {
        if (i > 0) ch = availSymbols[--i];
        else if (looped) ch = availSymbols[strlen(availSymbols) - 1];
        return 1;
      }
      return 0;
    }
  }
  return 0;
}

bool LiquidCrystal_I2C_Menu::inputStrVal(const String &title, char buffer[], uint8_t len, const char availSymbols[]) {
  return _inputStrVal(title.c_str(), buffer, len, availSymbols, 0);
}

bool LiquidCrystal_I2C_Menu::inputStrVal(const char title[], char buffer[], uint8_t len, const char availSymbols[]) {
  return _inputStrVal(title, buffer, len, availSymbols, 0);
}

uint8_t LiquidCrystal_I2C_Menu::selectVal(const String &title, const char **list, uint8_t count, uint8_t preselected, bool show_selected) {
  return _selectVal(title.c_str(), list, count, preselected, show_selected);
}

uint8_t LiquidCrystal_I2C_Menu::selectVal(const char title[], const char **list, uint8_t count, uint8_t preselected, bool show_selected) {
  return _selectVal(title, list, count, preselected, show_selected);
}

uint8_t LiquidCrystal_I2C_Menu::selectVal(const String &title, String list[], uint8_t count, uint8_t preselected, bool show_selected) {
  return _selectVal(title.c_str(), list, count, preselected, show_selected);
}

uint8_t LiquidCrystal_I2C_Menu::selectVal(const char title[], String list[], uint8_t count, uint8_t preselected, bool show_selected) {
  return _selectVal(title, list, count, preselected, show_selected);
}

uint8_t LiquidCrystal_I2C_Menu::selectVal(const String &title, int list[], uint8_t count, uint8_t preselected, bool show_selected) {
  return _selectVal(title.c_str(), list, count, preselected, show_selected);
}

uint8_t LiquidCrystal_I2C_Menu::selectVal(const char title[], int list[], uint8_t count, uint8_t preselected, bool show_selected) {
  return _selectVal(title, list, count, preselected, show_selected);
}

void LiquidCrystal_I2C_Menu::_prepareForPrint(char buffer[], char *value, uint8_t len) {
  char format[12] = {'\0'};
  sprintf(format, "%%-%d.%ds", len, len);
  sprintf(buffer, format, value);
}

void LiquidCrystal_I2C_Menu::_prepareForPrint(char buffer[], int value, uint8_t len) {
  char format[8] = {'\0'};
  sprintf(format, "%%-%dd", len);
  sprintf(buffer, format, value);
}

void LiquidCrystal_I2C_Menu::_prepareForPrint(char buffer[], String value, uint8_t len) {
  char format[12] = {'\0'};
  sprintf(format, "%%-%d.%ds", len, len);
  sprintf(buffer, format, value.c_str());
}

template <typename T>
uint8_t LiquidCrystal_I2C_Menu::_selectVal(const char title[], T list[], uint8_t count, uint8_t preselected, bool show_selected) {
  uint8_t offset = 0;
  uint8_t cursorOffset = 0;
  uint8_t lineLength = _cols - 3;
  uint8_t selected = preselected;
  bool needRepaint = 1;
  char buf[_cols + 1];
  uint8_t hasTitle = printTitle(title);

  createChar(0, scrollUp);
  createChar(1, scrollDown);
  createChar(2, iconOk);

  eEncoderState encoderState = eNone;
  while (1) {
    if (needRepaint) {
      needRepaint = 0;
      // Repaint everything except title
      for (uint8_t i = 0; i < min(count, _rows - hasTitle); i++) {
        _prepareForPrint(buf, list[offset + i], lineLength);
        printfAt(0, i + hasTitle, "  %s ", buf);
        if (show_selected) {
          if (offset + i == selected) {
            setCursor(1, i + hasTitle);
            write(2);
          }
        }
      }
      // Showing cursor
      printAt(0, cursorOffset + hasTitle, '>');
      // Showing scrollbar
      if (offset > 0) {
        setCursor(_cols - 1, hasTitle);
        write(0); // Arrow up
      }
      if (offset + _rows - hasTitle < count) {
        setCursor(_cols - 1, _rows - 1);
        write(1); // Arrow down
      }
    }
    encoderState = getEncoderState();
    switch (encoderState) {
      case eNone: {

          encoderIdle();
          continue;
        }
      case eLeft: {
          if (cursorOffset > 0) {
            printAt(0, cursorOffset + hasTitle, ' ');
            printAt(0, --cursorOffset + hasTitle, '>');
          }
          else if (offset > 0) {
            offset--;
            needRepaint = 1;
          }
          continue;
        }
      case eRight: {
          if (cursorOffset < min(_rows - hasTitle, count) - 1) {
            printAt(0, cursorOffset + hasTitle, ' ');
            printAt(0, ++cursorOffset + hasTitle, '>');
          }
          else if ((cursorOffset + hasTitle + 1 == _rows) & (offset + _rows - hasTitle < count)) {
            offset++;
            needRepaint = 1;
          }
          continue;
        }
      case eButton: {
          if ((show_selected)&(offset + cursorOffset != selected)) {
            // set new selected
            if ((selected >= offset) & (selected < offset + _rows - hasTitle))
              printAt(1, selected - offset + hasTitle, ' ');
            selected = offset + cursorOffset;
            setCursor(1, cursorOffset + hasTitle);
            write(2);
            continue;
          }
          else {
            clear();
            return selected;
          }
        }

    }

  }

}

uint8_t LiquidCrystal_I2C_Menu::showMenu(sMenuItem menu[], uint8_t menuLen, bool showTitle) {
  _showMenuTitle = showTitle;
  _menu = menu;
  _menuLen = menuLen;
  uint8_t selectedItem;

  createChar(0, scrollUp);
  createChar(1, scrollDown);
  selectedItem = showSubMenu(1);
  clear();
  return selectedItem;
}

uint8_t LiquidCrystal_I2C_Menu::showSubMenu(uint8_t key) {
  uint8_t result, subMenuLen = 0, subMenuIndex, offset = 0, cursorOffset = 0;
  sMenuItem **subMenu = NULL;
  bool needRepaint = 1;
  char *buffer;
  eEncoderState encoderState;
  subMenuLen = 0;
  uint8_t itemMaxLength = _cols - 1;
  // Filling subMenu array
  for (uint8_t i = 0; i < _menuLen; i++) {
    if (_menu[i].key == key)
      subMenuIndex = i;
    else if (_menu[i].parent == key) {
      subMenuLen++;
      subMenu = (sMenuItem**) realloc (subMenu, subMenuLen * sizeof(void*));
      subMenu[subMenuLen - 1] = &_menu[i];
    }
  }

  if (subMenuLen == 0) // There is no submenu
      return key; // So return the key of selected menu item

  // Showing submenu
  #ifdef SCROLL_LONG_CAPTIONS
    _scrollPos = 0;
    _scrollTime = millis() + DELAY_BEFORE_SCROLL;
  #endif

  if (subMenuLen > _rows - _showMenuTitle) itemMaxLength--;
  do {
    if (needRepaint) {
      needRepaint = 0;
      clear();
      buffer = (char*) malloc (_cols + 1);
      if (_showMenuTitle) {
        memcpy(buffer, _menu[subMenuIndex].caption, _cols);
        buffer[_cols] = 0;
        printAt(0, 0, buffer);
      }
      buffer[_cols] = 0;
      for (uint8_t i = 0; i < min(subMenuLen, _rows - _showMenuTitle); i++) {
        // " %-*.*s" does not work wtf
        memcpy(buffer, subMenu[offset + i]->caption, itemMaxLength);
        buffer[itemMaxLength] = 0;
        printAt(1 , i + _showMenuTitle,  buffer);
      }

      printAt(0, cursorOffset + _showMenuTitle, '>');

      if (offset > 0) {
        setCursor(_cols - 1, _showMenuTitle);
        write(0);
      }
      if (offset + _rows - _showMenuTitle < subMenuLen) {
        setCursor(_cols - 1, _rows - 1);
        write(1);
      }
      free(buffer);
    }
    encoderState = getEncoderState();
    switch (encoderState) {
      case eLeft: {
          // Scrolling up
          #ifdef SCROLL_LONG_CAPTIONS
            _scrollTime = millis() + DELAY_BEFORE_SCROLL;
          #endif
          if (cursorOffset > 0) {  // Moving cursor up if possible
            #ifdef SCROLL_LONG_CAPTIONS
              if (_scrollPos) {
                // If previous menu item is scrolling then print it again
                printAt(1, cursorOffset + _showMenuTitle, String(subMenu[cursorOffset + offset]->caption).substring(0, itemMaxLength));
                _scrollPos = 0;
              }
            #endif
            printAt(0, cursorOffset + _showMenuTitle, ' ');
            printAt(0, --cursorOffset + _showMenuTitle, '>');
          }
          else if (offset > 0) {
            // Cant move cursor upper. If there are other menu items then show them
            offset--;
            needRepaint = 1;
          }
          break;
        }
      case eRight: {
          // Scrolling down
          #ifdef SCROLL_LONG_CAPTIONS
            _scrollTime = millis() + DELAY_BEFORE_SCROLL;
          #endif
          if (cursorOffset < min(_rows - _showMenuTitle, subMenuLen) - 1) {// Moving cursor down if possible
            #ifdef SCROLL_LONG_CAPTIONS
              if (_scrollPos) {
                // If previous menu item is scrolling then print it again
                printAt(1, cursorOffset + _showMenuTitle, String(subMenu[cursorOffset + offset]->caption).substring(0, itemMaxLength));
                _scrollPos = 0;
              }
            #endif
            printAt(0, cursorOffset + _showMenuTitle, ' ');
            printAt(0, ++cursorOffset + _showMenuTitle, '>');
          }
          else if ((cursorOffset + _showMenuTitle + 1 == _rows) & (offset + _rows - _showMenuTitle < subMenuLen)) {
            // Cant move cursor lower. If there are other menu items then show them
            offset++;
            needRepaint = 1;
          }
          break;
        }
      case eButton: {
          // Item selected
          if (subMenu[cursorOffset + offset]->key == 0) { // It's "Back"
            free(subMenu);
            return 0;
          }
          // If selected item has a handler
          _selectedMenuItem = subMenu[cursorOffset + offset]->key;
          if (subMenu[cursorOffset + offset]->handler != NULL) { 
            (*subMenu[cursorOffset + offset]->handler)(); // then executing it
            createChar(0, scrollUp);
            createChar(1, scrollDown);			
          }
          else {// Otherwise calling showSubMenu for this item
            result = showSubMenu(subMenu[cursorOffset + offset]->key);
            if (result != 0) {
              free(subMenu);
              return result;
            }
          }
          #ifdef SCROLL_LONG_CAPTIONS
            _scrollTime = millis() + DELAY_BEFORE_SCROLL;
          #endif
          needRepaint = 1;
          break;
        }
      case eNone: {
          #ifdef SCROLL_LONG_CAPTIONS
            _scrollingCaption = subMenu[cursorOffset + offset]->caption;
            if (_scrollingCaption.length() > itemMaxLength) {
              if (_scrollTime < millis()) {
                _scrollPos++;
                if (_scrollPos == _scrollingCaption.length() - itemMaxLength)
                  _scrollTime = millis() + DELAY_AFTER_SCROLL;
                else if (_scrollPos > _scrollingCaption.length() - itemMaxLength) {
                  _scrollPos = 0;
                  _scrollTime = millis() + DELAY_BEFORE_SCROLL;
                }
                else
                  _scrollTime = millis() + SCROLL_DELAY;
                printAt(1, cursorOffset + _showMenuTitle, _scrollingCaption.substring(_scrollPos, _scrollPos + itemMaxLength));
              }
            }

          #endif // SCROLL_LONG_CAPTIONS

          encoderIdle();
        }
    }
  } while (1);
}

void LiquidCrystal_I2C_Menu::attachIdleFunc(void (*IdleFunc)(void)) {
  _IdleFunc = IdleFunc;
}

void LiquidCrystal_I2C_Menu::encoderIdle() {
  if (_IdleFunc != NULL) (*_IdleFunc)();
}


/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal_I2C_Menu::command(uint8_t value) {
  send(value, 0);
}

inline size_t LiquidCrystal_I2C_Menu::write(uint8_t value) {
  send(value, Rs);
  return 1;
}


/************ low level data pushing commands **********/

// write either command or data
void LiquidCrystal_I2C_Menu::send(uint8_t value, uint8_t mode) {
  uint8_t highnib = value & 0xf0;
  uint8_t lownib = (value << 4) & 0xf0;
  write4bits((highnib) | mode);
  write4bits((lownib) | mode);
}

void LiquidCrystal_I2C_Menu::write4bits(uint8_t value) {
  expanderWrite(value);
  pulseEnable(value);
}

void LiquidCrystal_I2C_Menu::expanderWrite(uint8_t _data) {
  Wire.beginTransmission(_addr);
  Wire.write((int)(_data) | _backlightval);
  Wire.endTransmission();
}

void LiquidCrystal_I2C_Menu::pulseEnable(uint8_t _data) {
  expanderWrite(_data | En);  // En high
  delayMicroseconds(1);   // enable pulse must be >450ns

  expanderWrite(_data & ~En); // En low
  delayMicroseconds(40);    // commands need > 37us to settle
}

void LiquidCrystal_I2C_Menu::load_custom_character(uint8_t char_num, uint8_t *rows) {
  createChar(char_num, rows);
}

void LiquidCrystal_I2C_Menu::setBacklight(uint8_t new_val) {
  if (new_val) {
    backlight();    // turn backlight on
  } else {
    noBacklight();  // turn backlight off
  }
}

void LiquidCrystal_I2C_Menu::printstr(const char c[]) {
  //This function is not identical to the function used for "real" I2C displays
  //it's here so the user sketch doesn't have to be changed
  print(c);
}
