// Minimal LED test - NO scanner involved
// This will confirm if your ESP32 works properly

static const int LED_PIN = 38;

void setup() {
   Serial.begin(115200);
   delay(1000);
   
   Serial.println("\n========================================");
   Serial.println("  LED ONLY TEST");
   Serial.println("========================================\n");
   
   pinMode(LED_PIN, OUTPUT);
   Serial.println("[OK] LED initialized on GPIO 38");
   Serial.println("[OK] Setup complete - LED will blink forever");
}

void loop() {
   digitalWrite(LED_PIN, HIGH);
   Serial.println("[LED] ON");
   delay(500);
   
   digitalWrite(LED_PIN, LOW);
   Serial.println("[LED] OFF");
   delay(500);
}
