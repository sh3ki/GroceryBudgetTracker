# Grocery Budget Tracker - Complete Build Guide

## Project Overview
A shopping cart-mounted budget tracking system with barcode scanning, touchscreen interface, and budget alert system.

---

## 1. COMPLETE HARDWARE COMPONENTS LIST (PHILIPPINES-SPECIFIC)

### Core Components (What You Have)
- ✅ **ELECROW ESP32-S3 7" Display (800×480)**
  - Dual-core processor (240MHz)
  - Built-in WiFi & Bluetooth
  - Touch screen capability
  - TF card slot
  - **Available Ports (from your images):**
    - **UART0** (4-pin JST-XH connector)
    - **GPIO_D** (4-pin JST-XH connector)
    - **I2C** (4-pin JST-XH connector)
    - **SPK** (Speaker - 2-pin JST-XH connector)
    - **BAT** (Battery - 2-pin JST-XH connector)
    - **BOOT** & **RESET** buttons

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
**Small Speaker 8Ω 1-2W**
- **Specs:** 8 Ohm impedance, 1-2 Watt power
- **Size:** 28-40mm diameter
- **Connection:** 2-pin JST-XH connector (will connect to SPK port)
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

#### E. ✅ CONNECTORS & CABLES (CRITICAL - SPECIFIC TO YOUR BOARD)

**JST-XH Connector Cables (Pre-made)**
Your ELECROW board uses JST-XH 2.54mm pitch connectors. You need:

1. **JST-XH 4-Pin Cable** (for UART0, GPIO_D, I2C)
   - Quantity: 2 pieces minimum
   - Length: 15-20cm with female connector one end, bare wires other end
   - **Where to Buy:**
     - Shopee: "JST XH 4 pin cable 2.54mm" - ₱15-30 each
     - Lazada: Search same
   - **Price:** ₱30-60 (for 2pcs)

2. **JST-XH 2-Pin Cable** (for SPK speaker port)
   - Quantity: 1 piece
   - **Where to Buy:**
     - Shopee: "JST XH 2 pin cable" - ₱10-20
   - **Price:** ₱10-20

3. **JST-XH 2-Pin Cable** (for BAT battery port - if using battery)
   - Quantity: 1 piece
   - **Price:** ₱10-20

**Dupont Jumper Wires (for connections to GM65 & LED)**
- **Type:** Female-to-Female
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

#### G. ✅ OPTIONAL COMPONENTS

**SD Card / TF Card** (for product database storage)
- **Size:** 8GB or 16GB (Class 10)
- **Where to Buy:** Any computer/phone shop
- **Price:** ₱150-250

**Mounting Bracket** (3D Printed or Metal)
- **Option 1:** 3D print custom bracket
  - 3D Printing Services in Manila: ₱200-400
- **Option 2:** Buy universal phone/tablet car mount
  - Where: Shopee, Lazada - "tablet car mount" - ₱250-500

---

### 📋 COMPLETE PHILIPPINES SHOPPING LIST WITH PRICES

| Item | Qty | Unit Price (₱) | Total (₱) | Where to Buy |
|------|-----|---------------|-----------|--------------|
| **GM65 Barcode Scanner** | 1 | 800-1,200 | 800-1,200 | Shopee/Lazada/CircuitRocks |
| **Power Bank 10,000mAh** | 1 | 400-800 | 400-800 | SM/Shopee (Romoss/Xiaomi) |
| **Small Speaker 8Ω 1W** | 1 | 20-50 | 20-50 | e-Gizmo/Shopee |
| **Red LED (5mm)** | 2 | 3-5 | 6-10 | e-Gizmo/Raon |
| **220Ω Resistor** | 2 | 1-2 | 2-4 | e-Gizmo/Raon |
| **JST-XH 4-pin Cable** | 2 | 15-30 | 30-60 | Shopee |
| **JST-XH 2-pin Cable** | 2 | 10-20 | 20-40 | Shopee |
| **Dupont Wires (40pcs)** | 1 set | 50-120 | 50-120 | e-Gizmo/Shopee |
| **USB-C Cable** | 1 | 80-150 | 80-150 | Any electronics store |
| **Velcro Straps** | 3 | 20-35 | 60-105 | Ace Hardware/Shopee |
| **Zip Ties (50pcs)** | 1 pack | 30-50 | 30-50 | Hardware store |
| **Heat Shrink Tube** | 1 pack | 30-60 | 30-60 | e-Gizmo/Shopee |
| **Foam Padding** | 1 sheet | 25-40 | 25-40 | National Bookstore |
| **SD Card 16GB** | 1 | 150-250 | 150-250 | Computer shops |
| | | **TOTAL:** | **₱1,703-2,939** | |

**Recommended Budget: ₱2,500 (includes some spare components)**

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

## 2. WIRING DIAGRAM & CONNECTIONS

### 🔌 EASY CONNECTION GUIDE (Using Your Board's Ports)

Your ELECROW ESP32-S3 display has dedicated ports with JST-XH connectors - this makes wiring MUCH easier!

---

### Port Pinout Reference

**UART0 Port (4-pin JST-XH):**
```
Pin 1: 5V   (Power output)
Pin 2: GND  (Ground)
Pin 3: TX   (Transmit from ESP32)
Pin 4: RX   (Receive to ESP32)
```

