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

// https://docs.arduino.cc/built-in-examples/digital/Button
#define BUTTON_PORT 6

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
#define POTENTIOMETER_PORT A5

/* 0: Tela inicial
   1: Escolha do primo p
   2: Escolha do primo q
*/

short int currentStage = 0;
short int p;
short int q;
short int curr = 0;
int primes[11] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
int lastPotPos;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupLCD();
  setupBtn();
  if(currentStage == 0) startup();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(isButtonPressed()) onButtonPress();
  int pot = getCurrentPotentiometerPosition();
  if(pot != lastPotPos) onPotentiometerValueChange(lastPotPos, pot);
  
  int x = digitalRead(BUTTON_PORT);
}

void log(String invoker, String action) {
  Serial.println("<stage::"+String(currentStage)+"> [" + String(millis()) + "] " + invoker + ": " + action);
  
} 

void setupBtn() {
  pinMode(BUTTON_PORT, INPUT_PULLUP);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_PORT) == HIGH;
}

void onButtonPress() {
  log("onButtonPress","pressionado " + String(digitalRead(BUTTON_PORT) + " / curr: " + String(curr)));
  if(currentStage == 0) { // Muda para o 1
    currentStage = 1;
    clearLCD();
    printLCD("Variavel p", 0,0);
  } else if(curr == 0) return; else
  if(currentStage == 1) { // Muda para o 2
    currentStage = 2;
    p = curr; // Salva a variável
    curr = 0;
    clearLCD();
    printLCD("Variavel q", 0,0);
  } else
  if(currentStage == 2) {
    currentStage = 3; // Prossegue com o jogo.
    q = curr; // Salva a variável
    curr = 0;
    clearLCD();
    printLCD("Fim de execucao", 0, 0);
  }
}

void setupPot() {
  pinMode(POTENTIOMETER_PORT, INPUT);
}

int getCurrentPotentiometerPosition() {
  int potVal = analogRead(POTENTIOMETER_PORT);
  return potVal;
}

void onPotentiometerValueChange(int from, int to) {
  log("onPotentiometerValueChange", String(getCurrentPotentiometerPosition()));
  lastPotPos = to;
  curr = primes[(int) (floor(to/93) >= 11 ? 10 : floor(to/93))];
  if(currentStage == 1 || currentStage == 2) refreshPotentiometerDisplay();
}



LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setupLCD () {
    log("setupLCD", "Inicializando LCD.");
    lcd.begin(16, 2);
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

void clearLCD() {
  lcd.clear();
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

void refreshPotentiometerDisplay() {
  printLCD("Primo: " + String(curr) + "  ",0, 1);
}

void startup() {
  char b[] = "Aperte o botao";
  clearLCD();
  printLCD("SESI CriptoBox",0,0);
  printLCD(b,0,1);
}
