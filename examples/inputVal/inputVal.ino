#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#define cols 20
#define rows 4
LiquidCrystal_I2C_Menu lcd(0x27, cols, rows);

// Encoder pins
#define pinCLK 2
#define pinDT  3
#define pinSW  4

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}

void loop() {
  // Asking user for array size
  uint8_t len = lcd.inputVal("Input array len", 5, 10, 8);
  uint8_t A[len];
  uint8_t t;
  
  // Initialize elements of the array
  for (uint8_t i = 0; i < len; i++) {
    lcd.printfAt(0, 0, "Input A[%d]: ", i);
    A[i] = lcd.inputValAt(12, 0, 0, 9, 5);
  }
  
  // Sorting array
  for (uint8_t i = 0; i < len - 1; i++) {
    for (uint8_t j = i + 1; j < len; j++) {
      if(A[i] > A[j]){
        t = A[i];
        A[i] = A[j];
        A[j] = t;
      }
    }
  }
  
  // Print sorted array
  lcd.clear();
  lcd.print("Sorted array:");
  lcd.setCursor(0, 1);
  for (uint8_t i = 0; i < len; i++)
    lcd.printf("%d ", A[i]);
  
  // Waiting for button to continue
  if (rows == 4) {
    lcd.printAt(0, 2, "Press button");
    lcd.printAt(0, 3, "to continue");
  }
  while (lcd.getEncoderState() != eButton);
}
