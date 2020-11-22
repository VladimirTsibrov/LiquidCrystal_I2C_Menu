#ifndef TSBR_LIQUID_CRYSTAL_I2C_MENU_H
#define TSBR_LIQUID_CRYSTAL_I2C_MENU_H

#include <Arduino.h>
#include <inttypes.h>
#include <Print.h>
#include <stdarg.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

//#define CYRILLIC_DISPLAY // Раскомментировать для поддержки дисплеев с кириллицей
#define SCROLL_LONG_CAPTIONS // Раскомментировать для прокрутки длинных названий в меню и списках
#define ENCODER_POOL_DELAY 5
#define NUMERIC_SIGNS "- "

#define SCROLL_DELAY        800
#define DELAY_BEFORE_SCROLL 4000
#define DELAY_AFTER_SCROLL  2000

//#define INACTIVITY_TIMEOUT 60000 // Таймаут бездействия до выхода из функций

enum eEncoderState {eNone, eLeft, eRight, eButton};

struct sMenuItem {
  uint8_t parent;
  uint8_t key;
  char    *caption;
  void    (*handler)();
};

#ifdef CYRILLIC_DISPLAY
  uint8_t strlenUTF8(const char *);
  void substrUTF8(const char*, char*, uint8_t, uint8_t);
#endif

/**
   This is the driver for the Liquid Crystal LCD displays that use the I2C bus.

   After creating an instance of this class, first call begin() before anything else.
   The backlight is on by default, since that is the most likely operating mode in
   most cases.
*/
class LiquidCrystal_I2C_Menu : public Print {
  public:
    /**
       Constructor

       @param lcd_addr  I2C slave address of the LCD display. Most likely printed on the
                LCD circuit board, or look in the supplied LCD documentation.
       @param lcd_cols  Number of columns your LCD display has.
       @param lcd_rows  Number of rows your LCD display has.
       @param charsize  The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
    */
    LiquidCrystal_I2C_Menu(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);

    /**
       Set the LCD display in the correct begin state, must be called before anything else is done.
    */
    void begin();

    /**
       Remove all the characters currently shown. Next print/write operation will start
       from the first position on LCD display.
    */
    void clear();

    /**
       Next print/write operation will will start from the first position on the LCD display.
    */
    void home();

    /**
       Do not show any characters on the LCD display. Backlight state will remain unchanged.
       Also all characters written on the display will return, when the display in enabled again.
    */
    void noDisplay();

    /**
       Show the characters on the LCD display, this is the normal behaviour. This method should
       only be used after noDisplay() has been used.
    */
    void display();

    /**
       Do not blink the cursor indicator.
    */
    void noBlink();

    /**
       Start blinking the cursor indicator.
    */
    void blink();

    /**
       Do not show a cursor indicator.
    */
    void noCursor();

    /**
       Show a cursor indicator, cursor can blink on not blink. Use the
       methods blink() and noBlink() for changing cursor blink.
    */
    void cursor();

    void scrollDisplayLeft();
    void scrollDisplayRight();
    void printLeft();
    void printRight();
    void leftToRight();
    void rightToLeft();
    void shiftIncrement();
    void shiftDecrement();
    void noBacklight();
    void backlight();
    bool getBacklight();
    void autoscroll();
    void noAutoscroll();
    void createChar(uint8_t, uint8_t[]);
    void setCursor(uint8_t, uint8_t);
    virtual size_t write(uint8_t);
    void command(uint8_t);

    inline void blink_on() {
      blink();
    }
    inline void blink_off() {
      noBlink();
    }
    inline void cursor_on() {
      cursor();
    }
    inline void cursor_off() {
      noCursor();
    }

    // Compatibility API function aliases
    void setBacklight(uint8_t new_val);       // alias for backlight() and nobacklight()
    void load_custom_character(uint8_t char_num, uint8_t *rows);  // alias for createChar()
    void printstr(const char[]);

    // Extended functions
    /**
       printAt = setCursor + print.
    */
    void printAt(uint8_t, uint8_t, const String &);
    void printAt(uint8_t, uint8_t, const char[]);
    void printAt(uint8_t, uint8_t, char);
    void printAt(uint8_t, uint8_t, unsigned char, int = DEC);
    void printAt(uint8_t, uint8_t, int, int = DEC);
    void printAt(uint8_t, uint8_t, unsigned int, int = DEC);
    void printAt(uint8_t, uint8_t, long, int = DEC);
    void printAt(uint8_t, uint8_t, unsigned long, int = DEC);
    void printAt(uint8_t, uint8_t, double, int = 2);
    void printAt(uint8_t, uint8_t, const Printable&);

