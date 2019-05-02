#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#define cols 20
#define rows 4
LiquidCrystal_I2C_Menu lcd(0x27, cols, rows);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

char ip[] = "192.168.001.001";

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  if (lcd.inputStrVal("Input IP", ip, 15, "0123456789")){
    lcd.print("You entered:");
    lcd.printAt(0, 1, ip);
  }
  else
    lcd.print("Input canceled");
  press_button_to_continue();
}

void press_button_to_continue(){
  // Waiting for button to continue
  if (rows == 4) {
    lcd.printAt(0, 2, "Press button");
    lcd.printAt(0, 3, "to continue");
  }
  while (lcd.getEncoderState() != eButton);
}
