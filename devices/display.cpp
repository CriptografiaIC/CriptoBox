#include "display.h"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



void setupLCD () {
    lcd.begin(16, 2);
}

void printLCD(const char* message) {
    lcd.print(message);
}

void printLCD(const char* message, short unsigned int x, short unsigned int y) {
    lcd.setCursor(x,y);
    lcd.print(message);
}