**GPIO_D Port (4-pin JST-XH):**
```
Pin 1: 5V   (Power output)
Pin 2: GND  (Ground)
Pin 3: GPIO (Digital I/O pin)
Pin 4: GPIO (Digital I/O pin)
```

**I2C Port (4-pin JST-XH):**
```
Pin 1: 5V   (Power output)
Pin 2: GND  (Ground)
Pin 3: SDA  (I2C Data)
Pin 4: SCL  (I2C Clock)
```

**SPK Port (2-pin JST-XH):**
```
Pin 1: SPK+ (Speaker positive)
Pin 2: SPK- (Speaker negative/ground)
```

**BAT Port (2-pin JST-XH):**
```
Pin 1: BAT+ (Battery positive, 3.7-12V input)
Pin 2: BAT- (Battery ground)
```

---

### 🔧 COMPLETE WIRING CONNECTIONS

#### Connection 1: GM65 Barcode Scanner → UART0 Port

**Using JST-XH 4-pin cable:**
```
ELECROW UART0 Port          GM65 Scanner Module
┌─────────────────┐        ┌──────────────────┐
│ Pin 1: 5V       │───────→│ VCC (Red wire)   │
│ Pin 2: GND      │───────→│ GND (Black wire) │
│ Pin 3: TX (out) │───────→│ RX  (White wire) │
│ Pin 4: RX (in)  │───────→│ TX  (Yellow wire)│
└─────────────────┘        └──────────────────┘
```

**Step-by-step:**
1. Get JST-XH 4-pin cable with bare wire ends
2. Connect GM65 wires to cable:
   - Red (5V) → Pin 1
   - Black (GND) → Pin 2
   - Yellow/TX → Pin 4 (RX on ELECROW)
   - White/RX → Pin 3 (TX on ELECROW)
3. Plug JST connector into UART0 port
4. Use heat shrink tubing on connections

---

#### Connection 2: Speaker → SPK Port

**Direct connection - EASIEST!**
```
ELECROW SPK Port            8Ω Speaker
┌─────────────────┐        ┌──────────────┐
│ Pin 1: SPK+     │───────→│ + (Red wire) │
│ Pin 2: SPK-/GND │───────→│ - (Black wire)│
└─────────────────┘        └──────────────┘
```

**Step-by-step:**
1. Get JST-XH 2-pin cable OR cut speaker wires and add JST connector
2. Solder speaker wires to JST cable:
   - Speaker + → Pin 1
   - Speaker - → Pin 2
3. Plug directly into SPK port
4. **No amplifier needed** - board has built-in amp!

---

#### Connection 3: Red LED Indicator → GPIO_D Port

**Using one GPIO pin:**
```
ELECROW GPIO_D Port         LED Circuit
┌─────────────────┐        
│ Pin 1: 5V       │        (Not used)
│ Pin 2: GND      │───┐    
│ Pin 3: GPIO     │───┼───→ [220Ω Resistor] → [LED Anode +]
│ Pin 4: GPIO     │   │                              ↓
└─────────────────┘   └─────────────────── [LED Cathode -]
```

**Step-by-step:**
1. Get JST-XH 4-pin cable with bare wire ends
2. Solder LED circuit:
   - LED Anode (+, longer leg) → 220Ω resistor → GPIO_D Pin 3 wire
   - LED Cathode (-, shorter leg) → GPIO_D Pin 2 (GND) wire
3. Use heat shrink on LED legs
4. Plug JST connector into GPIO_D port

**Alternative - Direct Dupont Connection:**
- Use female Dupont wires
- Connect to GPIO_D port pins
- More flexible but less secure

---

#### Connection 4: Power Supply → BAT Port

**Option A: Power Bank (EASIEST - RECOMMENDED)**
```
Power Bank USB              ESP32 Display
┌──────────────┐           ┌──────────────┐
│ USB-A Output │──[Cable]─→│ USB-C Port   │
└──────────────┘           └──────────────┘
```
**Just plug USB-C cable from power bank to display - DONE!**

---

**Option B: 18650 Battery Pack with BMS**
```
Battery Pack (7.4V)         ELECROW BAT Port
┌──────────────────┐       ┌─────────────────┐
│ 2S BMS Output +  │──────→│ Pin 1: BAT+     │
│ 2S BMS Output -  │──────→│ Pin 2: BAT-/GND │
└──────────────────┘       └─────────────────┘
```

**With Buck Converter (if battery is 7.4V):**
```
Battery Pack (7.4V) → [Buck Converter] → BAT Port (5V)
                      (adjust to 5V)
```

**Step-by-step for Battery:**
1. Connect 2× 18650 batteries in series (7.4V) in holder with BMS
2. Connect BMS output to Mini360 buck converter input
3. Adjust buck converter output to exactly 5V using screwdriver
4. Solder JST-XH 2-pin cable to buck converter output
5. Plug JST connector into BAT port

---

### 📸 VISUAL WIRING DIAGRAM

