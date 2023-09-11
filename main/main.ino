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
#define POTENTIOMETER_PORT A2

/* 0: Tela inicial
   1: Escolha do primo p
   2: Escolha do primo q
*/

short int currentStage = 0;
short int p;
short int q;
short int curr; 
int primes[5] = {2, 3, 5, 7, 11};
int lastPotPos;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupLCD();
  setupBtn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(currentStage == 0) startup();
  if(isButtonPressed) onButtonPress();
  int pot = getCurrentPotentiometerPosition();
  if(pot != lastPotPos) onPotentiometerValueChange(lastPotPos, pot);
  
}

void log(String invoker, String action) {
  Serial.println("[" + String(millis()) + "] " + invoker + ": " + action);
  
} 

void setupBtn() {
  pinMode(BUTTON_PORT, INPUT_PULLUP);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_PORT) == HIGH;
}

void onButtonPress() {
  log("onButtonPress","pressionado");
  if(currentStage == 0) { // Muda para o 1
    currentStage = 1;
    clearLCD();
    printLCD("Escolha a variável p", 0,0);
  }
  if(currentStage == 1) { // Muda para o 2
    currentStage = 2;
    p = curr; // Salva a variável
    curr = 0;
    clearLCD();
    printLCD("Escolha a variável q", 0,0);
  }
  if(currentStage == 2) {
    currentStage = 3; // Prossegue com o jogo.
    q = curr; // Salva a variável
    curr = 0;
    clearLCD();
    printLCD("Fim de execução", 0, 0);
  }
}

void setupPot() {
  pinMode(POTENTIOMETER_PORT, INPUT);
}

int getCurrentPotentiometerPosition() {
  int potVal = analogRead(POTENTIOMETER_PORT);
  if (potVal < 341)  // Lowest third of the potentiometer's range (0-340)
    potVal = (potVal * 3) / 4; // Normalize to 0-255
  return potVal;
}

void onPotentiometerValueChange(int from, int to) {
  lastPotPos = to;
  curr = primes[(int) (floor(to/51) >= 5 ? 4 : floor(to/51))];
  if(currentStage == 1 || currentStage == 2) refreshPotentiometerDisplay();
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
  printLCD("Primo: " + String(curr),0, 1);
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

void startup() {
  printLCD("Criptografia - IC", 0, 0);
  printLCD("Aperte o botão", 0, 1);
}
