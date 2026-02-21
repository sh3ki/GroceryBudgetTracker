# Grocery Budget Tracker - Complete Project Guide

This guide is the full documentation for your MVP build using the Arduino code file: **BudgetTracker.ino**.


## 1) Project Description

The Grocery Budget Tracker is an ESP32-S3 touchscreen system that helps users set a grocery budget, scan item barcodes, track spending in real time, and receive an alert when spending reaches/exceeds budget.

### Core project functions
- Input budget with on-screen numeric keyboard
- Scan barcode items through UART scanner
- Match barcode against local product list in code
- Auto-update cart and totals
- Show budget/spent/remaining and progress bar
- Trigger alert page + LED/sound alert when budget is reached
- Provide checkout summary and completion flow

### Main firmware file used
- **BudgetTracker.ino**

---

## 2) Components Used

### Required hardware (actual build)
- ELECROW ESP32-S3 7-inch RGB display (800x480)
- GT911 touch interface (used by the code)
- PCA9557 I/O expander path (used in initialization sequence)
- GM65 (or UART-compatible) barcode scanner module
- Speaker (for beeps/alerts)
- LED indicator + 220Ω resistor
- USB-C power/programming cable
- Jumper wires / proper connectors

### Software/firmware stack used
- Arduino IDE
- ESP32 Arduino core (ESP32-S3 support)
- LVGL
- LovyanGFX
- TAMC_GT911
- PCA9557 library

---

## 3) Complete Detailed Wiring Connections

> These are based on pin definitions and bus mapping in **BudgetTracker.ino**.

## 3.1 External I/O pins used by the project

| Function | Pin in Code | Notes |
|---|---:|---|
| Status LED output | GPIO 38 | `LED_PIN` |
| Display backlight control | GPIO 2 | `TFT_BL_PIN` |
| Barcode UART RX (ESP receives) | GPIO 44 | `BARCODE_RX` |
| Barcode UART TX (ESP sends) | GPIO 43 | `BARCODE_TX` |
| Speaker output (LEDC) | GPIO 42 | `SPK_PIN`, channel 0 |
| I2C SDA | GPIO 19 | Wire bus for touch/IO expander path |
| I2C SCL | GPIO 20 | Wire bus for touch/IO expander path |

## 3.2 Barcode scanner wiring (UART)

| Scanner Side | ESP32-S3 Side | Rule |
|---|---|---|
| Scanner TX | GPIO 44 (`BARCODE_RX`) | TX -> RX cross connection |
| Scanner RX | GPIO 43 (`BARCODE_TX`) | RX <- TX cross connection |
| Scanner GND | ESP GND | Must share common ground |
| Scanner VCC | Stable module-required supply | Use correct scanner voltage spec |

Notes:
- UART speed in code is `9600` (`SERIAL_8N1`).
- If scanner TX is 5V TTL and board pin is 3.3V-only logic, use level shifting/divider for scanner TX -> ESP RX.

## 3.3 LED indicator wiring

Connection order:
- `GPIO38 -> 220Ω resistor -> LED anode (+)`
- `LED cathode (-) -> GND`

Behavior in code:
- Short pulse on successful scan events.
- Flashing behavior during budget alert page.

## 3.4 Speaker wiring

- Speaker/control output is mapped to `GPIO42` using LEDC.
- Beeps are produced through `ledcWriteTone(...)` and duty control.
- Use proper load and board-compatible speaker connection method.

## 3.5 I2C wiring used by project

- SDA: `GPIO19`
- SCL: `GPIO20`
- Used by touch/expander initialization logic in setup.

## 3.6 Internal RGB display bus mapping (already defined in code)

The code configures these RGB pins for panel output:
- Data pins: `8, 3, 46, 9, 1, 5, 6, 7, 15, 16, 4, 45, 48, 47, 21, 14`
- Control/timing:
  - `HENABLE: 41`
  - `VSYNC: 40`
  - `HSYNC: 39`
  - `PCLK: 0`

Important:
- These are configured in firmware and should match your display board routing.

---

## 4) Build MVP - Continuous Steps (Hardware then Software)