```
                    ┌─────────────────────────────────────────┐
                    │   ELECROW ESP32-S3 7" Display (Back)   │
                    │                                          │
       [BARCODE]    │  ┌────┐  ┌────┐  ┌────┐  ┌──┐  ┌──┐   │
       GM65         │  │I2C │  │GPIO│  │UART│  │SPK│ │BAT│   │
          │         │  └────┘  │ _D │  │ 0  │  └──┘  └──┘   │
          └────────────────────┤    ├──┤    │                │
                    │          └────┘  └────┘                │
                    │            │                            │
                    │            └──────────┐                 │
                    │                       │                 │
                    └───────────────────────┼─────────────────┘
                                           │
                                      [LED Circuit]
                                      
┌────────────────────────────────────────────────────────────┐
│                    COMPONENT CONNECTIONS                    │
├────────────────────────────────────────────────────────────┤
│                                                             │
│  GM65 Scanner ───[JST 4-pin]──→ UART0 Port                │
│                                                             │
│  Speaker ───[JST 2-pin]──→ SPK Port                        │
│                                                             │
│  LED + Resistor ───[JST 4-pin]──→ GPIO_D Port              │
│                                                             │
│  Power Bank ───[USB-C Cable]──→ USB-C Port                 │
│  OR                                                         │
│  Battery Pack ───[JST 2-pin]──→ BAT Port                   │
│                                                             │
└────────────────────────────────────────────────────────────┘
```

---

### 🎯 SIMPLIFIED CONNECTION SUMMARY

| Component | Connects To | Cable Type | Notes |
|-----------|-------------|------------|-------|
| **GM65 Barcode Scanner** | UART0 Port | JST-XH 4-pin | TX↔RX crossover |
| **Speaker (8Ω)** | SPK Port | JST-XH 2-pin | Direct plug, no amp needed |
| **Red LED + Resistor** | GPIO_D Port | JST-XH 4-pin | Use Pin 3 (GPIO) + Pin 2 (GND) |
| **Power Bank** | USB-C Port | USB-C Cable | Easiest power option |
| **Battery (if used)** | BAT Port | JST-XH 2-pin | Must be 5V (or use buck converter) |
| **SD Card** | Built-in TF slot | - | For product database |

---

## 3. SOFTWARE & DEVELOPMENT STACK

### Development Environment Setup

#### Required Software:
1. **Arduino IDE 2.x** (Easiest option)
   - Download: https://www.arduino.cc/en/software
   - Install ESP32 board support

2. **Alternative: PlatformIO** (More professional)
   - VS Code extension
   - Better library management

3. **USB-to-Serial Drivers:**
   - CP210x or CH340 drivers (depends on your board)

#### Required Libraries:

```cpp
// Install via Arduino Library Manager:
1. TFT_eSPI          // Display driver (v2.5.0+)
2. lvgl              // UI framework (v8.3.0+)
3. XPT2046_Touchscreen // Touch controller
4. ArduinoJson       // Data parsing (v6.x)
```

**Library Configuration:**
- **TFT_eSPI:** Must configure `User_Setup.h` for your display
- **LVGL:** Configure `lv_conf.h` for ESP32-S3

---

## 4. USER INTERFACE DESIGN (LVGL)

### Screen Layout Design

#### Main Screen Structure:
```
┌─────────────────────────────────────────────┐
│  GROCERY BUDGET TRACKER         🔋 85%      │ ← Status Bar
├─────────────────────────────────────────────┤
│                                             │
│  Budget: ₱2,500.00    Spent: ₱1,847.50    │ ← Budget Display
│  Remaining: ₱652.50 (26%)                  │
│  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░                │ ← Progress Bar
│                                             │
├─────────────────────────────────────────────┤
│  CART ITEMS (12)                  [+ ADD]   │ ← Items Header
├─────────────────────────────────────────────┤
│  🛒 Item List (Scrollable)                  │
│                                             │
│  ┌─────────────────────────────────┐       │
│  │ Lucky Me Pancit Canton          │       │
│  │ ₱15.50 × 3      [↑↓] [🗑]  ₱46.50│       │
│  └─────────────────────────────────┘       │
│  ┌─────────────────────────────────┐       │
│  │ Coca-Cola 1.5L                  │       │
│  │ ₱65.00 × 2      [↑↓] [🗑]  ₱130.00│      │
│  └─────────────────────────────────┘       │
│  ┌─────────────────────────────────┐       │
│  │ Alaska Evap Milk                │       │
│  │ ₱32.75 × 4      [↑↓] [🗑]  ₱131.00│      │
│  └─────────────────────────────────┘       │
│                                             │
│                ⋮                            │
│                                             │
├─────────────────────────────────────────────┤
│  [SCAN ITEM]  [CLEAR ALL]  [CHECKOUT]      │ ← Action Buttons
└─────────────────────────────────────────────┘
```

#### Screen Flows:

**1. Welcome/Setup Screen:**
- Input budget using numeric keypad
- "Start Shopping" button

**2. Scanning Screen:**
- Active camera/scanner indicator
- "Ready to Scan" message
- Live feedback when item detected

**3. Item Detail Popup:**
- Show scanned item name & price
- Quantity selector (+ / -)
- "Add to Cart" / "Cancel" buttons

**4. Budget Alert Screen:**
- Red background flash effect
- "BUDGET REACHED!" message
- Current total vs budget
- Option to continue or checkout

