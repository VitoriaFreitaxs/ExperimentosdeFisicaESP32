#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa LCD no endereço 0x27 (16 colunas e 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Novos pinos analógicos da ESP32
const int ldr1Pin = 34;  // GPIO34 (entrada analógica apenas)
const int ldr2Pin = 35;  // GPIO35 (entrada analógica apenas)

int limiar = 2000; // Ajuste conforme a iluminação e divisão de tensão
bool detectadoLDR1 = false;
bool detectadoLDR2 = false;

unsigned long tempoInicio = 0;
unsigned long tempoFim = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ldr1Pin, INPUT);
  pinMode(ldr2Pin, INPUT);

  // Inicia o I2C nos pinos padrão da ESP32 (SDA = 21, SCL = 22)
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Experimento Queda");
  lcd.setCursor(0, 1);
  lcd.print("Aguardando...");
}

void loop() {
  int valorLDR1 = analogRead(ldr1Pin);
  int valorLDR2 = analogRead(ldr2Pin);

  if (!detectadoLDR1 && valorLDR1 < limiar) {
    tempoInicio = micros();
    detectadoLDR1 = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor 1 ativado");
  }

  if (detectadoLDR1 && !detectadoLDR2 && valorLDR2 < limiar) {
    tempoFim = micros();
    detectadoLDR2 = true;

    unsigned long tempoQueda = tempoFim - tempoInicio;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tempo de queda:");
    lcd.setCursor(0, 1);
    lcd.print(tempoQueda);
    lcd.print(" us");

    Serial.print("Tempo de queda: ");
    Serial.print(tempoQueda);
    Serial.println(" microssegundos");

    delay(5000); // Espera antes de resetar

    // Reinicia estado
    detectadoLDR1 = false;
    detectadoLDR2 = false;
    tempoInicio = 0;
    tempoFim = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nova medicao...");
  }
}