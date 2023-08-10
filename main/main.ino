#include <LiquidCrystal.h>

// https://docs.arduino.cc/learn/electronics/lcd-displays

#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

// https://mundoprojetado.com.br/buzzer-como-usar-com-o-arduino/

#define BUZZER_PORT 10


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupLCD();
}

void loop() {
  // put your main code here, to run repeatedly:
  anim_1_test(true);
  // Será usado no keypad/numpad (https://www.ardumotive.com/how-to-use-a-keypad-en.html)
}

void log(String invoker, String action) {
  Serial.println("[" + String(millis()) + "] " + invoker + ": " + action);
  
} 


LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setupLCD () {
    log("setupLCD", "Inicializando LCD com autoscroll.");
    lcd.begin(16, 2);
    lcd.autoscroll();
}

void printLCD(const String message) {
    log("printLCD", "Imprimiu '" + message + "'");
    lcd.print(message);
}

void printLCD(const String message, short unsigned int x, short unsigned int y) {
    log("printLCD", "Imprimiu '" + message + "' nas coordenadas " + x + "," + y);
    lcd.setCursor(x,y);
    lcd.print(message);
}

void setLCDState(bool state) {
  if(state) {
    log("setLCDState", "Desligou o LCD");
    lcd.noDisplay();
  }else{
    log("setLCDState", "Ligou o LCD");
    lcd.display();
  }
}

/*
https://mundoprojetado.com.br/buzzer-como-usar-com-o-arduino/
Frequência das notas:
Dó - 262 Hz
Ré - 294 Hz
Mi - 330 Hz
Fá - 349 Hz
Sol - 392 Hz
Lá - 440 Hz
Si - 494 Hz
Dó - 528 Hz
*/
void playNote(int frequency, int duration) {
  float period = 1000.0/frequency; //Periodo em ms
  for (int i = 0; i< duration/(period);i++){ //Executa a rotina de dentro o tanta de vezes que a frequencia desejada cabe dentro da duracao
    digitalWrite(BUZZER_PORT,HIGH);
    delayMicroseconds(period*500); //Metade do periodo em ms
    digitalWrite(BUZZER_PORT, LOW);
    delayMicroseconds(period*500);
  }
}

// Playground

void anim_1_test(bool clean_before) {
  short int maxX = 15;
  short int maxY = 1;
  const String alphabet{"abcdefghijklmnopqrstuvwxyz"};
  int index = 0;

  for(int x=0; x<=maxX; x++) {
    for(int y=0; y<=maxY; y++) {
      char letter = alphabet[index % alphabet.length() - 1];
      index++;

      // Já que o cursor está na posição anterior, podemos usar a função genérica para isso.
      if(clean_before) {
        printLCD("");
      }
      printLCD(String(letter), x, y);
    }
  }
}