**5. Checkout Summary:**
- Final item list
- Total amount
- "Complete" button → Reset to welcome screen

---

## 5. STEP-BY-STEP IMPLEMENTATION GUIDE

### PHASE 1: Hardware Assembly (2-3 hours)

#### Step 1: Prepare All Components

**Unbox and verify you have:**
- [ ] ELECROW ESP32-S3 Display
- [ ] GM65 Barcode Scanner Module
- [ ] Power Bank (10,000mAh) OR Battery pack with BMS
- [ ] Speaker (8Ω 1W)
- [ ] Red LED (5mm) + 220Ω Resistor
- [ ] JST-XH 4-pin cables (×2)
- [ ] JST-XH 2-pin cables (×2)
- [ ] Dupont jumper wires (40pc set)
- [ ] USB-C cable
- [ ] Velcro straps, zip ties
- [ ] Heat shrink tubing
- [ ] Foam padding

**Tools needed:**
- Soldering iron + solder
- Wire strippers
- Small screwdriver
- Heat gun or lighter (for heat shrink)
- Multimeter (for testing)
- Hot glue gun (optional)

---

#### Step 2: Prepare GM65 Barcode Scanner Connection

**The GM65 module typically comes with 4 wires:**
- Red: VCC (5V)
- Black: GND
- Yellow: TX (transmit from scanner)
- White or Green: RX (receive to scanner)

**Connection process:**
1. Take one JST-XH 4-pin cable with bare wire ends
2. Strip the ends if not already bare
3. Solder or crimp GM65 wires to JST cable:
   ```
   JST Pin 1 (5V)  ← Red wire (GM65 VCC)
   JST Pin 2 (GND) ← Black wire (GM65 GND)
   JST Pin 3 (TX)  ← Yellow wire (GM65 TX) - This goes to ESP32 RX!
   JST Pin 4 (RX)  ← White wire (GM65 RX) - This receives from ESP32 TX!
   ```
4. Cover each connection with heat shrink tubing
5. Test with multimeter:
   - Continuity check each wire
   - Verify no shorts between pins

**IMPORTANT:** TX from scanner goes to RX on ESP32 (Pin 4), RX from scanner goes to TX on ESP32 (Pin 3)

---

#### Step 3: Prepare Speaker Connection

**Simple 2-wire connection:**
1. Take JST-XH 2-pin cable OR use speaker's existing wires
2. If speaker has bare wires:
   - Solder JST connector to speaker wires
   - Red/+ wire → JST Pin 1 (SPK+)
   - Black/- wire → JST Pin 2 (SPK-)
3. Cover solder joints with heat shrink
4. Label as "SPEAKER"

**Test (optional):**
- You can briefly touch a 3V battery to speaker wires to hear a pop/click sound

---

#### Step 4: Build LED Alert Circuit

**Create LED circuit with resistor:**
1. Take the 220Ω resistor
2. Solder one resistor leg to LED anode (longer leg, +)
3. Cover solder joint with heat shrink
4. Take JST-XH 4-pin cable with bare ends
5. Solder connections:
   ```
   JST Pin 2 (GND) → LED Cathode (shorter leg, -)
   JST Pin 3 (GPIO) → Resistor (other end from LED)
   JST Pin 1 & 4 → Leave unconnected
   ```
6. Test with multimeter (diode mode - LED should show ~1.8-2.2V drop)

**Alternative simple method:**
- Use 2× female Dupont jumper wires
- Solder one to resistor+LED anode
- Solder one to LED cathode
- Cover with heat shrink
- Can plug directly into GPIO_D port pins

---

#### Step 5: Prepare Power System

**Option A: Power Bank (RECOMMENDED for beginners)**
1. Just get a USB-C cable!
2. Plug USB-A end into power bank
3. Plug USB-C end into ESP32 display
4. **DONE!** - Easiest power solution
5. Make sure power bank can supply 2A continuously

---

**Option B: 18650 Battery Pack (Advanced)**

**If your battery pack outputs 7.4V, you need buck converter:**

1. **Prepare battery pack:**
   - Insert 2× 18650 batteries into holder (check polarity!)
   - Verify BMS protection is working
   - Check voltage with multimeter: should read ~7.4-8.4V

2. **Connect buck converter:**
   ```
   Battery BMS → Buck Converter → BAT Port
   
   Battery + (red) ────→ Buck IN+
   Battery - (black) ──→ Buck IN-
                         ↓
   Adjust potentiometer to 5V output (use multimeter!)
                         ↓
   Buck OUT+ (red) ────→ JST Pin 1 (BAT+)
   Buck OUT- (black) ──→ JST Pin 2 (BAT-)
   ```

3. **Critical: Adjust voltage to 5V BEFORE connecting to display:**
   - Connect battery to buck converter
   - Use multimeter on output
   - Turn potentiometer screw until output reads **exactly 5.0-5.2V**
   - Test with multimeter under no load and with ~500mA load

4. **Connect JST cable:**
   - Solder JST-XH 2-pin cable to buck converter output
   - Red wire → Buck OUT+ → JST Pin 1
   - Black wire → Buck OUT- → JST Pin 2
   - Add heat shrink tubing
   - Optional: Add power switch between battery and buck converter

---

