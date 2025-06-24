// Reziproker Frequenzmesser mit OLED-Ausgabe und wählbarer Gatezeit über GPA5 (Pin 26 am MCP @ 0x24)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23017.h>

#define MCP_HF  0x20
#define MCP_REF 0x24
#define GAL 0
#define GAU 1
#define GBL 2
#define GBU 3
#define CE_Not 6
#define C_state 7
#define Y0 8
#define Y7 15
#define GATESEL 5 // GPA5 = Pin 26

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_MCP23017 mcpHF;
Adafruit_MCP23017 mcpREF;

const int AVG_SIZE = 10;
double avgBuffer[AVG_SIZE];
int avgIndex = 0;
bool bufferFull = false;

unsigned long freq = 0;
unsigned long ref  = 0;
bool useLongGate = false;

uint8_t get_byte(Adafruit_MCP23017 &mcp) {
  return mcp.readGPIO(1);
}

unsigned long readCounter(Adafruit_MCP23017 &mcp) {
  unsigned long val = 0;

  mcp.digitalWrite(GBU, LOW);
  val |= ((unsigned long)get_byte(mcp)) << 24;
  mcp.digitalWrite(GBU, HIGH);

  mcp.digitalWrite(GBL, LOW); 
  val |= ((unsigned long)get_byte(mcp)) << 16;
  mcp.digitalWrite(GBL, HIGH);

  mcp.digitalWrite(GAU, LOW); 
  val |= ((unsigned long)get_byte(mcp)) << 8;
  mcp.digitalWrite(GAU, HIGH);

  mcp.digitalWrite(GAL, LOW); 
  val |= get_byte(mcp);
  mcp.digitalWrite(GAL, HIGH);

  return val;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ SSD1306 nicht gefunden!");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  Serial.println("🔧 Initialisiere MCPs...");
  mcpHF.begin(0);
  mcpREF.begin(4);

  for (int i = Y0; i <= Y7; i++) {
    mcpHF.pinMode(i, INPUT);
    mcpREF.pinMode(i, INPUT);
  }
  for (int i = 0; i <= 6; i++) {
    mcpHF.pinMode(i, OUTPUT); mcpHF.digitalWrite(i, HIGH);
    mcpREF.pinMode(i, OUTPUT); mcpREF.digitalWrite(i, HIGH);
  }
  mcpHF.pinMode(C_state, INPUT);
  mcpREF.pinMode(GATESEL, INPUT);

  delay(500);
  Serial.println("Warte auf Gate");
  while(!mcpHF.digitalRead(C_state)){yield();} 
  while(mcpHF.digitalRead(C_state)){yield();} 
  while(!mcpHF.digitalRead(C_state)){yield();}

  Serial.println("Zaehler laeuft");
  Serial.println("✅ Initialisierung abgeschlossen.");
}

void loop() {
  useLongGate = mcpREF.digitalRead(GATESEL); // HIGH = lange Gatezeit (60 s)

  // Flankenerkennung: LOW → HIGH → LOW
  while (mcpHF.digitalRead(C_state)) yield(); // warte auf FALLENDE Flanke → Gateende

  if (useLongGate) {
    for (int i = 0; i < 59; i++) {
      while (!mcpHF.digitalRead(C_state)) yield();
      while (mcpHF.digitalRead(C_state)) yield();
    }
  }
  

  delayMicroseconds(10);

  mcpHF.digitalWrite(CE_Not, LOW);
  mcpREF.digitalWrite(CE_Not, LOW);
  delayMicroseconds(2);

  freq = readCounter(mcpHF);
  ref  = readCounter(mcpREF);
  while (!mcpHF.digitalRead(C_state)) yield();  // warte auf HIGH (Start Gate)
  mcpHF.digitalWrite(CE_Not, HIGH);
  mcpREF.digitalWrite(CE_Not, HIGH);

  

  if (ref > 0) {
    double gate_time = (double)ref / 10000000.0;
    double f = (double)freq / gate_time;

    avgBuffer[avgIndex++] = f;
    if (avgIndex >= AVG_SIZE) { avgIndex = 0; bufferFull = true; }

    int count = bufferFull ? AVG_SIZE : avgIndex;
    double sum = 0;
    for (int i = 0; i < count; i++) sum += avgBuffer[i];
    double avg = sum / count;

    Serial.printf("📊 Frequenz: %.9f Hz | REF: %lu | Gatezeit: %s\n", avg, ref, useLongGate ? "60s" : "1s");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Frequenz (");
    display.print(useLongGate ? "60s):" : "1s):");

    display.setCursor(0, 12);
    if (f < 1.0) {
      display.printf("%.7f mHz", f * 1000.0);
    } else if (f >= 1000000.0) {
      display.printf("%.7f MHz", f / 1000000.0);
    } else if (f >= 1000.0) {
      display.printf("%.7f kHz", f / 1000.0);
    } else {
      display.printf("%.6f Hz", f);
    }

    display.setCursor(0, 24);
    display.printf("AVG: %.5f Hz", avg);
    display.display();
  } else {
    Serial.println("⚠️ Referenzzähler = 0");
  }
}
