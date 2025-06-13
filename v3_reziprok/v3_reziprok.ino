#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include <ESP8266WiFi.h>  

#define MCP_HF 0x20
#define MCP_REF 0x24

#define GAL 0
#define GAU 1
#define GBL 2
#define GBU 3

#define Y0 8  // Bit 0 = GPIO8
#define Y1 9
#define Y2 10
#define Y3 11
#define Y4 12
#define Y5 13
#define Y6 14
#define Y7 15

#define CE_Not 6
#define C_state 7

Adafruit_MCP23017 mcpHF;
Adafruit_MCP23017 mcpREF;

const int AVERAGE_COUNT = 10;
float freqHistory[AVERAGE_COUNT];
int avgIndex = 0;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);

  mcpHF.begin(MCP_HF);
  mcpREF.begin(MCP_REF);

  // Init Datenleitungen (Y0�Y7) als Input
  for (int i = Y0; i <= Y7; i++) {
    mcpHF.pinMode(i, INPUT);
    mcpREF.pinMode(i, INPUT);
  }

  // Init Multiplexleitungen und CE_Not als Output
  for (int i = 0; i <= 6; i++) {
    mcpHF.pinMode(i, OUTPUT); mcpHF.digitalWrite(i, HIGH);
    mcpREF.pinMode(i, OUTPUT); mcpREF.digitalWrite(i, HIGH);
  }

  // Clock-Status f�r 1Hz-Tor ist nur an mcpHF
  mcpHF.pinMode(C_state, INPUT);

  // Warten bis 1?Hz-Tor aktiv ist
  Serial.println("Warte auf Gate...");
  while (!mcpHF.digitalRead(C_state)) yield();
  while ( mcpHF.digitalRead(C_state)) yield();
  while (!mcpHF.digitalRead(C_state)) yield();

  delay(500);
  Serial.println("Zaehler laeuft.");
}

void loop() {
  float freq = measureReciprocal();

  // Gleitender Mittelwert berechnen
  freqHistory[avgIndex] = freq;
  avgIndex = (avgIndex + 1) % AVERAGE_COUNT;

  float avgFreq = 0;
  for (int i = 0; i < AVERAGE_COUNT; i++) {
    avgFreq += freqHistory[i];
  }
  avgFreq /= AVERAGE_COUNT;

  Serial.printf("Frequenz: %.6f Hz\n", avgFreq);
  delay(10);
}

uint8_t get_byte(Adafruit_MCP23017 &mcp) {
  return mcp.readGPIO(1);
}

unsigned long read32(Adafruit_MCP23017 &mcp) {
  unsigned long value = 0;

  mcp.digitalWrite(GBU, LOW); delayMicroseconds(1);
  value |= ((unsigned long)get_byte(mcp)) << 24;
  mcp.digitalWrite(GBU, HIGH);

  mcp.digitalWrite(GBL, LOW); delayMicroseconds(1);
  value |= ((unsigned long)get_byte(mcp)) << 16;
  mcp.digitalWrite(GBL, HIGH);

  mcp.digitalWrite(GAU, LOW); delayMicroseconds(1);
  value |= ((unsigned long)get_byte(mcp)) << 8;
  mcp.digitalWrite(GAU, HIGH);

  mcp.digitalWrite(GAL, LOW); delayMicroseconds(1);
  value |= get_byte(mcp);
  mcp.digitalWrite(GAL, HIGH);

  return value;
}

float measureReciprocal() {
  // Warten auf Ende des Gatesignals (1Hz Flanke)
  while ( mcpHF.digitalRead(C_state)) yield();
  delay(1);

  // Beide Z�hler stoppen
  mcpHF.digitalWrite(CE_Not, LOW);
  mcpREF.digitalWrite(CE_Not, LOW);

  // Z�hlerwerte lesen
  unsigned long hf_count = read32(mcpHF);
  unsigned long ref_count = read32(mcpREF);

  mcpHF.digitalWrite(CE_Not, HIGH);
  mcpREF.digitalWrite(CE_Not, HIGH);

  // Warten auf neue Gate-Flanke
  while (!mcpHF.digitalRead(C_state)) yield();

  // Frequenz berechnen
  float duration_sec = (float)ref_count / 10000000.0; // da 10MHz
  float frequency = (float)hf_count / duration_sec;

  return frequency;
}