#### Step 6: Final Assembly & Connection

**Connect everything to ELECROW display:**

1. **Power first (to test):**
   - If using power bank: Plug USB-C cable
   - If using battery: Plug JST connector into BAT port
   - **Turn on display - verify it boots up!**
   - If nothing happens, check power connections

2. **Connect GM65 scanner:**
   - Plug JST 4-pin connector into **UART0 port**
   - Scanner LED should light up (may be red or green)
   - Scanner might beep once

3. **Connect speaker:**
   - Plug JST 2-pin connector into **SPK port**
   - Polarity usually doesn't matter for speakers but follow color codes

4. **Connect LED circuit:**
   - Plug JST 4-pin connector into **GPIO_D port**
   - LED won't light yet (needs software control)

5. **Insert SD card (optional):**
   - Format SD card as FAT32
   - Insert into TF card slot on display

**Cable management:**
- Use zip ties to bundle cables
- Route cables away from touch screen area
- Secure loose components with hot glue or double-sided tape
- Ensure nothing blocks the display or touch surface

---

#### Step 7: Physical Mounting Preparation

**Create cart mount:**

1. **Measure your cart handle:**
   - Typical shopping cart handle diameter: 25-30mm
   - Measure width between handle bars

2. **Prepare mounting:**
   - Cut foam padding to match display back size
   - Stick foam to back of display (protects + grip)
   - Prepare velcro straps:
     - 2 straps: horizontal (top & bottom)
     - 1 strap: vertical (optional for stability)

3. **Test fit:**
   - Position display on cart handle
   - Wrap velcro straps around handle
   - Ensure display is:
     - At comfortable viewing angle (~45-60° tilt)
     - Stable when cart moves
     - Not blocking cart use
     - Easy to touch all areas of screen

**Mounting tips:**
- Display should face shopper pushing cart
- Mount near the handle grip area
- Leave room for hands on handle
- Consider adding anti-theft lock mechanism for real deployment

---

#### Step 8: Pre-Programming Hardware Test

**Before writing code, test each component:**

**Test 1: Power & Display**
- Power on device
- Display should show backlight
- Touch screen should register touches (if firmware pre-loaded)
- ✅ If display works: Power system is good

**Test 2: Barcode Scanner**
- Connect USB-to-serial adapter to scanner (if available)
- OR use Arduino Serial Monitor after basic setup
- Scan any barcode (from product, book, etc.)
- Scanner should beep and LED should flash
- Serial output should show barcode number
- ✅ If scanner outputs data: UART connection is correct

**Test 3: Speaker**
- Will test with software (needs PWM signal)
- Can do quick test with function generator if available

**Test 4: LED**
- Temporarily connect LED+resistor to 3.3V
- LED should light up
- ✅ If LED lights: Circuit is correct

---

### ⚠️ COMMON HARDWARE ASSEMBLY MISTAKES

| Problem | Cause | Solution |
|---------|-------|----------|
| Display won't power on | Wrong voltage, bad connection | Check power with multimeter, verify 5V |
| Scanner not responding | TX/RX swapped | Swap wires on Pin 3 and Pin 4 |
| No sound from speaker | Bad connection, speaker damaged | Test speaker with battery, check solder joints |
| LED always on | Connected to 5V instead of GPIO | Verify LED is on GPIO pin, not power pin |
| Battery drains fast | Short circuit, wrong voltage | Check for shorts with multimeter |
| Display flickers | Insufficient current | Use 2A+ power supply, check cable quality |

---

### PHASE 2: Software Development (4-6 hours)

#### Step 1: Setup Arduino IDE
```bash
1. Open Arduino IDE
2. Go to: File → Preferences
3. Add ESP32 board URL:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Go to: Tools → Board → Boards Manager
5. Search "ESP32" and install "esp32 by Espressif"
6. Select Board: "ESP32S3 Dev Module"
```

#### Step 2: Install Libraries
```
Tools → Manage Libraries
Search and install:
- TFT_eSPI (by Bodmer)
- lvgl (by LVGL)
- ArduinoJson (by Benoit Blanchon)
```

#### Step 3: Configure TFT_eSPI
Navigate to library folder and edit `User_Setup.h`:
```cpp
// For ELECROW ESP32-S3 7" Display
#define ILI9488_DRIVER
#define TFT_WIDTH  800
#define TFT_HEIGHT 480
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS   9
#define TFT_DC   8
#define TFT_RST  14
```

#### Step 4: Basic Code Structure

**Main Arduino Sketch Structure:**
```cpp
// budget_tracker.ino

#include <TFT_eSPI.h>
#include <lvgl.h>
#include <HardwareSerial.h>

// Hardware definitions
#define BARCODE_RX 17
#define BARCODE_TX 18
#define BUZZER_PIN 21
#define LED_PIN 22

// LVGL display buffers
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[800 * 10];

// Serial for barcode scanner
HardwareSerial BarcodeSerial(1);

// Global variables
float budget = 0;
float totalSpent = 0;
bool budgetExceeded = false;

// Data structures
struct CartItem {
    String name;
    String barcode;
    float price;
    int quantity;
};

std::vector<CartItem> cart;

void setup() {
    Serial.begin(115200);
    BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
    
    // Initialize pins
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    // Initialize display
    initDisplay();
    
    // Initialize LVGL
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 800 * 10);
    
    // Create UI screens
    createWelcomeScreen();
}

void loop() {
    // Handle barcode scanning
    if (BarcodeSerial.available()) {
        String barcode = BarcodeSerial.readStringUntil('\n');
        handleScannedBarcode(barcode);
    }
    
    // Update LVGL
    lv_timer_handler();
    delay(5);
    
    // Check budget alert
    if (totalSpent >= budget && !budgetExceeded) {
        triggerBudgetAlert();
    }
}
```