    /**
       printf - печать форматированных строк.
    */
    void printf(const char *, ...);

    /**
       printfAt - печать форматированных строк с указанной позиции.
    */
    void printfAt(uint8_t, uint8_t, const char *, ...);

    /**
       Установка пинов для энкодера
    */
    void attachEncoder(uint8_t, uint8_t, uint8_t);

    /**

    */
    eEncoderState getEncoderState();

    /**
       printMultiline - печать текста с возможностью вертикальной прокрутки.
    */
    void printMultiline(const String &);
    void printMultiline(const char[]);

    /**
       Ввод значений путем инкремента/декремента начального значения.
       inputValAt не очищает экран, позволяя использовать printf для вывод заголовка.
    */
    template <typename T> T inputVal(const String &, T, T, T, T = 1, void (*)(T) = NULL); // title, min, max, default, step = 1, onChangeFunc
    template <typename T> T inputVal(const char[], T, T, T, T = 1, void (*)(T) = NULL); // title, min, max, default, step = 1, onChangeFunc
    template <typename T> T inputValAt(uint8_t, uint8_t, T, T, T, T = 1, void (*)(T) = NULL); // x, y, min, max, default, step = 1, onChangeFunc

    /**
       Ввод числовых и строковых значений путем редактирования отдельных разрядов.
       Возвращает TRUE если пользователь подтвердил ввод.
    */
    template <typename T> bool inputValBitwise(const String &, T &, uint8_t, uint8_t = 0, bool _signed = 0); // title, value, precision, scale
    template <typename T> bool inputValBitwise(const char[], T &, uint8_t, uint8_t = 0, bool _signed = 0); // title, value, precision, scale
    bool inputStrVal(const String &, char[], uint8_t, const char[]); // title, buffer, length, available symbols
    bool inputStrVal(const char[], char[], uint8_t, const char[]); // title, buffer, length, available symbols


    /**
       Выбор значения из списка. Возвращает индекс выбранного элемента.
    */
    uint8_t selectVal(const String &, const char**, uint8_t, bool = true, uint8_t = 0); //title, list of values, count, show selected, selected index
    uint8_t selectVal(const char[], const char**, uint8_t, bool = true, uint8_t = 0); //title, list of values, count, show selected, selected index
    uint8_t selectVal(const String &, String[], uint8_t, bool = true, uint8_t = 0); //title, list of values, count, show selected, selected index
    uint8_t selectVal(const char[], String[], uint8_t, bool = true, uint8_t = 0); //title, list of values, count, show selected, selected index
    uint8_t selectVal(const String &, int[], uint8_t, bool = true, uint8_t = 0); //title, list of values, count, show selected, selected index
    uint8_t selectVal(const char[], int[], uint8_t, bool = true, uint8_t = 0); //title, list of values, count, show selected, selected index

    /**
       Функция отображения меню. Возвращает ключ выбранного пункта меню.
    */
    uint8_t showMenu(sMenuItem[], uint8_t, bool);
    uint8_t getSelectedMenuItem(){return _selectedMenuItem;};
    void attachIdleFunc(void (*IdleFunc)(void));
  private:
    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void expanderWrite(uint8_t);
    void pulseEnable(uint8_t);
    uint8_t _addr;
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _charsize;
    uint8_t _backlightval;

    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _pinBtn;
    unsigned long _prevPoolTime;
    bool _pinButtonPrev;
    bool _pinAPrev;
    bool _showMenuTitle;
    uint8_t _menuLen;
    sMenuItem *_menu;
    #ifdef SCROLL_LONG_CAPTIONS
      uint8_t _scrollPos;
      unsigned long _scrollTime;
    #endif
    #if defined(INACTIVITY_TIMEOUT)
      unsigned long lastActivityTime;
    #endif
    uint8_t _selectedMenuItem;
    void  (*_IdleFunc)() = NULL;
    bool _inputLongVal(const char[], long &, long, long);
    bool getNextEditable(char S[], uint8_t lenS, const char availSymbols[], uint8_t &currentPos, bool direction);
    bool isEditable(const char* ch, const char availSymbols[]);
    bool getNextSymbol(char *ch, bool direction, const char availSymbols[], bool looped = 0);
    bool _inputStrVal(const char title[], char buffer[], uint8_t len, const char availSymbols[], bool _signed);
    template <typename T> uint8_t _selectVal(const char[], T[], uint8_t, bool, uint8_t);
    bool printTitle(const char title[]);
    void _prepareForPrint(char [], char*, uint8_t);
    void _prepareForPrint(char [], int, uint8_t);
    void _prepareForPrint(char [], String, uint8_t);
    uint8_t showSubMenu(uint8_t);
    void encoderIdle();
};