## Step 1 - Prepare hardware and mount core parts
- Mount display securely.
- Place scanner module and ensure scanning angle is usable.
- Mount LED and speaker where visible/audible.
- Organize wiring with strain relief before powering.

## Step 2 - Wire all connections exactly
- Connect LED circuit to GPIO38 with resistor and GND.
- Connect scanner UART lines (TX/RX crossed) and GND.
- Connect speaker to configured output path (GPIO42-based alert output path).
- Ensure I2C lines are available for touch path (`GPIO19/20`).
- Verify common ground across modules.

## Step 3 - Power path and first power-on
- Power board via USB-C.
- Power scanner with correct module voltage.
- Confirm no overheating, no unstable resets, no loose conductors.

## Step 4 - Install software tools
- Install Arduino IDE (latest stable).
- Install ESP32 boards package in Boards Manager.
- Select an ESP32-S3 compatible target board profile matching your hardware.
- Select correct COM port.

## Step 5 - Install all required Arduino libraries
In Arduino IDE Library Manager, install:
- `lvgl`
- `LovyanGFX`
- `TAMC_GT911`
- `PCA9557`

Also ensure these framework headers resolve from ESP32 core:
- `Arduino.h`
- `Wire.h`
- `HardwareSerial` support

## Step 6 - Open and configure firmware
- Open **BudgetTracker.ino**.
- Confirm pin constants match your physical wiring:
  - `LED_PIN`, `SPK_PIN`, `BARCODE_RX`, `BARCODE_TX`, `TFT_BL_PIN`
- Confirm scanner baud in code is correct (`9600`).
- Confirm product list in `products[]` is your intended catalog.

## Step 7 - Compile and upload
- Compile sketch in Arduino IDE.
- Upload to board.
- Open Serial Monitor at `115200` for boot logs.

## Step 8 - Operate the MVP flow
- Enter budget on Welcome screen.
- Start shopping.
- Scan products to add to cart.
- Use Check Price when needed.
- Follow alert/checkout flow to completion.

---

## 5) Software Setup - Full Requirements

## 5.1 Arduino IDE requirements
- Arduino IDE 2.x (recommended)
- Internet access for board/library installation
- USB serial driver support for your board

## 5.2 Board package setup (ESP32)
- Open **Boards Manager**
- Install **ESP32 by Espressif Systems**
- Choose ESP32-S3 compatible board profile

## 5.3 Library setup (required by this code)
Install these exactly in Library Manager:
- LVGL (`lvgl.h`)
- LovyanGFX (`LovyanGFX.hpp` + esp32s3 RGB panel classes)
- TAMC_GT911 (`TAMC_GT911.h`)
- PCA9557 (`PCA9557.h`)

## 5.4 Required code modules used in the sketch
- Display/UI:
  - LVGL display and input drivers
  - LovyanGFX RGB flush path
- Input:
  - GT911 touch driver
  - UART barcode reader (`HardwareSerial(2)`)
- Alerts:
  - LED GPIO control
  - Speaker beep via LEDC
- Data logic:
  - Product catalog + cart computation
  - Screen routing and modal handling

## 5.5 Recommended Arduino IDE settings
- Use board profile that supports PSRAM and RGB panel target
- Keep serial monitor at `115200`
- Ensure USB cable supports data (not charge-only)

---

## 6) User Interface Specification (Exactly Matching Current Code)

This section documents the UI as currently implemented in **BudgetTracker.ino**.

## 6.1 Welcome Screen
Elements shown:
- Title: `GROCERY BUDGET TRACKER`
- Label: `Input budget:`
- Budget text area (single-line, centered)
- Button: `Start Shopping`
- On-screen numeric keyboard at bottom

Keyboard layout in code:
- Row 1: `.`, `0`, `Backspace`
- Row 2: `7`, `8`, `9`
- Row 3: `4`, `5`, `6`
- Row 4: `1`, `2`, `3`

Behavior:
- Input is sanitized to numeric format with max 2 decimals.
- Start only proceeds if parsed budget is > 0.

## 6.2 Scanning Screen
Elements shown:
- Title: `Scanning Screen`
- Labels:
  - `Budget: PHP ...`
  - `Spent: PHP ...`
  - `Remaining: PHP ...`