#### Step 5: Key Functions to Implement

**A. Budget Alert System:**
```cpp
void triggerBudgetAlert() {
    budgetExceeded = true;
    
    // Flash LED
    for(int i=0; i<10; i++) {
        digitalWrite(LED_PIN, HIGH);
        playTone(BUZZER_PIN, 1000, 100); // 1kHz, 100ms
        delay(100);
        digitalWrite(LED_PIN, LOW);
        playTone(BUZZER_PIN, 800, 100); // 800Hz, 100ms
        delay(100);
    }
    
    // Show alert screen
    showBudgetAlertScreen();
}

void playTone(int pin, int frequency, int duration) {
    tone(pin, frequency, duration);
}
```

**B. Barcode Processing:**
```cpp
void handleScannedBarcode(String barcode) {
    // Query price database (can be local JSON or API)
    CartItem item = lookupProduct(barcode);
    
    if (item.price > 0) {
        // Show item confirmation dialog
        showItemDialog(item);
    } else {
        // Manual price entry
        showManualEntryDialog(barcode);
    }
}

CartItem lookupProduct(String barcode) {
    // Option 1: Local database (SD card JSON)
    // Option 2: WiFi API call to online database
    // Option 3: Manual entry fallback
    
    // Example with local JSON:
    File file = SD.open("/products.json");
    // Parse and search...
    return foundItem;
}
```

**C. UI Creation with LVGL:**
```cpp
void createWelcomeScreen() {
    lv_obj_t * screen = lv_obj_create(NULL);
    
    // Title label
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "Set Your Budget");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);
    
    // Budget input
    lv_obj_t * budgetInput = lv_textarea_create(screen);
    lv_textarea_set_placeholder_text(budgetInput, "Enter amount...");
    lv_obj_align(budgetInput, LV_ALIGN_CENTER, 0, -50);
    
    // Numeric keyboard
    lv_obj_t * kb = lv_keyboard_create(screen);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_textarea(kb, budgetInput);
    
    // Start button
    lv_obj_t * btn = lv_btn_create(screen);
    lv_obj_add_event_cb(btn, start_shopping_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_t * btnLabel = lv_label_create(btn);
    lv_label_set_text(btnLabel, "START SHOPPING");
    
    lv_scr_load(screen);
}

void createMainScreen() {
    // Similar structure for main shopping screen
    // Add: Budget bar, item list, buttons
}
```

---

### PHASE 3: Product Database Setup

#### Option A: Local SD Card Database (Recommended for offline)

**Create products.json file:**
```json
{
  "products": [
    {
      "barcode": "4800016644184",
      "name": "Lucky Me Pancit Canton Original",
      "price": 15.50,
      "category": "Noodles"
    },
    {
      "barcode": "4800888120175",
      "name": "Coca-Cola 1.5L",
      "price": 65.00,
      "category": "Beverages"
    },
    {
      "barcode": "4800024466322",
      "name": "Alaska Evaporated Milk",
      "price": 32.75,
      "category": "Dairy"
    }
  ]
}
```

**Loading code:**
```cpp
#include <ArduinoJson.h>
#include <SD.h>

bool loadProductDatabase() {
    if (!SD.begin()) {
        Serial.println("SD Card failed!");
        return false;
    }
    
    File file = SD.open("/products.json");
    if (!file) {
        Serial.println("Failed to open products.json");
        return false;
    }
    
    DynamicJsonDocument doc(32768);
    DeserializationError error = deserializeJson(doc, file);
    
    if (error) {
        Serial.println("JSON parse failed!");
        return false;
    }
    
    file.close();
    return true;
}
```

#### Option B: WiFi API Integration

```cpp
#include <WiFi.h>
#include <HTTPClient.h>

CartItem fetchProductOnline(String barcode) {
    HTTPClient http;
    String url = "https://api.yourstore.com/products/" + barcode;
    
    http.begin(url);
    int httpCode = http.GET();
    
    CartItem item;
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        
        item.name = doc["name"].as<String>();
        item.price = doc["price"].as<float>();
        item.barcode = barcode;
        item.quantity = 1;
    }
    
    http.end();
    return item;
}
```

---

### PHASE 4: Testing & Calibration (2 hours)

#### Checklist:
- [ ] Touch screen calibration
- [ ] Barcode scanner recognition test (test various barcodes)
- [ ] Audio alert volume adjustment
- [ ] LED brightness test
- [ ] Battery runtime test (minimum 3 hours)
- [ ] Cart mounting stability test
- [ ] Database lookup speed test
- [ ] Budget calculation accuracy
- [ ] UI responsiveness on rapid taps

---

## 6. MOUNTING ON SHOPPING CART

