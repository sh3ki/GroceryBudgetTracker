// stage2_budget_tracker_working.ino
// Budget tracker using confirmed working GM65 UART setup

#include <Arduino.h>

static const int LED_PIN = 38;
static const int BARCODE_RX = 20;  // GM65 TX -> IO20
static const int BARCODE_TX = 19;  // GM65 RX -> IO19

HardwareSerial BarcodeSerial(2);

struct Product {
  const char* barcode;
  const char* name;
  float price;
};

const Product products[] = {
  {"4800552888110", "Sample Product (from your test scan)", 49.50},
  {"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
  {"4800888120175", "Coca-Cola 1.5L", 65.00},
  {"4800024466322", "Alaska Evaporated Milk", 32.75},
};

const int PRODUCT_COUNT = sizeof(products) / sizeof(Product);

struct CartTotals {
  float budget = 2500.0f;
  float spent = 0.0f;
  bool alerted = false;
} totals;

bool findProduct(const String& code, String& nameOut, float& priceOut) {
  for (int i = 0; i < PRODUCT_COUNT; i++) {
    if (code == products[i].barcode) {
      nameOut = products[i].name;
      priceOut = products[i].price;
      return true;
    }
  }
  return false;
}

void printTotals() {
  float remaining = totals.budget - totals.spent;
  float pct = (totals.budget > 0.01f) ? (totals.spent / totals.budget) * 100.0f : 0.0f;

  Serial.println("\n----------------------------------------");
  Serial.print("Budget:    PHP ");
  Serial.println(totals.budget, 2);
  Serial.print("Spent:     PHP ");
  Serial.println(totals.spent, 2);
  Serial.print("Remaining: PHP ");
  Serial.println(remaining, 2);
  Serial.print("Used:      ");
  Serial.print(pct, 1);
  Serial.println("%");
  Serial.println("----------------------------------------");

  if (!totals.alerted && totals.spent >= totals.budget) {
    totals.alerted = true;
    Serial.println("[ALERT] Budget reached or exceeded!");
    for (int i = 0; i < 6; i++) {
      digitalWrite(LED_PIN, (i % 2 == 0) ? HIGH : LOW);
      delay(120);
    }
    digitalWrite(LED_PIN, LOW);
  }
}

String readBarcodeLine() {
  String code = "";
  unsigned long start = millis();

  while (millis() - start < 250) {
    while (BarcodeSerial.available()) {
      char c = (char)BarcodeSerial.read();

      if (c == '\r' || c == '\n') {
        if (code.length() > 0) {
          code.trim();
          if (code.length() >= 3) return code;
          code = "";
        }
      } else if (c >= 32 && c <= 126) {
        code += c;
      }
    }
  }

  code.trim();
  if (code.length() >= 3) return code;
  return "";
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
  while (BarcodeSerial.available()) {
    BarcodeSerial.read();
  }

  Serial.println("\n========================================");
  Serial.println("GROCERY BUDGET TRACKER - STAGE 2");
  Serial.println("Working config: RX=IO20, TX=IO19, BAUD=9600");
  Serial.println("========================================");
  Serial.print("[OK] Product database loaded: ");
  Serial.print(PRODUCT_COUNT);
  Serial.println(" items");

  Serial.println("\nEnter your budget (example: 2500) then press ENTER:");
  while (Serial.available() == 0) {
    delay(10);
  }

  String input = Serial.readStringUntil('\n');
  float parsed = input.toFloat();
  if (parsed > 0.0f) totals.budget = parsed;

  Serial.print("[OK] Budget set: PHP ");
  Serial.println(totals.budget, 2);
  printTotals();

  Serial.println("\nReady. Scan barcodes now...");
}

void loop() {
  String code = readBarcodeLine();
  if (code.length() == 0) return;

  String productName;
  float productPrice = 0.0f;

  Serial.println("\n========================================");
  Serial.print("[SCAN] Barcode: ");
  Serial.println(code);

  if (findProduct(code, productName, productPrice)) {
    totals.spent += productPrice;
    Serial.print("[FOUND] ");
    Serial.print(productName);
    Serial.print(" - PHP ");
    Serial.println(productPrice, 2);

    digitalWrite(LED_PIN, HIGH);
    delay(90);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("[UNKNOWN] Not in product database yet.");
  }

  Serial.println("========================================");
  printTotals();
}
