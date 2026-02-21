// stage1_GM65_activity_check.ino
// Baud-independent activity check for GM65 TX line

#include <Arduino.h>

static const int LED_PIN = 38;
static const int GM65_TX_TO_ESP_RX_PIN = 20;  // GM65 TX should be here

volatile uint32_t edgeCount = 0;
volatile uint32_t riseCount = 0;
volatile uint32_t fallCount = 0;

void IRAM_ATTR onEdge() {
  edgeCount++;
  if (digitalRead(GM65_TX_TO_ESP_RX_PIN)) riseCount++;
  else fallCount++;
}

void setup() {
  Serial.begin(115200);
  delay(800);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(GM65_TX_TO_ESP_RX_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GM65_TX_TO_ESP_RX_PIN), onEdge, CHANGE);

  Serial.println("\nGM65 ACTIVITY CHECK");
  Serial.println("Expected wiring:");
  Serial.println("  GM65 TX -> IO20");
  Serial.println("  GM65 RX -> IO19");
  Serial.println("  GM65 GND -> GND");
  Serial.println("  GM65 power from 5V USB (D+/D- removed)");
  Serial.println("\nWatch edge count while scanning.");
  Serial.println("If edges stay near 0, scanner TX is NOT outputting UART.");
}

void loop() {
  static uint32_t lastEdge = 0;
  static uint32_t lastRise = 0;
  static uint32_t lastFall = 0;

  delay(1000);

  noInterrupts();
  uint32_t e = edgeCount;
  uint32_t r = riseCount;
  uint32_t f = fallCount;
  interrupts();

  uint32_t de = e - lastEdge;
  uint32_t dr = r - lastRise;
  uint32_t df = f - lastFall;

  lastEdge = e;
  lastRise = r;
  lastFall = f;

  Serial.print("[1s] edges=");
  Serial.print(de);
  Serial.print(" rise=");
  Serial.print(dr);
  Serial.print(" fall=");
  Serial.print(df);
  Serial.print(" pin_now=");
  Serial.println(digitalRead(GM65_TX_TO_ESP_RX_PIN));

  if (de > 40) {
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
  }
}