template <typename T> T LiquidCrystal_I2C_Menu::inputVal(const String &title, T minValue, T maxValue, T defaultValue, T step, void (*onChangeFunc)(T)) {
  return inputVal(title.c_str(), minValue, maxValue, defaultValue, step, onChangeFunc);
}

template <typename T> T LiquidCrystal_I2C_Menu::inputVal(const char title[], T minValue, T maxValue, T defaultValue, T step, void (*onChangeFunc)(T)) {
  T v = defaultValue;
  if ((minValue > maxValue) or (v < minValue) or (v > maxValue))
    return v;
  uint8_t hasTitle = printTitle(title);
  v = inputValAt(0, hasTitle, minValue, maxValue, defaultValue, step, onChangeFunc);
  clear();
  return v;
}

template <typename T> T LiquidCrystal_I2C_Menu::inputValAt(uint8_t x, uint8_t y, T minValue, T maxValue, T defaultValue, T step, void (*onChangeFunc)(T)) {
  T v = defaultValue;
  if ((minValue > maxValue) or (v < minValue) or (v > maxValue))
    return v;
  eEncoderState encoderState = eNone;
  printAt(x, y, v);
  if (onChangeFunc != NULL) (*onChangeFunc)(v);
  String S;
  #if defined(INACTIVITY_TIMEOUT)
    lastActivityTime = millis();
  #endif
  while (1) {
    encoderState = getEncoderState();
    #ifdef INACTIVITY_TIMEOUT
      // Выход по таймауту
      if (millis() - lastActivityTime > INACTIVITY_TIMEOUT) return defaultValue;
    #endif
    switch (encoderState) {
      case eNone:
        encoderIdle();
        continue;
      case eButton:
        return v;
      case eLeft:
        S = String(v);
        if (v >= minValue + step) v -= step;
        else v = minValue;
        break;
      case eRight:
        S = String(v);
        if (v + step <= maxValue) v += step;
        else v = maxValue;
        break;
    }
    for (uint8_t i = 0; i < S.length(); i++)
      S.setCharAt(i, ' ');
    printAt(x, y, S.c_str());
    printAt(x, y, v);
	if (onChangeFunc != NULL) (*onChangeFunc)(v);
  }
}

template <typename T> bool LiquidCrystal_I2C_Menu::inputValBitwise(const String &title, T &value, uint8_t precision, uint8_t scale, bool _signed) {
  return inputValBitwise(title.c_str(), value, precision, scale);
}

template <typename T> bool LiquidCrystal_I2C_Menu::inputValBitwise(const char title[], T &value, uint8_t precision, uint8_t scale, bool _signed) {
  char buffer[_cols];
  String S;
  int dotPos;
  if (value)
    dtostrf(abs(value), precision, scale, buffer);
  else
    dtostrf(1, precision, scale, buffer);
  S = buffer;
  if (value == 0) S.replace("1", "0");

  dotPos = S.lastIndexOf(".");
  if (S.length() > (unsigned)(precision + (dotPos >= 0))) return 0; // Значение не соответствует заданной точности
  S.replace(" ", "0");
  if ((dotPos > 0) & (dotPos < precision - scale)) S = "0" + S;
  if (_signed) {
    if (value < 0) S = "-" + S;
    else S = " " + S;
  }
  S.getBytes((unsigned char*)buffer, S.length() + 1);
  if (_inputStrVal(title, buffer, S.length(), "0123456789", _signed)) {
    S = buffer;
    S.trim();
    if (dotPos >= 0) value = S.toFloat();
    else value = S.toInt();
    return 1;
  }

  return 0;
}

#endif // TSBR_LIQUID_CRYSTAL_I2C_MENU_H 
