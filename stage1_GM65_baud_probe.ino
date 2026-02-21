// stage1_GM65_baud_probe.ino
// Diagnose GM65 UART data on CrowPanel using I2C header pins

#include <Arduino.h>

static const int LED_PIN = 38;
static const int BARCODE_RX = 20;  // GM65 TX -> ESP32 RX
static const int BARCODE_TX = 19;  // ESP32 TX -> GM65 RX

HardwareSerial BarcodeSerial(2);

static const uint32_t BAUD_LIST[] = {9600, 19200, 38400, 57600, 115200};
static const int BAUD_COUNT = sizeof(BAUD_LIST) / sizeof(BAUD_LIST[0]);

int baudIndex = 0;
unsigned long lastBaudSwitch = 0;
const unsigned long BAUD_WINDOW_MS = 10000;

String digitBuffer;

void blinkOk() {
  digitalWrite(LED_PIN, HIGH);
  delay(80);
  digitalWrite(LED_PIN, LOW);
}

void startScanner(uint32_t baud) {
  BarcodeSerial.end();
  delay(80);
  BarcodeSerial.begin(baud, SERIAL_8N1, BARCODE_RX, BARCODE_TX);

  Serial.println();
  Serial.println("========================================");
  Serial.print("[PROBE] Listening at baud: ");
  Serial.println(baud);
  Serial.println("[PROBE] Scan ONE barcode during this 10s window");
  Serial.println("========================================");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BARCODE_RX, INPUT_PULLUP);  // helps avoid floating RX noise

  Serial.println("\n\nGM65 UART BAUD PROBE");
  Serial.println("Wiring expected:");
  Serial.println("  GM65 TX -> IO20");
  Serial.println("  GM65 RX -> IO19");
  Serial.println("  GM65 GND -> GND");
  Serial.println("  GM65 5V/GND from power-only USB (D+/D- removed)");

  startScanner(BAUD_LIST[baudIndex]);
  lastBaudSwitch = millis();
}

void loop() {
  if (millis() - lastBaudSwitch >= BAUD_WINDOW_MS) {
    baudIndex = (baudIndex + 1) % BAUD_COUNT;
    startScanner(BAUD_LIST[baudIndex]);
    lastBaudSwitch = millis();
    digitBuffer = "";
  }

  while (BarcodeSerial.available()) {
    uint8_t b = (uint8_t)BarcodeSerial.read();

    Serial.print("[BYTE] 0x");
    if (b < 16) Serial.print('0');
    Serial.print(b, HEX);
    Serial.print("  '");
    if (b >= 32 && b <= 126) {
      Serial.print((char)b);
    } else {
      Serial.print('.');
    }
    Serial.println("'");

    if (b >= '0' && b <= '9') {
      digitBuffer += (char)b;
    }

    if (b == '\r' || b == '\n') {
      if (digitBuffer.length() >= 8) {
        Serial.println("----------------------------------------");
        Serial.print("[DECODED] Possible barcode: ");
        Serial.println(digitBuffer);
        Serial.print("[DECODED] Length: ");
        Serial.println(digitBuffer.length());
        Serial.print("[DECODED] Baud working: ");
        Serial.println(BAUD_LIST[baudIndex]);
        Serial.println("----------------------------------------");
        blinkOk();
      }
      digitBuffer = "";
    }
  }
}
