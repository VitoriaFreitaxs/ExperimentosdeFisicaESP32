#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LDR1_PIN 32
#define LDR2_PIN 33
#define LDR3_PIN 34

#define THRESHOLD 500 

LiquidCrystal_I2C lcd(0x27, 16, 2); //  0x27 ou 0x3F

unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;

bool passed1 = false;
bool passed2 = false;
bool passed3 = false;

float distancia_cm = 40.0;  // Distância entre o primeiro e o último LDR

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(LDR1_PIN, INPUT);
  pinMode(LDR2_PIN, INPUT);
  pinMode(LDR3_PIN, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Aguardando...");
}

void loop() {
  int ldr1 = analogRead(LDR1_PIN);
  int ldr2 = analogRead(LDR2_PIN);
  int ldr3 = analogRead(LDR3_PIN);

  // Detecta ausência de luz (sombra)
  if (!passed1 && ldr1 < THRESHOLD) {
    time1 = millis();
    passed1 = true;
    Serial.println("LDR1 ativado");
  }

  if (passed1 && !passed2 && ldr2 < THRESHOLD) {
    time2 = millis();
    passed2 = true;
    Serial.println("LDR2 ativado");
  }

  if (passed2 && !passed3 && ldr3 < THRESHOLD) {
    time3 = millis();
    passed3 = true;
    Serial.println("LDR3 ativado");
  }

  if (passed3) {
    unsigned long deltaTime = time3 - time1; // ms
    float deltaTime_sec = deltaTime / 1000.0; // s
    float velocidade = distancia_cm / deltaTime_sec; // cm/s

    Serial.print("Tempo: ");
    Serial.print(deltaTime_sec, 3);
    Serial.print(" s, Vel: ");
    Serial.print(velocidade, 2);
    Serial.println(" cm/s");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(deltaTime_sec, 2);
    lcd.print("s");

    lcd.setCursor(0, 1);
    lcd.print("V:");
    lcd.print(velocidade, 1);
    lcd.print("cm/s");

    // Aguarda um tempo e reinicia
    delay(5000);
    lcd.clear();
    lcd.print("Aguardando...");

    // Reinicia variáveis
    passed1 = passed2 = passed3 = false;
    time1 = time2 = time3 = 0;
  }

  delay(10); // Pequeno delay para evitar rebotes
}
