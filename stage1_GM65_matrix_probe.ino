// stage1_GM65_matrix_probe.ino
// Deep UART probe for GM65 on CrowPanel

#include <Arduino.h>

static const int LED_PIN = 38;
HardwareSerial BarcodeSerial(2);

struct UartCfg {
  int rxPin;
  int txPin;
  bool invert;
  const char* name;
};

static const UartCfg CFGS[] = {
  {20, 19, false, "RX20_TX19_NORMAL"},
  {20, 19, true,  "RX20_TX19_INVERT"},
  {19, 20, false, "RX19_TX20_NORMAL"},
  {19, 20, true,  "RX19_TX20_INVERT"}
};

static const uint32_t BAUDS[] = {4800, 9600, 19200, 38400, 57600, 115200};

static const int CFG_COUNT = sizeof(CFGS) / sizeof(CFGS[0]);
static const int BAUD_COUNT = sizeof(BAUDS) / sizeof(BAUDS[0]);

int cfgIndex = 0;
int baudIndex = 0;
unsigned long lastSwitchMs = 0;
const unsigned long WINDOW_MS = 8000;

String digits;
int byteCount = 0;

void blink() {
  digitalWrite(LED_PIN, HIGH);
  delay(60);
  digitalWrite(LED_PIN, LOW);
}

void startProbe() {
  const UartCfg &cfg = CFGS[cfgIndex];
  uint32_t baud = BAUDS[baudIndex];

  BarcodeSerial.end();
  delay(80);
  BarcodeSerial.begin(baud, SERIAL_8N1, cfg.rxPin, cfg.txPin, cfg.invert);

  pinMode(cfg.rxPin, INPUT_PULLUP);

  digits = "";
  byteCount = 0;

  Serial.println();
  Serial.println("================================================");
  Serial.print("[CFG] ");
  Serial.println(cfg.name);
  Serial.print("[CFG] baud=");
  Serial.println(baud);
  Serial.println("[ACTION] Scan barcode now (8s window)");
  Serial.println("================================================");
}

void nextProbe() {
  baudIndex++;
  if (baudIndex >= BAUD_COUNT) {
    baudIndex = 0;
    cfgIndex++;
    if (cfgIndex >= CFG_COUNT) {
      cfgIndex = 0;
      Serial.println("\n[INFO] Restarting full matrix...\n");
    }
  }
  startProbe();
  lastSwitchMs = millis();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\nGM65 MATRIX PROBE");
  Serial.println("Expected GM65 wiring:");
  Serial.println("  GM65 TX -> IO20, GM65 RX -> IO19, GND common");
  Serial.println("  GM65 powered by 5V power-only USB (no D+/D-)");

  startProbe();
  lastSwitchMs = millis();
}

void loop() {
  if (millis() - lastSwitchMs >= WINDOW_MS) {
    Serial.print("[RESULT] bytes seen in window: ");
    Serial.println(byteCount);
    if (digits.length() >= 8) {
      Serial.print("[RESULT] candidate barcode: ");
      Serial.println(digits);
      blink();
    }
    nextProbe();
  }

  while (BarcodeSerial.available()) {
    uint8_t b = (uint8_t)BarcodeSerial.read();
    byteCount++;

    Serial.print("[BYTE] 0x");
    if (b < 16) Serial.print('0');
    Serial.print(b, HEX);
    Serial.print(" '");
    if (b >= 32 && b <= 126) Serial.print((char)b);
    else Serial.print('.');
    Serial.println("'");

    if (b >= '0' && b <= '9') {
      digits += (char)b;
    }

    if (b == '\r' || b == '\n') {
      if (digits.length() >= 8) {
        Serial.print("[DECODED] ");
        Serial.println(digits);
        blink();
      }
      digits = "";
    }
  }
}