### Mounting Options:

**Option 1: Handle Mount (Recommended)**
```
Design:
- 3D print clamp bracket that wraps around cart handle
- Two-piece design with screw closure
- Rubber/foam padding inside
- Velcro strap backup
```

**Option 2: Front Basket Mount**
```
- Attach to front basket wire frame
- Use adjustable brackets
- Angled for easy viewing
```

**Option 3: Top Bar Mount**
```
- Clamp to top horizontal bar of cart
- Swivel arm for angle adjustment
```

### Mounting Considerations:
- Weight distribution
- Shock absorption (rubber pads)
- Easy removal for charging
- Theft prevention (optional lock mechanism)

---

## 7. POWER MANAGEMENT & BATTERY LIFE

### Power Optimization Code:
```cpp
// Reduce screen brightness when idle
void setBrightness(uint8_t level) {
    ledcWrite(0, level); // 0-255
}

// Sleep mode after 5 minutes idle
void checkIdleTime() {
    if (millis() - lastActivity > 300000) { // 5 min
        esp_light_sleep_start();
    }
}

// Wake on touch
void setupWakeup() {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0); // Touch interrupt
}
```

### Battery Indicator:
```cpp
float getBatteryVoltage() {
    int raw = analogRead(34); // Connect battery voltage divider to GPIO34
    float voltage = (raw / 4095.0) * 3.3 * 2; // Assuming 2:1 divider
    return voltage;
}

int getBatteryPercent() {
    float voltage = getBatteryVoltage();
    // For 2S Li-ion: 8.4V = 100%, 6.4V = 0%
    int percent = map(voltage * 10, 64, 84, 0, 100);
    return constrain(percent, 0, 100);
}
```

---

## 8. COST BREAKDOWN (PHILIPPINES PRICES)

### Detailed Cost Analysis

| Component | Quantity | Unit Price (₱) | Total (₱) | Status |
|-----------|----------|---------------|-----------|---------|
| **ELECROW ESP32-S3 7" Display** | 1 | - | Already Owned | ✅ Have |
| GM65 2D Barcode Scanner Module | 1 | 800-1,200 | 1,000 | 🛒 Buy |
| Power Bank 10,000mAh (Romoss/Xiaomi) | 1 | 400-800 | 600 | 🛒 Buy |
| Speaker 8Ω 1W (with wires) | 1 | 20-50 | 35 | 🛒 Buy |
| Red LED 5mm High Brightness | 2 | 3-5 | 8 | 🛒 Buy |
| Resistor 220Ω 1/4W | 2 | 1-2 | 3 | 🛒 Buy |
| JST-XH 4-pin Cable (15cm) | 2 | 15-30 | 50 | 🛒 Buy |
| JST-XH 2-pin Cable (15cm) | 2 | 10-20 | 30 | 🛒 Buy |
| Dupont Jumper Wire Set (40pcs F-F) | 1 set | 50-120 | 80 | 🛒 Buy |
| USB-C Cable (1-2m, good quality) | 1 | 80-150 | 120 | 🛒 Buy |
| Velcro Straps 25mm (30cm, reusable) | 3 | 20-35 | 75 | 🛒 Buy |
| Zip Ties Assorted (50-100pc pack) | 1 pack | 30-50 | 40 | 🛒 Buy |
| Heat Shrink Tubing Assorted Kit | 1 set | 30-60 | 45 | 🛒 Buy |
| Foam Padding EVA Sheet | 1 | 25-40 | 30 | 🛒 Buy |
| SD Card 16GB Class 10 | 1 | 150-250 | 200 | 🛒 Buy |
| Solder Wire (if needed) | 1 roll | 80-120 | - | Optional |
| **SUBTOTAL (Required)** | - | - | **₱2,316** | |
| **Contingency (10%)** | - | - | **₱232** | |
| **TOTAL PROJECT COST** | - | - | **₱2,548** | |

---

### Budget Options

**🟢 MINIMUM BUILD (Basic Functionality) - ₱1,800**
- GM65 Scanner: ₱1,000
- Power Bank basic: ₱400
- Speaker: ₱30
- LED + Resistor: ₱10
- Cables (JST): ₱80
- USB Cable: ₱100
- Basic mounting (zip ties): ₱40
- Heat shrink: ₱40
- Foam: ₱30
- Dupont wires: ₱70

**🟡 RECOMMENDED BUILD (Good Quality) - ₱2,500**
- All above + better quality components
- Quality power bank: ₱600
- SD card included: ₱200
- Velcro straps: ₱75
- Better cables and mounting

**🔵 PREMIUM BUILD (Professional Grade) - ₱3,500**
- All above + extras
- Larger power bank (20,000mAh): ₱1,000
- Custom 3D printed bracket: ₱300
- Extra sensors/components
- Better weather protection

---

### Where to Shop (Metro Manila)

**One-Stop Shopping Strategy:**

**Day 1: Electronics Components**
- **e-Gizmo Mechatronics** (Taft Ave, Manila)
  - Buy: Speaker, LED, resistors, cables, heat shrink
  - Budget: ₱300-400
  
**Day 2: Main Components**
- **Shopee/Lazada Online Order:**
  - GM65 Scanner (₱1,000)
  - JST cables (₱80)
  - Dupont wires (₱80)
  - Total: ₱1,160
  - Delivery: 2-5 days

