// stage1_GM65_dual_activity_check.ino
// Check activity on both IO20 and IO19 simultaneously

#include <Arduino.h>

static const int LED_PIN = 38;
static const int PIN_A = 20; // expected GM65 TX target
static const int PIN_B = 19; // expected GM65 RX target

volatile uint32_t edgeA = 0;
volatile uint32_t edgeB = 0;

void IRAM_ATTR onEdgeA() { edgeA++; }
void IRAM_ATTR onEdgeB() { edgeB++; }

void setup() {
  Serial.begin(115200);
  delay(800);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_A), onEdgeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_B), onEdgeB, CHANGE);

  Serial.println("\nGM65 DUAL ACTIVITY CHECK");
  Serial.println("Scan repeatedly for 10-15 seconds.");
  Serial.println("If both stay zero, scanner UART is not present on these pins.");
}

void loop() {
  static uint32_t lastA = 0;
  static uint32_t lastB = 0;

  delay(1000);

  noInterrupts();
  uint32_t a = edgeA;
  uint32_t b = edgeB;
  interrupts();

  uint32_t da = a - lastA;
  uint32_t db = b - lastB;
  lastA = a;
  lastB = b;

  Serial.print("[1s] IO20_edges=");
  Serial.print(da);
  Serial.print(" IO19_edges=");
  Serial.print(db);
  Serial.print("  IO20_now=");
  Serial.print(digitalRead(PIN_A));
  Serial.print(" IO19_now=");
  Serial.println(digitalRead(PIN_B));

  if (da > 10 || db > 10) {
    digitalWrite(LED_PIN, HIGH);
    delay(60);
    digitalWrite(LED_PIN, LOW);
  }
}
