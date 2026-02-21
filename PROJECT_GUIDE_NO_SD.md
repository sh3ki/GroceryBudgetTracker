# Grocery Budget Tracker - Complete Build Guide (NO SD CARD VERSION)

## Project Overview
A shopping cart-mounted budget tracking system with barcode scanning, touchscreen interface, and budget alert system.

**This version stores products directly in code (no SD card needed) - perfect for beginners!**

---

## 1. COMPLETE HARDWARE COMPONENTS LIST (PHILIPPINES-SPECIFIC)

### Core Components (What You Have)
- ✅ **ELECROW ESP32-S3 7" Display (800×480)**
  - Dual-core processor (240MHz)
  - Built-in WiFi & Bluetooth
  - Touch screen capability
  - TF card slot (NOT NEEDED for this guide)
  - **Available Ports (from your images):**
    - **UART0** (4-pin JST-XH connector)
    - **GPIO_D** (4-pin JST-XH connector)
    - **I2C** (4-pin JST-XH connector)
    - **SPK** (Speaker - 2-pin JST-XH connector)
    - **BAT** (Battery - 2-pin JST-XH connector)
    - **BOOT** & **RESET** buttons

### ✅ YOUR ACTUAL PARTS STATUS (UPDATED)

Based on your checklist, this is your current hardware set:

- ✅ ELECROW ESP32-S3 7" Display (800×480)
- ✅ GM65 Barcode Scanner
- ✅ Small Speaker 8Ω 0.5W
- ✅ Red LED (5mm)
- ✅ 220Ω Resistor
- ✅ USB-C Cable
- ✅ Jumper Wires
- ❌ JST-XH cables (you don't have these - will use jumper wires)
- ❌ SD Card (NOT NEEDED for this guide)

**Important:** Because you do not have JST-XH cables, this guide uses direct jumper wire connections.

---

### 🛒 SHOPPING LIST - ALL COMPONENTS (Philippines)

#### A. ✅ BARCODE SCANNER MODULE
**GM65 2D Barcode Scanner Module**
- **Quantity:** 1 piece
- **Features:** Reads 1D/2D barcodes + QR codes
- **Voltage:** 5V DC
- **Interface:** UART (TX/RX)
- **Where to Buy (Philippines):**
  - Shopee PH: Search "GM65 barcode scanner module" - ₱800-1,200
  - Lazada PH: Search "GM66 2D QR code scanner UART"
  - **Recommended Shop:** TechDesign Philippines, CircuitRocks PH
- **Price Range:** ₱800-1,200

---

#### B. ✅ POWER SYSTEM

**Option 1: Power Bank (EASIEST - RECOMMENDED)**
- **Type:** Standard USB Power Bank with 2A+ output
- **Capacity:** 10,000-20,000 mAh
- **Requirements:** 
  - Must have 5V/2A output
  - Micro USB or USB-C input for charging
- **Where to Buy:**
  - Any mall (SM, Robinsons) electronics section
  - Shopee/Lazada: "Romoss 10000mAh" or "Xiaomi Power Bank"
- **Price:** ₱400-800
- **Connection:** Use USB-C cable to connect to ESP32 display

**Option 2: Lithium Battery with BMS (More Professional)**
- **Battery Type:** 3.7V 18650 Li-ion (2S configuration = 7.4V)
- **With Protection:** 2S BMS (Battery Management System) built-in
- **Capacity:** 5200mAh (2600mAh per cell × 2)
- **Where to Buy:**
  - **e-Gizmo Mechatronics** (Taft Ave, Manila): ₱450-600
  - **Alexan Store** (Raon, Manila): ₱400-550
  - **Shopee PH:** Search "18650 battery holder 2S BMS" - ₱350-500
- **Price:** ₱400-600 (includes battery holder + BMS)

**DC-DC Buck Converter (if using battery option)**
- **Model:** Mini 360 DC-DC Buck Converter or LM2596
- **Input:** 4.5-12V
- **Output:** Adjustable to 5V (3A max)
- **Where to Buy:**
  - e-Gizmo: ₱50-80
  - Shopee: Search "Mini 360 buck converter" - ₱35-60
- **Price:** ₱35-80

---

#### C. ✅ SPEAKER (Audio Alert System)
**Small Speaker 8Ω 0.5-1W**
- **Specs:** 8 Ohm impedance, 0.5-1 Watt power
- **Size:** 28-40mm diameter
- **Connection:** Direct wire to SPK port (or jumper wires)
- **Where to Buy:**
  - **e-Gizmo:** 8Ω 1W speaker - ₱25-40
  - **Deeco Tech** (Gilmore, QC): ₱30-50
  - **Shopee:** Search "8 ohm 1w speaker" - ₱20-45
- **Price:** ₱20-50

**Note:** The ELECROW board likely has built-in audio amplifier on SPK port, so NO separate amplifier needed!

---

#### D. ✅ LED ALERT INDICATOR
**Red LED + Resistor**
- **LED:** 5mm or 10mm Red LED (high brightness)
  - Quantity: 1-2 pieces
  - Voltage: 2-2.2V
  - Current: 20mA
  - **Price:** ₱3-5 each
  
- **Resistor:** 220Ω (1/4W)
  - Quantity: 1-2 pieces
  - **Price:** ₱1-2 each

- **Where to Buy:**
  - **e-Gizmo:** Full component kits available
  - **Alexan/Raon:** Electronics component stores
  - **Shopee:** Search "5mm red LED high brightness" & "220 ohm resistor"
- **Total Price:** ₱5-10

---

#### E. ✅ CONNECTORS & CABLES

**Dupont Jumper Wires (for all connections)**
- **Type:** Female-to-Female (primary)
- **Type:** Male-to-Female (backup)
- **Quantity:** 40-piece set (assorted 10cm, 20cm)
- **Where to Buy:**
  - e-Gizmo: ₱80-120 per set
  - Shopee: "dupont wire 40pcs female to female" - ₱50-80
- **Price:** ₱50-120

**USB-C Cable** (for programming & power bank connection)
- **Type:** USB-A to USB-C
- **Length:** 1-2 meters
- **Where to Buy:** Any electronics store, mall
- **Price:** ₱80-150

---

#### F. ✅ MOUNTING & ASSEMBLY MATERIALS

**Velcro Straps (Reusable)**
- **Size:** 25mm × 30cm or adjustable
- **Quantity:** 2-3 pieces
- **Where to Buy:**
  - Ace Hardware, Handyman
  - Shopee: "velcro strap adjustable"
- **Price:** ₱50-100

**Zip Ties / Cable Ties**
- **Size:** 15cm-20cm (assorted)
- **Quantity:** Pack of 50-100
- **Where to Buy:** Hardware store, Shopee
- **Price:** ₱30-50

**Foam Padding / EVA Foam**
- **Use:** Cushion between device and cart handle
- **Size:** 1 sheet (can cut to size)
- **Where to Buy:** National Bookstore, craft stores
- **Price:** ₱25-40

**Heat Shrink Tubing (Assorted)**
- **Sizes:** 2mm, 3mm, 5mm
- **Quantity:** Assorted pack
- **Where to Buy:**
  - e-Gizmo: ₱40-60
  - Shopee: "heat shrink tube assorted"
- **Price:** ₱30-60

---

### 📋 COMPLETE PHILIPPINES SHOPPING LIST WITH PRICES (NO SD CARD)

| Item | Qty | Unit Price (₱) | Total (₱) | Where to Buy |
|------|-----|---------------|-----------|--------------|
| **GM65 Barcode Scanner** | 1 | 800-1,200 | 800-1,200 | Shopee/Lazada/CircuitRocks |
| **Power Bank 10,000mAh** | 1 | 400-800 | 400-800 | SM/Shopee (Romoss/Xiaomi) |
| **Small Speaker 8Ω 0.5W** | 1 | 20-50 | 20-50 | e-Gizmo/Shopee |
| **Red LED (5mm)** | 2 | 3-5 | 6-10 | e-Gizmo/Raon |
| **220Ω Resistor** | 2 | 1-2 | 2-4 | e-Gizmo/Raon |
| **Dupont Wires (40pcs)** | 1 set | 50-120 | 50-120 | e-Gizmo/Shopee |
| **USB-C Cable** | 1 | 80-150 | 80-150 | Any electronics store |
| **Velcro Straps** | 3 | 20-35 | 60-105 | Ace Hardware/Shopee |
| **Zip Ties (50pcs)** | 1 pack | 30-50 | 30-50 | Hardware store |
| **Heat Shrink Tube** | 1 pack | 30-60 | 30-60 | e-Gizmo/Shopee |
| **Foam Padding** | 1 sheet | 25-40 | 25-40 | National Bookstore |
| | | **TOTAL:** | **₱1,424-2,589** | |

**Recommended Budget: ₱2,000 (includes some spare components)**

**💰 Cost Savings vs SD Card Version: ₱150-250 (no SD card needed!)**

---

### 🏪 RECOMMENDED PHILIPPINE STORES

**Physical Stores in Metro Manila:**
1. **e-Gizmo Mechatronics Central**
   - Address: 2071 Taft Avenue, Malate, Manila
   - Best for: All electronic components, modules, cables
   - Contact: (02) 8-400-4701

2. **Raon Electronics (Alexan, CDR King area)**
   - Location: C.M. Recto Avenue, Manila
   - Best for: Components, LEDs, resistors, cables

3. **Deeco Tech (Gilmore)**
   - Address: Gilmore IT Center, Quezon City
   - Best for: Development boards, modules

4. **CircuitRocks PH** (Online + Physical)
   - Website: circuitrocks.com
   - Best for: Premium modules, ESP32 accessories

**Online Stores:**
- **Shopee Philippines** (Search terms provided above)
- **Lazada Philippines**
- **TechDesign PH** (Shopee seller)
- **PH Electronics Hub** (Lazada seller)

---

## 2. BEGINNER PATH (NO SD CARD, NO JST CONNECTORS)

This is the step-by-step guide built for your exact parts and beginner level.

**Why No SD Card is Better for Learning:**
- ✅ No formatting issues
- ✅ Faster product lookups (no file I/O delays)
- ✅ Simpler code (easier to understand)
- ✅ Fewer hardware failure points
- ✅ One less thing to troubleshoot
- ✅ Can store 50-100 products easily in flash memory

**Limitation:** You can store around 50-100 products in code instead of thousands on SD card. For a grocery shopping trip, 50-100 products is usually enough!

---

### 2.1 Goal for this build (MVP first)

You will build this in 3 safe stages:

1. **Hardware smoke test** (LED + speaker + scanner serial output)
2. **Budget logic with hardcoded products** (serial UI first)
3. **Touchscreen/LVGL UI integration** (same logic, nicer interface)

This path is easier than jumping directly into full LVGL from day 1.

---

### 2.2 Connector reality check (No JST available)

Since JST-XH is not available, use direct jumper wiring:

**Direct Jumper Wire Method:**
- Use jumper wires that fit tightly into board sockets
- Use tape/hot glue strain relief so wires don't slip during movement
- Label each wire with tape to avoid confusion

From your board photo, use these confirmed labels:
- **UART0:** `IO44_RXD`, `IO43_TXD`, `3V3`, `GND` (**no 5V on UART0**)
- **GPIO_D:** `IO38`, `NC`, `3V3`, `GND`
- **I2C:** `IO20_SDA`, `IO19_SCL`, `3V3`, `GND`

**Recommended for GM65 data on this board:** use the **I2C header pins** (`IO20/IO19`) as remapped UART pins. Keep `UART0` free for stable USB upload/monitor.

---

### 2.3 Exact wiring map (for your current setup)

Use labels printed beside your board ports. If your board silk labels differ, always follow board labels.

#### A) GM65 Barcode Scanner → I2C Header (recommended UART remap)

| GM65 Wire | Connect to Display Port | Notes |
|-----------|-------------------------|-------|
| VCC (5V) | External 5V source | Do **not** use any 3V3 pin for GM65 power |
| GND | I2C GND | Common ground |
| TX | I2C IO20_SDA | **Cross** TX→RX |
| RX | I2C IO19_SCL | **Cross** RX←TX |

##### ✅ Final connection when BOTH are powered by one power bank

Use this exact setup with your GM65 board (has both USB and UART headers):

| Line | From | To |
|------|------|----|
| Display power | Power bank USB output | Display USB-C |
| GM65 power + | Power bank 5V | GM65 USB header `5V` *(or GM65 USB cable)* |
| GM65 power - | Power bank GND | GM65 USB header `GND` |
| UART data 1 | GM65 UART `TX` | Display I2C `IO20_SDA` |
| UART data 2 | GM65 UART `RX` | Display I2C `IO19_SCL` |
| Common ground | GM65 GND | Display I2C `GND` |

**GM65 header labels for reference:**
- **UART header:** `GND RX TX 5V`
- **USB header:** `GND D- D+ 5V`

**What to leave unconnected:**
- GM65 UART `5V` pin (if GM65 is already powered from USB side)
- USB data lines `D+` and `D-` (not needed for UART mode)

**Power method for GM65 in your setup:**
1. Power the display via USB-C as normal
2. Power GM65 from power bank USB output (use USB cable or direct wire to USB header)
3. Tie grounds together (GM65 GND ↔ display GND via I2C GND pin)

⚠️ **UART logic safety:**
- ESP32 UART pins are 3.3V logic
- If your GM65 TX is 5V TTL output, add a voltage divider/level shifter on GM65 TX → ESP32 RX

---

#### B) LED + 220Ω resistor → GPIO_D

Circuit order:

`GPIO pin -> 220Ω resistor -> LED anode (+ long leg)`

`LED cathode (- short leg) -> GND`

Use `GPIO_D IO38` as signal and `GND` as return.

**Step-by-step wiring:**
1. Take a female-to-female jumper wire (any color, recommend RED)
2. Connect one end to GPIO_D `IO38` pin socket
3. Solder 220Ω resistor to the other end of wire
4. Solder LED anode (long leg) to resistor
5. Take another jumper wire (recommend BLACK)
6. Connect one end to GPIO_D `GND` pin socket
7. Solder LED cathode (short leg) to this wire
8. Cover all solder joints with heat shrink tubing
9. Test polarity: swap LED if it doesn't light up

---

#### C) Speaker 8Ω 0.5W → SPK

| Speaker | SPK Port |
|---------|---------|
| + wire | SPK+ |
| - wire | SPK- |

**Step-by-step wiring:**
1. Identify speaker wires (usually red = +, black = -)
2. Strip 5mm insulation from both wires
3. Connect speaker + wire to SPK+ port pin (use jumper wire or direct insert)
4. Connect speaker - wire to SPK- port pin
5. Secure with tape or hot glue

0.5W speaker is fine for testing. Keep volume moderate in code.

**Note:** If you can't identify SPK port pinout easily, leave speaker disconnected for Stage 1-2 testing. You'll still get LED alerts.

---

#### D) Power

- Use **USB-C cable** from power bank to display USB-C port
- Use a stable 5V/2A power bank (2A minimum recommended)
- **For GM65:** Connect USB cable from power bank to GM65 USB port OR use jumper wires to GM65 USB header pins

---

#### E) NO SD CARD NEEDED!

Skip all SD card setup. Your products will be stored in code!

**Advantages:**
- ✅ No formatting issues
- ✅ No file I/O errors
- ✅ Faster lookup speed
- ✅ One less thing to troubleshoot

**How to add products:**
You'll edit the Arduino code to add product barcodes, names, and prices directly. See Stage 2 code for details.

---

### 2.4 Pre-power safety checklist

Before powering on:

- [ ] No exposed wire strands touching each other
- [ ] Scanner TX/RX are crossed correctly (TX→RX, RX→TX)
- [ ] GM65 VCC comes from stable external 5V (not UART0 3V3)
- [ ] GM65 UART 5V pin is NOT connected if USB side already powers GM65
- [ ] GM65 USB D+ / D- are not connected (UART mode only)
- [ ] LED has 220Ω resistor in series (not parallel)
- [ ] GND is shared between ESP32 and GM65
- [ ] Jumper wires are firm, not wiggly (add tape/glue if loose)
- [ ] Power bank is fully charged
- [ ] No SD card inserted (not needed for this guide)

---

### 2.5 Software setup (beginner-safe)

#### Step 1: Install Arduino IDE

1. Download **Arduino IDE 2.x** from: https://www.arduino.cc/en/software
2. Install on your Windows PC
3. Launch Arduino IDE

---

#### Step 2: Add ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Find **"Additional boards manager URLs"** field
4. Paste this URL:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
5. Click **OK**
6. Go to **Tools → Board → Boards Manager**
7. Search for **"esp32"**
8. Install **"esp32 by Espressif Systems"** (latest version)
9. Wait for installation to complete

---

#### Step 3: Select Your Board

1. Connect ESP32 display to PC via USB-C cable
2. Go to **Tools → Board → esp32**
3. Select **"ESP32S3 Dev Module"**
4. Go to **Tools → Port**
5. Select the COM port for your board (e.g., COM3, COM5)
   - If no port appears, install CH340 or CP210x drivers

---

#### Step 4: Configure Board Settings

Set these in **Tools** menu:
- **USB CDC On Boot:** Enabled
- **CPU Frequency:** 240MHz (WiFi/BT)
- **Flash Size:** 4MB (or match your board)
- **Partition Scheme:** Default 4MB with spiffs
- **PSRAM:** OPI PSRAM (if your board has it)
- **Upload Speed:** 921600

---

#### Step 5: Install Libraries (OPTIONAL for Stage 1-2)

For Stage 1 and Stage 2, you **DON'T NEED** any libraries!

For Stage 3 (LVGL touchscreen UI), install these later:
1. Go to **Tools → Manage Libraries**
2. Search and install:
   - `TFT_eSPI` (for display)
   - `lvgl` (for UI widgets)

**For now, skip this step and proceed to Stage 1 code!**

---

### 2.6 Code Stage 1 - Hardware smoke test

**Goal:** Verify all hardware works (LED, speaker, barcode scanner).

**What this code does:**
- Blinks LED 3 times on startup
- Beeps speaker 3 times (if enabled)
- Listens for barcode scanner data
- Prints barcode to Serial Monitor when scanned

---

#### Step 1: Create New Sketch

1. Open Arduino IDE
2. Go to **File → New Sketch**
3. Delete all default code
4. Copy and paste the code below

---

#### Step 2: Stage 1 Code

```cpp
// stage1_hardware_test_no_sd.ino
// Hardware smoke test for ESP32-S3 grocery budget tracker
// NO SD CARD VERSION

#include <Arduino.h>

// === PIN DEFINITIONS (based on your ELECROW board labels) ===
static const int LED_PIN = 38;     // GPIO_D IO38
static const int BARCODE_RX = 20;  // I2C IO20_SDA (used as UART RX)
static const int BARCODE_TX = 19;  // I2C IO19_SCL (used as UART TX)

// === SPEAKER CONFIGURATION ===
// Set to 1 to enable speaker test, 0 to disable
#define ENABLE_SPEAKER_TEST 0

static const int SPK_PIN = 3;       // SPK GPIO pin (adjust if needed)
static const int SPK_CHANNEL = 0;   // PWM channel for speaker

// === UART SETUP ===
HardwareSerial BarcodeSerial(2);  // UART2 for GM65 scanner (UART1 conflicts with display)

// === BEEP FUNCTION ===
void beep(int frequency, int duration_ms) {
#if ENABLE_SPEAKER_TEST
   ledcSetup(SPK_CHANNEL, frequency, 8);  // 8-bit resolution
   ledcAttachPin(SPK_PIN, SPK_CHANNEL);
   ledcWrite(SPK_CHANNEL, 128);  // 50% duty cycle
   delay(duration_ms);
   ledcWrite(SPK_CHANNEL, 0);    // Stop sound
#else
   // Speaker disabled, just delay
   (void)frequency;  // Suppress unused variable warning
   delay(duration_ms);
#endif
}

// === SETUP FUNCTION ===
void setup() {
   // Initialize Serial Monitor
   Serial.begin(115200);
   delay(1000);  // Longer delay for stability
   
   Serial.println("\n\n");
   Serial.println("========================================");
   Serial.println("  GROCERY BUDGET TRACKER - STAGE 1");
   Serial.println("  Hardware Smoke Test (No SD Card)");
   Serial.println("========================================");
   Serial.println("  NOTE: Blank screen is NORMAL!");
   Serial.println("  Display works in Stage 3 only.");
   Serial.println("========================================\n");
   
   // Initialize LED pin
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);
   Serial.println("[OK] LED pin initialized (GPIO 38)");
   
   // Initialize barcode scanner UART
   // Using UART2 but remapped to GPIO 20/19 (I2C header pins)
   BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
   Serial.println("[OK] Barcode scanner UART2 initialized");
   Serial.println("      Using pins: RX=GPIO20, TX=GPIO19");
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
   Serial.println("READY! Please scan a barcode now...");
   Serial.println("========================================\n");
}

// === MAIN LOOP ===
void loop() {
   // Check if barcode scanner sent any data
   if (BarcodeSerial.available()) {
      String barcode = "";
      
      // Read all available characters
      while (BarcodeSerial.available()) {
         char c = BarcodeSerial.read();
         if (c != '\r' && c != '\n') {  // Skip newline characters
            barcode += c;
         }
         delay(2);  // Small delay for data stability
      }
      
      // If we got a barcode, print it
      if (barcode.length() > 0) {
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
```

---

#### Step 3: Upload Code

1. Click **Upload** button (right arrow icon) in Arduino IDE
2. Wait for compilation
3. Wait for upload
4. If upload fails:
   - Check USB cable connection
   - Check COM port selection
   - Press and hold BOOT button on board, then press RESET, then release BOOT
   - Try upload again

---

#### Step 4: Test Hardware

1. Open **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. You should see:
   ```
   ========================================
     GROCERY BUDGET TRACKER - STAGE 1
     Hardware Smoke Test (No SD Card)
   ========================================
   [OK] LED pin initialized (GPIO 38)
   [OK] Barcode scanner UART initialized
   ...
   Ready! Please scan a barcode now...
   ```
4. **LED should blink 3 times** (if wired correctly)
5. **Scan any barcode** with GM65 scanner
6. Barcode text should appear in Serial Monitor

---

#### Step 5: Troubleshooting Stage 1

**Problem: LED doesn't blink**
- ✅ Check LED polarity (swap if needed)
- ✅ Check 220Ω resistor is in series
- ✅ Verify GPIO 38 connection
- ✅ Check GND connection

**Problem: No barcode text appears**
- ✅ Check UART wiring (TX→RX crossover)
- ✅ Verify GM65 is powered (red LED on scanner should be on)
- ✅ Check GND is shared between ESP32 and GM65
- ✅ Try scanning multiple times
- ✅ Check GM65 baud rate (should be 9600)

**Problem: Upload fails**
- ✅ Check USB-C cable (use data cable, not charge-only)
- ✅ Check COM port in Tools menu
- ✅ Hold BOOT button while uploading
- ✅ Install CH340 or CP210x USB drivers

**If Stage 1 works, proceed to Stage 2!**

---

### 2.7 Code Stage 2 - Budget tracker with hardcoded products (NO SD CARD)

**Goal:** Create full budget tracking logic with product database in code.

**What this code does:**
- Stores products in flash memory (no SD card needed)
- Prompts for budget via Serial Monitor
- Scans barcodes and looks up products
- Calculates total spent
- Alerts when budget exceeded (LED + speaker)
- Shows all data in Serial Monitor

**Features:**
- ✅ Fast product lookup (no file I/O)
- ✅ Easy to add/edit products (just edit code)
- ✅ Can store 50-100 products easily
- ✅ No SD card formatting issues

---

#### Step 1: Product Database Structure

Products are stored as an array of structures in code:

```cpp
struct Product {
   const char* barcode;  // Product barcode (string)
   const char* name;     // Product name (string)
   float price;          // Product price (float)
};
```

Example products:
```cpp
const Product products[] = {
   {"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
   {"4800888120175", "Coca-Cola 1.5L", 65.00},
   {"4800024466322", "Alaska Evaporated Milk", 32.75},
};
```

---

#### Step 2: How to Add Your Own Products

**Method 1: Scan at home first**
1. Run Stage 1 code
2. Scan products you want to add
3. Copy barcodes from Serial Monitor
4. Look up prices at store or online
5. Add to products array in Stage 2 code

**Method 2: Add while shopping**
1. Take photo of product barcode
2. Type barcode into code later
3. Add product name and price

**Example workflow:**
```
Step 1: Scan product → barcode shows in Serial Monitor: 4902430123456
Step 2: Check store price → ₱42.50
Step 3: Add to code:
   {"4902430123456", "Knorr Sinigang Mix", 42.50},
```

---

#### Step 3: Stage 2 Complete Code

Create new sketch or replace Stage 1 code:

```cpp
// stage2_budget_tracker_no_sd.ino
// Complete budget tracker with hardcoded product database
// NO SD CARD VERSION

#include <Arduino.h>

// === PIN DEFINITIONS ===
static const int LED_PIN = 38;     // GPIO_D IO38
static const int BARCODE_RX = 20;  // I2C IO20_SDA (used as UART RX)
static const int BARCODE_TX = 19;  // I2C IO19_SCL (used as UART TX)

// === SPEAKER CONFIGURATION ===
#define ENABLE_SPEAKER_TEST 0
static const int SPK_PIN = 3;
static const int SPK_CHANNEL = 0;

// === UART SETUP ===
HardwareSerial BarcodeSerial(1);

// ==============================================================
// === PRODUCT DATABASE (EDIT THIS SECTION TO ADD PRODUCTS) ===
// ==============================================================

struct Product {
   const char* barcode;
   const char* name;
   float price;
};

// ADD YOUR PRODUCTS HERE!
// Format: {"BARCODE", "Product Name", PRICE},
const Product products[] = {
   // Sample Philippine grocery products
   {"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
   {"4800888120175", "Coca-Cola 1.5L", 65.00},
   {"4800024466322", "Alaska Evaporated Milk", 32.75},
   {"4800194145428", "Purefoods Corned Beef 150g", 58.00},
   {"4800092285019", "Surf Powder Detergent 630g", 125.00},
   {"4800016632259", "Lucky Me Beef Mami", 16.25},
   {"4800016644207", "Lucky Me Chicken", 15.00},
   {"4800888103024", "Sprite 1.5L", 65.00},
   {"4800024466339", "Alaska Condensed Milk", 38.50},
   {"4800194145411", "CDO Liver Spread", 35.00},
   
   // Add more products here...
   // {"YOUR_BARCODE", "Your Product Name", YOUR_PRICE},
};

// Calculate total number of products (don't edit this)
const int PRODUCT_COUNT = sizeof(products) / sizeof(Product);

// ==============================================================
// === END OF PRODUCT DATABASE ===
// ==============================================================

// === BUDGET TRACKING ===
struct CartTotals {
   float budget = 2500.0;
   float spent = 0.0;
   bool alerted = false;
} totals;

// === BEEP FUNCTION ===
void beep(int frequency, int duration_ms) {
#if ENABLE_SPEAKER_TEST
   ledcSetup(SPK_CHANNEL, frequency, 8);
   ledcAttachPin(SPK_PIN, SPK_CHANNEL);
   ledcWrite(SPK_CHANNEL, 120);
   delay(duration_ms);
   ledcWrite(SPK_CHANNEL, 0);
#else
   (void)frequency;
   delay(duration_ms);
#endif
}

// === BUDGET ALERT FUNCTION ===
void budgetAlert() {
   if (totals.alerted) return;  // Only alert once
   
   totals.alerted = true;
   Serial.println("\n╔════════════════════════════════════════╗");
   Serial.println("║  ⚠️  BUDGET LIMIT REACHED OR EXCEEDED  ║");
   Serial.println("╚════════════════════════════════════════╝\n");
   
   // Flash LED and beep 8 times
   for (int i = 0; i < 8; i++) {
      digitalWrite(LED_PIN, (i % 2) ? LOW : HIGH);
      beep((i % 2) ? 900 : 1300, 120);
      delay(80);
   }
   digitalWrite(LED_PIN, LOW);
}

// === FIND PRODUCT BY BARCODE ===
bool findProductByBarcode(const String &barcode, String &nameOut, float &priceOut) {
   for (int i = 0; i < PRODUCT_COUNT; i++) {
      if (barcode == products[i].barcode) {
         nameOut = products[i].name;
         priceOut = products[i].price;
         return true;
      }
   }
   return false;
}

// === PRINT TOTALS ===
void printTotals() {
   float remaining = totals.budget - totals.spent;
   float pct = (totals.budget > 0.01f) ? (totals.spent / totals.budget) * 100.0f : 0.0f;
   
   Serial.println("\n┌────────────────────────────────────────┐");
   Serial.print  ("│ Budget:    ₱");
   Serial.print(totals.budget, 2);
   for (int i = 0; i < (29 - String(totals.budget, 2).length()); i++) Serial.print(" ");
   Serial.println("│");
   
   Serial.print  ("│ Spent:     ₱");
   Serial.print(totals.spent, 2);
   for (int i = 0; i < (29 - String(totals.spent, 2).length()); i++) Serial.print(" ");
   Serial.println("│");
   
   Serial.print  ("│ Remaining: ₱");
   Serial.print(remaining, 2);
   for (int i = 0; i < (29 - String(remaining, 2).length()); i++) Serial.print(" ");
   Serial.println("│");
   
   Serial.print  ("│ Used:      ");
   Serial.print(pct, 1);
   Serial.print("%");
   for (int i = 0; i < (29 - String(pct, 1).length()); i++) Serial.print(" ");
   Serial.println("│");
   Serial.println("└────────────────────────────────────────┘");
   
   if (totals.spent >= totals.budget) {
      budgetAlert();
   }
}

// === READ BARCODE LINE ===
String readBarcodeLine() {
   String code = "";
   unsigned long start = millis();
   
   while (millis() - start < 250) {
      while (BarcodeSerial.available()) {
         char c = (char)BarcodeSerial.read();
         if (c == '\r' || c == '\n') {
            if (code.length() > 0) return code;
         } else {
            code += c;
         }
      }
   }
   
   code.trim();
   return code;
}

// === SETUP FUNCTION ===
void setup() {
   Serial.begin(115200);
   delay(600);
   
   Serial.println("\n\n");
   Serial.println("╔════════════════════════════════════════╗");
   Serial.println("║   GROCERY BUDGET TRACKER - STAGE 2    ║");
   Serial.println("║      Budget Logic + Product DB        ║");
   Serial.println("║         (No SD Card Version)          ║");
   Serial.println("╚════════════════════════════════════════╝\n");
   
   // Initialize LED
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);
   Serial.println("[OK] LED initialized");
   
   // Initialize barcode scanner
   BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
   Serial.println("[OK] Barcode scanner initialized");
   
   // Show database stats
   Serial.print("\n[INFO] Product database loaded: ");
   Serial.print(PRODUCT_COUNT);
   Serial.println(" products");
   Serial.println("       Products stored in flash memory (no SD card)");
   
   // Prompt for budget
   Serial.println("\n════════════════════════════════════════");
   Serial.println("Enter your budget in Serial Monitor:");
   Serial.println("(Example: 2500)");
   Serial.println("Then press ENTER");
   Serial.println("════════════════════════════════════════\n");
   
   // Wait for budget input
   while (Serial.available() == 0) {
      delay(10);
   }
   
   String input = Serial.readStringUntil('\n');
   totals.budget = input.toFloat();
   
   if (totals.budget <= 0) {
      Serial.println("[WARN] Invalid budget, using default: ₱2500.00");
      totals.budget = 2500.0;
   } else {
      Serial.print("[OK] Budget set to: ₱");
      Serial.println(totals.budget, 2);
   }
   
   // Startup beep
   beep(1200, 120);
   
   // Print initial totals
   printTotals();
   
   Serial.println("\n════════════════════════════════════════");
   Serial.println("✓ Ready! Start scanning products now...");
   Serial.println("════════════════════════════════════════\n");
}

// === MAIN LOOP ===
void loop() {
   // Read barcode from scanner
   String code = readBarcodeLine();
   if (code.length() == 0) return;
   
   // Look up product
   String name;
   float price = 0;
   
   if (findProductByBarcode(code, name, price)) {
      // Product found
      totals.spent += price;
      
      Serial.println("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
      Serial.println("┃  ✓ PRODUCT SCANNED                   ┃");
      Serial.println("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      Serial.print("  Product: ");
      Serial.println(name);
      Serial.print("  Price:   ₱");
      Serial.println(price, 2);
      Serial.print("  Barcode: ");
      Serial.println(code);
      
      beep(1500, 70);  // Success beep
      
   } else {
      // Product not found
      Serial.println("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
      Serial.println("┃  ✗ UNKNOWN BARCODE                   ┃");
      Serial.println("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      Serial.print("  Barcode: ");
      Serial.println(code);
      Serial.println("  Please add this product to database");
      
      beep(700, 180);  // Error beep
   }
   
   // Print updated totals
   printTotals();
}
```

---

#### Step 4: Upload and Test Stage 2

1. Click **Upload** button
2. Wait for upload to complete
3. Open **Serial Monitor** (baud rate: 115200)
4. You should see:
   ```
   ╔════════════════════════════════════════╗
   ║   GROCERY BUDGET TRACKER - STAGE 2    ║
   ║      Budget Logic + Product DB        ║
   ║         (No SD Card Version)          ║
   ╚════════════════════════════════════════╝
   
   [OK] LED initialized
   [OK] Barcode scanner initialized
   
   [INFO] Product database loaded: 10 products
          Products stored in flash memory (no SD card)
   
   ════════════════════════════════════════
   Enter your budget in Serial Monitor:
   (Example: 2500)
   Then press ENTER
   ════════════════════════════════════════
   ```

5. **Type your budget** in Serial Monitor input box (e.g., `2500`)
6. Press **ENTER**
7. You should see budget confirmation and totals
8. **Start scanning products!**

---

#### Step 5: Test Scenarios

**Test 1: Scan known product**
- Scan a product from the database
- Should show product name and price
- Total should update
- LED should blink once
- Short beep sound (if enabled)

**Test 2: Scan unknown product**
- Scan a product NOT in database
- Should show "UNKNOWN BARCODE"
- Total should NOT change
- Longer error beep (if enabled)

**Test 3: Exceed budget**
- Keep scanning products until total > budget
- Should trigger budget alert
- LED should flash 8 times
- Alternating beep sounds (if enabled)
- Alert message in Serial Monitor

---

#### Step 6: Add Your Own Products

To add products to the database:

1. Locate this section in code:
   ```cpp
   const Product products[] = {
      {"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
      // ... existing products ...
   };
   ```

2. Add new products at the end (before the closing `};`):
   ```cpp
   {"YOUR_BARCODE_HERE", "Your Product Name", YOUR_PRICE},
   ```

3. Example adding new products:
   ```cpp
   const Product products[] = {
      {"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
      {"4800888120175", "Coca-Cola 1.5L", 65.00},
      // ... existing products ...
      
      // NEW PRODUCTS:
      {"9999999999999", "Test Product 1", 25.00},
      {"1234567890123", "Test Product 2", 150.75},
      {"4800111222333", "Safeguard Soap", 45.50},
   };
   ```

4. **Important:** Don't forget the comma after each product!

5. Upload code again

6. Product count will automatically update

---

#### Step 7: Troubleshooting Stage 2

**Problem: All products show as "UNKNOWN BARCODE"**
- ✅ Check barcode matches exactly (including leading zeros)
- ✅ GM65 might add extra characters (CR/LF) - code handles this
- ✅ Print scanned barcode in code to debug:
  ```cpp
  Serial.print("DEBUG: Scanned barcode: [");
  Serial.print(code);
  Serial.println("]");
  ```

**Problem: Budget input doesn't work**
- ✅ Make sure Serial Monitor is set to 115200 baud
- ✅ Make sure "Newline" is selected in Serial Monitor dropdown
- ✅ Type budget number and press ENTER
- ✅ If stuck, press RESET button on board and try again

**Problem: Prices don't add up correctly**
- ✅ Check float precision in code (should be `.2f` for 2 decimals)
- ✅ Make sure product prices in database are correct
- ✅ Check for duplicate barcodes in database

**Problem: LED/speaker doesn't work with alerts**
- ✅ Verify LED wiring from Stage 1
- ✅ For speaker, set `ENABLE_SPEAKER_TEST` to `1`
- ✅ Check speaker wiring and SPK_PIN value
- ✅ Speaker may be very quiet with 0.5W - this is normal

**If Stage 2 works, proceed to Stage 3 for touch UI!**

---

### 2.8 Code Stage 3 - Touchscreen/LVGL UI integration (ADVANCED)

**Note:** Stage 3 is more advanced and requires:
- LVGL library configuration
- TFT_eSPI library configuration
- Touch driver setup
- More complex code

**Recommended approach:**
1. Get Stage 2 working perfectly first
2. Use Stage 2 system for actual shopping trips
3. Learn LVGL basics separately
4. Then migrate to Stage 3 when comfortable

**Stage 3 Strategy:**
- Keep all Stage 2 logic (product lookup, budget calculations)
- Replace Serial Monitor UI with LVGL widgets
- Add touch buttons for budget input
- Add product list display
- Add progress bar for budget
- Keep LED and speaker alerts

**We'll provide Stage 3 code if you request it after mastering Stage 2!**

---

### 2.9 Troubleshooting (Complete Guide)

#### Hardware Issues

**Problem: GM65 scanner doesn't power on**
- ✅ Check USB power connection
- ✅ Check 5V and GND wires
- ✅ Try different USB port on power bank
- ✅ Check if power bank has enough charge
- ✅ Scanner should have red LED when powered

**Problem: GM65 powers on but doesn't scan**
- ✅ Scanner needs to be configured to output data
- ✅ Try scanning a 2D QR code (GM65 setup code)
- ✅ Check GM65 datasheet for configuration
- ✅ Some scanners need button press or trigger

**Problem: Scanned barcode appears garbled in Serial Monitor**
- ✅ Check baud rate (must be 9600 for GM65)
- ✅ Check TX/RX crossover (TX→RX, RX→TX)
- ✅ Check GND connection between ESP32 and GM65
- ✅ Try different baud rates: 4800, 9600, 19200, 115200
- ✅ Add voltage divider if GM65 TX is 5V (ESP32 is 3.3V)

**Problem: LED doesn't work**
- ✅ Check LED polarity (long leg = anode = +)
- ✅ Check resistor value (should be 220Ω)
- ✅ Check resistor is in series (not parallel)
- ✅ Verify GPIO 38 connection
- ✅ Try different GPIO pin (edit code)
- ✅ Test LED with 3V coin battery to verify it works

**Problem: Speaker doesn't work or very quiet**
- ✅ 0.5W speaker is quiet by default
- ✅ Check speaker polarity (shouldn't matter but try swapping)
- ✅ Verify SPK pin number in code matches board
- ✅ Set `ENABLE_SPEAKER_TEST` to `1`
- ✅ Increase `ledcWrite()` value (max 255)
- ✅ Check if SPK port has built-in amplifier

**Problem: Random reboots or resets**
- ✅ Power supply too weak (use 5V/2A minimum)
- ✅ Loose power connections
- ✅ Short circuit somewhere
- ✅ Check for exposed wire strands touching
- ✅ Use better quality power bank

---

#### Software Issues

**Problem: Code won't compile**
- ✅ Check for missing semicolons `;`
- ✅ Check for unmatched braces `{ }`
- ✅ Check for missing quotes `" "`
- ✅ Read error messages carefully
- ✅ Copy code again (might have copy/paste error)

**Problem: Code won't upload**
- ✅ Check USB cable (must be data cable, not charge-only)
- ✅ Check COM port selection in Tools menu
- ✅ Close Serial Monitor before upload
- ✅ Press and hold BOOT button during upload
- ✅ Install CH340 or CP210x drivers
- ✅ Try different USB cable
- ✅ Try different USB port on PC

**Problem: Serial Monitor shows nothing**
- ✅ Check baud rate (must be 115200)
- ✅ Wait a few seconds after upload
- ✅ Press RESET button on board
- ✅ Check if board is powered
- ✅ Try different USB cable

**Problem: Serial Monitor shows garbage characters**
- ✅ Wrong baud rate - set to 115200
- ✅ Board might still be uploading - wait
- ✅ USB cable issue - try different cable

---

#### Product Database Issues

**Problem: Can't add more than 50-100 products**
- ✅ ESP32 has limited flash memory
- ✅ You're hitting memory limits
- ✅ Solution 1: Remove unused products
- ✅ Solution 2: Use SD card version (PROJECT_GUIDE.md)
- ✅ Solution 3: Store products in SPIFFS/LittleFS

**Problem: Products not found even with correct barcode**
- ✅ Check barcode matches exactly (case-sensitive)
- ✅ Check for leading/trailing spaces
- ✅ Check for missing/extra digits
- ✅ Some barcodes have check digits at end
- ✅ Print scanned barcode to debug:
  ```cpp
  Serial.print("Scanned: [");
  Serial.print(code);
  Serial.print("] Length: ");
  Serial.println(code.length());
  ```

**Problem: Wrong prices shown**
- ✅ Check product array for typos
- ✅ Check decimal point position
- ✅ Update prices in code if changed at store

---

#### Budget Logic Issues

**Problem: Budget input doesn't work**
- ✅ Serial Monitor must be open
- ✅ Make sure to press ENTER after typing budget
- ✅ Check "Newline" setting in Serial Monitor dropdown
- ✅ If stuck, press RESET and try again

**Problem: Total spent is wrong**
- ✅ Check if duplicate products are scanned
- ✅ Check product prices in database
- ✅ Check float rounding errors (should use `.2f`)

**Problem: Budget alert doesn't trigger**
- ✅ Check if spent >= budget logic
- ✅ Check if `totals.alerted` flag works
- ✅ Add debug prints:
  ```cpp
  Serial.print("Spent: ");
  Serial.print(totals.spent);
  Serial.print(" Budget: ");
  Serial.println(totals.budget);
  ```

---

### 2.10 Tips for Success

**Before Shopping:**
1. ✅ Test all hardware at home first
2. ✅ Add common products to database
3. ✅ Set realistic budget
4. ✅ Charge power bank fully
5. ✅ Have backup plan (pen and paper)

**During Shopping:**
1. ✅ Scan barcode as you put item in cart
2. ✅ Check Serial Monitor occasionally
3. ✅ If barcode not found, add to list for later
4. ✅ Pay attention to budget alerts

**After Shopping:**
1. ✅ Add any new products to database at home
2. ✅ Update prices if they changed
3. ✅ Review total vs actual receipt
4. ✅ Upload updated code for next trip

---

### 2.11 Next Steps After Stage 2

**Option 1: Keep using Stage 2 (Serial Monitor UI)**
- Works perfectly for grocery shopping
- Easy to debug and modify
- Can carry laptop or use phone with Serial Monitor app

**Option 2: Add WiFi logging**
- Send data to smartphone via WiFi
- Store shopping history in cloud
- Create web interface for product management

**Option 3: Upgrade to Stage 3 (LVGL touch UI)**
- Full touchscreen interface
- On-screen budget display
- Touch buttons for controls
- More polished user experience

**Option 4: Add SD card support later**
- Store thousands of products
- Log shopping history
- Export data to CSV for analysis
- Follow PROJECT_GUIDE.md for SD card version

**Option 5: Add more features**
- WiFi sync with online prices
- Barcode database auto-lookup
- Shopping list / cart comparison
- Multi-cart support for family
- Receipt photo capture
- Price history tracking

---

### 2.12 What to do next after this section

After you complete this section successfully:
- ✅ You have a working grocery budget tracker!
- ✅ You can use it for actual shopping trips
- ✅ You understand how product database works
- ✅ You can add/edit products easily
- ✅ You're ready for advanced features if desired

**Congratulations! You've built your first IoT project!** 🎉

---

## 3. WIRING DIAGRAM REFERENCE

### 📸 Complete Wiring Visual

```
┌─────────────────────────────────────────────────────────────┐
│                      POWER BANK (10,000mAh)                  │
│                                                              │
│  USB-A Port 1 ──────────────┐                               │
│  USB-A Port 2 ──────┐       │                               │
└─────────────────────┼───────┼───────────────────────────────┘
                      │       │
                      │       └──→ USB-C Cable
                      │              │
                      │              ↓
              ┌───────────────────────────────────────┐
              │                                        │
        ┌─────┤      GM65 BARCODE SCANNER             │
        │     │                                        │
        │     │  USB Header:  ●GND ●D- ●D+ ●5V       │
        │     │  UART Header: ●GND ●RX ●TX ●5V       │
        │     └────────┬──────┬──────┬────────────────┘
        │              │      │      │
        │             GND    RX     TX
        │              │      │      │
        │              ↓      ↓      ↓
        │     ┌────────────────────────────────────────────┐
        │     │     ELECROW ESP32-S3 7" DISPLAY           │
        │     │                                             │
      │     │  I2C Port (used for scanner UART data):   │
      │     │  [IO20_SDA] [IO19_SCL] [3V3] [GND]        │
        │     │      ↑          ↑              ↑            │
        │     │      └──────────┼──────────────┘            │
        │     │   (crossed)    │                            │
        └─────┼────────────────┘                            │
              │                                             │
              │  GPIO_D Port:                               │
              │  [IO38] [NC] [3V3] [GND]                   │
              │    │                  │                      │
              │    │                  │                      │
              │    │    ┌─────220Ω───┤                      │
              │    │    │             │                      │
              │    └────┴─────→ [LED] ─────→ to GND         │
              │         │(+)  (-)                            │
              │         └─ Red LED (anode to GPIO38)        │
              │                                             │
              │  SPK Port: [SPK+] [SPK-]                    │
              │              │      │                        │
              │              └──────┴──→ [8Ω Speaker]       │
              │                                             │
              │  USB-C Port: ←───── Power from Power Bank   │
              │                                             │
              └─────────────────────────────────────────────┘
```

---

### 🎯 Connection Summary Table

| From Device | Pin/Port | Wire Color | To Device | Pin/Port | Notes |
|-------------|----------|------------|-----------|----------|-------|
| Power Bank | USB-A 1 | USB cable | ESP32 Display | USB-C | Main power |
| Power Bank | USB-A 2 | USB cable | GM65 | USB port | Scanner power |
| GM65 | UART TX | Yellow | ESP32 | I2C IO20_SDA | Crossed |
| GM65 | UART RX | White | ESP32 | I2C IO19_SCL | Crossed |
| GM65 | UART GND | Black | ESP32 | I2C GND | Common ground |
| 220Ω resistor | One leg | Red | ESP32 | GPIO_D IO38 | LED signal |
| LED | Anode (+) | - | 220Ω resistor | Other leg | Long leg |
| LED | Cathode (-) | Black | ESP32 | GPIO_D GND | Short leg |
| Speaker | + wire | Red | ESP32 | SPK+ | Optional |
| Speaker | - wire | Black | ESP32 | SPK- | Optional |

---

## 4. ADVANCED FEATURES (OPTIONAL)

### 4.1 Adding WiFi Product Lookup

Instead of hardcoding products, fetch from online database:

```cpp
// Use WiFi to lookup product by barcode
#include <WiFi.h>
#include <HTTPClient.h>

bool lookupProductOnline(String barcode, String &name, float &price) {
   HTTPClient http;
   String url = "https://api.example.com/products/" + barcode;
   http.begin(url);
   int httpCode = http.GET();
   
   if (httpCode == 200) {
      String payload = http.getString();
      // Parse JSON response
      // Extract name and price
      return true;
   }
   return false;
}
```

---

### 4.2 Adding Shopping History Log

Store past shopping trips in SPIFFS:

```cpp
#include <SPIFFS.h>

void saveShoppingTrip() {
   File file = SPIFFS.open("/history.txt", FILE_APPEND);
   file.println("Date: " + String(millis()));
   file.println("Budget: " + String(totals.budget));
   file.println("Spent: " + String(totals.spent));
   file.close();
}
```

---

### 4.3 Bluetooth Connection to Phone

Send data to smartphone app via Bluetooth:

```cpp
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup() {
   SerialBT.begin("BudgetTracker");
}

void sendToPhone(String data) {
   SerialBT.println(data);
}
```

---

## 5. MOUNTING TO SHOPPING CART

### 5.1 Materials Needed
- 2-3 Velcro straps
- Foam padding
- Zip ties

### 5.2 Mounting Steps

1. **Position display** on cart handle
2. **Wrap foam padding** around handle first
3. **Use velcro straps** to secure display
4. **Route cables** along handle with zip ties
5. **Attach power bank** to cart side with velcro
6. **Mount GM65 scanner** near barcode scanning position
7. **Test stability** - shake cart gently

---

## 6. MAINTENANCE & CARE

### 6.1 Daily Use
- ✅ Charge power bank after each use
- ✅ Check all connections before shopping
- ✅ Clean display with soft cloth
- ✅ Protect from rain/water

### 6.2 Weekly
- ✅ Update product prices in database
- ✅ Add new products discovered
- ✅ Clean barcode scanner lens
- ✅ Check wire connections for looseness

### 6.3 Monthly
- ✅ Review and optimize product database
- ✅ Check for Arduino IDE updates
- ✅ Backup code to cloud/USB
- ✅ Replace worn velcro straps if needed

---

## 7. FREQUENTLY ASKED QUESTIONS (FAQ)

### Q1: How many products can I store without SD card?
**A:** Around 50-100 products depending on ESP32 flash memory and product name lengths. This is usually enough for regular grocery shopping!

### Q2: Can I add SD card support later?
**A:** Yes! See the main PROJECT_GUIDE.md for SD card version. You can migrate your product list to CSV file.

### Q3: Does this work with QR codes?
**A:** Yes! GM65 scanner reads both barcodes and QR codes.

### Q4: Can I scan multiple items quickly?
**A:** Yes, scan as fast as the scanner can read. Code handles rapid scanning.

### Q5: What if I scan the same item twice by mistake?
**A:** Current code adds it twice. For Stage 3 LVGL version, we can add undo button.

### Q6: Can I use different ESP32 board?
**A:** Yes, but you'll need to check pin mappings. This guide is specifically for ELECROW ESP32-S3 7" display.

### Q7: Can I use different barcode scanner?
**A:** Yes, any UART barcode scanner will work. Just check baud rate and wiring.

### Q8: Will this work in stores with bad lighting?
**A:** GM65 has built-in LED light for scanning in dim conditions.

### Q9: Can I add price correction feature?
**A:** Yes! For Stage 3 LVGL version, we can add keyboard for manual price entry.

### Q10: How long does power bank last?
**A:** 10,000mAh power bank should last 6-8 hours of continuous use.

---

## 8. TROUBLESHOOTING CHECKLIST

Print this and bring to store for first test:

### Pre-Shopping Checklist
- [ ] Power bank fully charged
- [ ] All connections secure
- [ ] Code uploaded successfully
- [ ] Products added to database
- [ ] Budget set correctly
- [ ] LED test passed
- [ ] Scanner test passed
- [ ] Serial Monitor working (or laptop ready)

### During Shopping Checklist
- [ ] Scanner powers on (red LED visible)
- [ ] Barcodes scanning successfully
- [ ] Products found in database
- [ ] Prices adding up correctly
- [ ] Budget alerts working
- [ ] No random reboots

### Post-Shopping Checklist
- [ ] Compare total with actual receipt
- [ ] Note any missing products
- [ ] Note any price differences
- [ ] Add new products to database at home
- [ ] Charge power bank for next use

---

## 9. PROJECT VARIATIONS & IDEAS

### 9.1 Multi-User Support
- Add user profiles
- Track spending per family member
- Compare shopping habits

### 9.2 Price Comparison
- Store prices from different stores
- Alert if product cheaper elsewhere
- Track price trends over time

### 9.3 Shopping List Integration
- Pre-load shopping list
- Check off items as scanned
- Alert if item not on list

### 9.4 Nutritional Tracking
- Add nutrition info to products
- Track calories/nutrients
- Alert for dietary restrictions

### 9.5 Coupon/Discount Support
- Apply discounts automatically
- Track savings
- Suggest products on sale

---

## 10. RESOURCES & REFERENCES

### Online Resources
- **Arduino ESP32 Documentation:** https://docs.espressif.com/projects/arduino-esp32/
- **LVGL Documentation:** https://docs.lvgl.io/
- **GM65 Datasheet:** Search "GM65 barcode scanner datasheet"

### Philippine Communities
- **Arduino Philippines Facebook Group**
- **Philippine Robotics Community**
- **Makers Manila**

### Video Tutorials
- Search YouTube: "ESP32 barcode scanner project"
- Search YouTube: "ESP32 LVGL tutorial"
- Search YouTube: "Arduino budget tracker"

---

## 11. APPENDIX

### A. Pin Reference Table (ELECROW ESP32-S3)

| Port | Pin 1 | Pin 2 | Pin 3 | Pin 4 |
|------|-------|-------|-------|-------|
| UART0 | IO44_RXD | IO43_TXD | 3V3 | GND |
| GPIO_D | IO38 | NC | 3V3 | GND |
| I2C | IO20_SDA | IO19_SCL | 3V3 | GND |
| SPK | SPK+ | SPK- | - | - |
| BAT | BAT+ | BAT- | - | - |

### B. Common Barcode Formats
- **EAN-13:** 13 digits (most common in Philippines)
- **UPC-A:** 12 digits (US products)
- **EAN-8:** 8 digits (small products)
- **Code 128:** Variable length (warehouse)
- **QR Code:** 2D barcode (can store URLs)

### C. Sample Product List Template

```cpp
{"BARCODE", "PRODUCT NAME", PRICE},

// Instant Noodles
{"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
{"4800016632259", "Lucky Me Beef Mami", 16.25},

// Beverages
{"4800888120175", "Coca-Cola 1.5L", 65.00},
{"4800888103024", "Sprite 1.5L", 65.00},

// Dairy
{"4800024466322", "Alaska Evaporated Milk", 32.75},
{"4800024466339", "Alaska Condensed Milk", 38.50},

// Canned Goods
{"4800194145428", "Purefoods Corned Beef 150g", 58.00},
{"4800194145411", "CDO Liver Spread", 35.00},

// Cleaning Products
{"4800092285019", "Surf Powder Detergent 630g", 125.00},
```

---

## 12. VERSION HISTORY

**Version 1.0 - No SD Card Edition**
- Initial release
- Hardware smoke test (Stage 1)
- Budget tracker with hardcoded products (Stage 2)
- LVGL UI plan (Stage 3 - future)
- Complete beginner guide
- Philippine-specific parts sourcing
- Troubleshooting guide

---

## 13. LICENSE & CREDITS

This project guide is provided free for educational purposes.

**Created by:** GitHub Copilot (Claude Sonnet 4.5)
**Date:** February 2026
**For:** Grocery budget tracking ESP32 project

Feel free to modify, share, and improve!

---

## 14. GETTING HELP

If you get stuck:

1. **Re-read relevant section** of this guide carefully
2. **Check troubleshooting section** for your specific issue
3. **Google the error message** (exact text from Serial Monitor)
4. **Ask in Arduino forums** with code and error details
5. **Post in Philippine maker communities** for local help
6. **Check YouTube tutorials** for visual guidance

**When asking for help, always provide:**
- [ ] Code you're using (copy/paste)
- [ ] Exact error message (screenshot)
- [ ] Hardware list
- [ ] Wiring description
- [ ] What you've tried already

---

## 15. FINAL NOTES

**Congratulations on choosing the NO SD CARD path!**

This version is:
- ✅ Simpler to build
- ✅ Easier to debug
- ✅ Faster product lookup
- ✅ Perfect for beginners
- ✅ Cheaper (no SD card cost)

**Remember:**
- Start with Stage 1 (hardware test)
- Don't skip to Stage 2 until Stage 1 works
- Add products gradually (start with 10-20)
- Test at home before using in store
- Have backup plan (pen and paper) for first trip

**You can do this!** 💪

Every maker starts with simple projects. Take your time, follow instructions carefully, and don't be afraid to make mistakes - that's how we learn!

Good luck with your build! 🎉

---

**END OF GUIDE**