**Day 3: Power & Accessories**
- **Any SM/Robinsons Mall:**
  - Power Bank Romoss 10,000mAh (₱600)
  - USB-C cable (₱120)
- **Hardware Store (Ace/Handyman):**
  - Velcro straps (₱75)
  - Zip ties (₱40)
- **National Bookstore:**
  - Foam padding (₱30)

**Day 4: Storage**
- **Computer Shop/Octagon:**
  - SD Card 16GB (₱200)

**Total Shopping Time: 1-2 hours in-store + 2-5 days online delivery**

---

### Alternative: Battery Option Cost

**If using battery instead of power bank:**

| Component | Price (₱) | Notes |
|-----------|----------|-------|
| 18650 Battery (2pcs) | 300-400 | 2600mAh per cell |
| 2S Battery Holder with BMS | 100-150 | With protection |
| Mini360 Buck Converter | 40-60 | Adjustable output |
| Power Switch | 15-25 | Toggle switch |
| JST Battery Connector | 20-30 | |
| **Battery Option Total** | **₱475-665** | |
| vs Power Bank | ₱400-600 | Simpler! |

**Recommendation:** Stick with power bank - easier, safer, cheaper!

---

## 9. ADVANCED FEATURES (Optional Enhancements)

### A. WiFi Sync & Cloud Backup
- Upload shopping history to cloud
- Sync product database updates
- Family budget sharing

### B. Receipt Printing
- Add thermal printer module (e.g., CSN-A2)
- Print shopping summary at checkout

### C. Loyalty Card Integration
- NFC reader module (PN532)
- Store & apply loyalty cards

### D. Multi-Language Support
- LVGL supports UTF-8
- Add language selector in settings

### E. Nutritional Information
- Show calories, allergens per item
- Healthy choice suggestions

### F. Voice Alerts
- Add DFPlayer Mini MP3 module
- Custom voice messages ("Budget reached!", "Item added")

---

## 10. TROUBLESHOOTING GUIDE

### Common Issues:

**1. Barcode Scanner Not Reading:**
- Check UART connections (TX/RX swapped?)
- Verify baud rate (usually 9600)
- Test scanner with USB-to-Serial adapter first
- Ensure proper lighting (scanner needs contrast)

**2. Touch Screen Not Responding:**
- Calibrate touch using TFT_eSPI calibration sketch
- Check if touch controller is enabled in setup
- Verify SPI connections

**3. Display Flickering:**
- Insufficient power supply (check voltage)
- Loose connections
- Bad USB cable

**4. Battery Drains Quickly:**
- Reduce screen brightness
- Implement sleep mode
- Check for short circuits
- Battery might be old/damaged

**5. Items Not Found in Database:**
- Implement manual entry fallback
- Update product database
- Check barcode format (EAN-13, UPC-A, etc.)

---

## 11. FINAL DEPLOYMENT CHECKLIST

Before deploying to actual shopping cart:

- [ ] Full 3-hour battery runtime test passed
- [ ] 100+ barcode scans without crash
- [ ] Touch screen responsive in all areas
- [ ] Alert system triggers correctly at budget
- [ ] Physical mounting secure (shake test)
- [ ] Weatherproofing (if used outdoors)
- [ ] Product database loaded with 100+ items
- [ ] User testing with real shoppers
- [ ] Backup/charging plan established
- [ ] User manual created for customers

---

## 12. DEVELOPMENT TIMELINE

**Week 1:** Hardware assembly & testing
**Week 2:** Basic software (display, touch, scanner)
**Week 3:** UI development with LVGL
**Week 4:** Database integration & testing
**Week 5:** Alert system & polish
**Week 6:** Physical mounting & deployment testing

**Total Estimated Time:** 40-60 hours for complete build

---

## 13. RESOURCES & REFERENCES

### Official Documentation:
- ESP32-S3 Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- LVGL Documentation: https://docs.lvgl.io/
- TFT_eSPI Library: https://github.com/Bodmer/TFT_eSPI

### Helpful Tutorials:
- ELECROW Display Setup: Check manufacturer website
- LVGL UI Design: https://docs.lvgl.io/master/get-started/quick-overview.html
- ESP32 Barcode Scanner: Search "ESP32 UART barcode scanner"

### Community Support:
- ELECROW Forum
- ESP32 Forum: esp32.com
- Arduino Forum
- Reddit: r/esp32

---

## CONCLUSION

You now have everything needed to build your grocery budget tracker! The key steps are:

1. **Get the GM65 barcode scanner** (most critical additional component)
2. **Choose power solution** (power bank is easiest)
3. **Assemble hardware** following pin diagrams
4. **Set up Arduino IDE** with ESP32 support
5. **Implement UI with LVGL** (start simple, add features)
6. **Create product database** (JSON on SD card)
7. **Test thoroughly** before cart deployment
8. **Design & 3D print mounting bracket**

**Start with a minimal viable product:**
- Basic budget input
- Single-item scanning
- Simple total display
- Basic alert

Then gradually add:
- Full item list with editing
- Better UI design
- Database integration
- Advanced features

Good luck with your build! 🛒🔧
