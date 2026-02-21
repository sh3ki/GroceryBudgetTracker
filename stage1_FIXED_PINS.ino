// stage1_FIXED_PINS.ino
// Hardware test with SAFE GPIO pins
// NO SD CARD VERSION

#include <Arduino.h>

// === SAFE PIN DEFINITIONS ===
static const int LED_PIN = 38;      // LED on GPIO 38
static const int BARCODE_RX = 20;   // GM65 TX → ESP32 GPIO 20 (I2C SDA pin)
static const int BARCODE_TX = 19;   // ESP32 GPIO 19 → GM65 RX (I2C SCL pin)

// === UART SETUP ===
HardwareSerial BarcodeSerial(2);   // UART2 remapped to GPIO 20/19

void setup() {
   // Initialize Serial Monitor
   Serial.begin(115200);
   delay(1000);
   
   Serial.println("\n\n========================================");
   Serial.println("  GROCERY BUDGET TRACKER - STAGE 1");
   Serial.println("  FIXED VERSION - I2C Header UART Pins");
   Serial.println("========================================");
   Serial.println("  Using GPIO 20/19 (I2C header)");
   Serial.println("  NOTE: Blank screen is NORMAL!");
   Serial.println("========================================\n");
   
   // Initialize LED pin
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);
   Serial.println("[OK] LED pin initialized (GPIO 38)");
   
   // Initialize barcode scanner UART on SAFE pins
   BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
   while (BarcodeSerial.available()) {
      BarcodeSerial.read();
   }
   Serial.println("[OK] Barcode scanner UART2 initialized");
   Serial.println("      Using I2C header pins: RX=GPIO20, TX=GPIO19");
   Serial.println("      Baud rate: 9600");
   
   // Test sequence: Blink LED 3 times
   Serial.println("\n[TEST] Starting LED test (3 blinks)...");
   for (int i = 0; i < 3; i++) {
      Serial.print("  Blink ");
      Serial.print(i + 1);
      Serial.println("/3");
      
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      
      digitalWrite(LED_PIN, LOW);
      delay(200);
   }
   
   Serial.println("[OK] Hardware test complete!\n");
   Serial.println("========================================");
   Serial.println("READY! Connect GM65 and scan barcode...");
   Serial.println("========================================\n");
   Serial.println("WIRING FOR GM65:");
   Serial.println("  Power Bank → Modified USB → GM65 (5V+GND only)");
   Serial.println("  GM65 UART TX → Display I2C IO20_SDA");
   Serial.println("  GM65 UART RX → Display I2C IO19_SCL");
   Serial.println("  GM65 UART GND → Display GND");
   Serial.println("========================================\n");
}

void loop() {
   // Check if barcode scanner sent any data
   if (BarcodeSerial.available()) {
      String barcode = "";
      
      // Read all available characters
      while (BarcodeSerial.available()) {
         char c = BarcodeSerial.read();
         if (c >= 32 && c <= 126 && c != '\r' && c != '\n') {
            barcode += c;
         }
         delay(2);
      }
      barcode.trim();
      
      // If we got a barcode, print it
      if (barcode.length() >= 3) {
         Serial.println("\n========================================");
         Serial.print("[SCAN] Barcode: ");
         Serial.println(barcode);
         Serial.print("[INFO] Length: ");
         Serial.print(barcode.length());
         Serial.println(" characters");
         Serial.println("========================================\n");
         
         // Blink LED to confirm scan
         digitalWrite(LED_PIN, HIGH);
         delay(100);
         digitalWrite(LED_PIN, LOW);
      }
   }
}