- Budget progress bar
- Status label (default `Ready to scan`)
- Cart list area with dynamic rows
- Buttons:
  - `Checkout`
  - `Check Price`
  - `Back`

Cart row controls:
- `-` (decrease qty)
- `+` (increase qty)
- `Del` (remove item)

## 6.3 Check Price Modal (from Scanning screen)
Elements shown:
- Title: `Check Price`
- Hint: `Scan product to check their price`
- Result label default:
  - `Name: -`
  - `Price: -`
- Button: `Close`

Behavior:
- Scanned barcode updates result label with name/price.
- Unknown barcode shows not found.

## 6.4 Alert Screen
Elements shown:
- Title: `BUDGET REACHED!`
- Label: `Budget`
- Dynamic budget value
- Label: `Spent`
- Dynamic spent value
- Buttons:
  - `Continue`
  - `Checkout`

Behavior:
- This page appears when `spent >= budget` and budget is set.
- LED and alert beep pattern are active while on this screen.

## 6.5 Checkout Screen
Elements shown:
- Title: `Checkout Summary`
- Table columns:
  - `Product Name`
  - `Price`
  - `Qty`
  - `Total`
- Budget and total labels at bottom
- Buttons:
  - `Complete`
  - `Back`

Behavior:
- Complete resets cart/budget and shows thank-you modal.

## 6.6 Thank-You Modal
Elements shown:
- Message: `Thank you for using Grocery Budget Tracker`

Behavior:
- Plays short ascending tone sequence.
- Auto-closes and returns to Welcome screen.

---

## 7) Process and Flow of the Project

## 7.1 Runtime screen flow
1. Boot -> Welcome
2. Welcome -> Scanning (after valid budget)
3. Scanning -> Alert (when budget reached)
4. Alert -> Scanning (Continue) OR Checkout
5. Scanning -> Checkout (manual Checkout)
6. Checkout -> Thank-you modal -> Welcome

## 7.2 Data flow
- Scanner input line -> barcode lookup in `products[]`
- Product found -> add/update cart
- Cart update -> recalc `spent`
- Budget compare -> alert decision
- UI refresh updates labels, list, bar, checkout table

---

## 8) Troubleshooting

## Build/IDE issues
### Problem: `Arduino.h` include/intellisense warning in VS Code
- Cause: editor include path configuration, not always compile failure.
- Action: compile in Arduino IDE; if successful, firmware is valid.

### Problem: missing library compile errors
- Action: install required libraries listed in this guide.
- Ensure correct library names and include headers.

## Hardware/runtime issues
### Problem: scanner does not add products
- Check UART cross wiring (`TX->RX`, `RX->TX`)
- Check scanner baud (`9600`) and scanner output mode
- Ensure common ground with ESP32

### Problem: always “Unknown barcode”
- Barcode not present in `products[]`
- Add barcode entry in **BudgetTracker.ino** and re-upload

### Problem: no touch response
- Verify I2C lines (`GPIO19/20`) and touch init
- Confirm touch library installed correctly

### Problem: no beep
- Check speaker wiring/path on configured pin
- Ensure `ENABLE_SPEAKER_ALERT` is enabled in code

### Problem: LED not lighting
- Check resistor and polarity
- Confirm LED is connected to `GPIO38` and GND

### Problem: random resets
- Power instability or short circuits
- Improve power source/current and wiring integrity

---

## 9) Safety Precautions

- Always power off before rewiring.
- Never short 5V/3.3V to GND.
- Use a resistor for LED current limiting.
- Keep exposed contacts insulated.
- Secure wires to prevent movement stress.
- Use correct logic-level protection for UART if scanner TX is 5V TTL.
- Keep device dry and away from conductive debris.
- Avoid overdriving speaker outputs with incompatible loads.

---

## 10) Notes for Maintaining the Project

- Main firmware source of truth: **BudgetTracker.ino**
- Update `products[]` when product catalog changes.
- Keep pin definitions synchronized with actual hardware wiring.
- Any UI change in code should be reflected in this guide’s UI section.
