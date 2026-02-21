# Grocery Budget Tracker (ESP32-S3 + LVGL + Barcode Scanner)

A complete embedded grocery budgeting system built on an ELECROW 7-inch ESP32-S3 touch display. The project helps users set a shopping budget, scan products using a barcode scanner, track spending in real time, and get immediate budget alerts (visual + sound + LED).

---

## 1) Project Purpose and Usefulness

### What problem it solves
- Prevents overspending while shopping.
- Gives real-time remaining-budget visibility.
- Reduces manual computation by automatically summing scanned products.

### Why this is useful
- Practical for grocery trips and budgeting discipline.
- Fast interaction via barcode scanning and touch UI.
- Works as a compact, portable cart-mounted tool.

---

## 2) System Overview

### Main hardware platform
- ELECROW ESP32-S3 7" RGB display (800x480) with capacitive touch.
- GM65 (or compatible) UART barcode scanner.
- 8Ω speaker for sound alerts.
- External LED indicator for quick visual alerts.

### Main software stack
- Arduino framework (ESP32).
- LVGL UI framework.
- LovyanGFX for RGB panel driving.
- GT911 touch driver.

### Core behavior
1. User enters budget on Welcome screen.
2. Scanner adds items to cart in real-time.
3. Running totals update continuously.
4. When `spent >= budget`, app switches to Budget Alert screen.
5. User can continue shopping or proceed to checkout.

---

## 3) Features and Functional Scope

- Budget input using on-screen numeric keyboard.
- Barcode lookup from in-code product database.
- Real-time cart updates with quantity +/- and delete controls.
- Progress bar showing budget usage percentage.
- Check Price modal for lookup without adding to cart.
- Alert mode with LED blink + beep pattern.
- Checkout summary table with totals.
- Thank-you modal and action sound sequence.
- Multi-screen LVGL flow with reusable styles.

---

## 4) Hardware Requirements

## Required components
- 1x ELECROW ESP32-S3 7" display panel
- 1x GM65 barcode scanner module (UART)
- 1x 8Ω speaker (0.5W to 1W typical)
- 1x LED (5mm red recommended)
- 1x 220Ω resistor (for LED)
- Jumper wires / connector cables
- USB-C cable for power + upload
- Optional: power bank for portability

## Software requirements
- Arduino IDE (latest stable)
- ESP32 board support package
- Libraries:
  - `lvgl`
  - `LovyanGFX`
  - `TAMC_GT911`
  - `PCA9557`

---

## 5) Wiring Connections (as implemented in code)

> Source: `BudgetTracker.ino` pin constants and panel configuration.

## External connections
- LED output pin: `GPIO 38` (`LED_PIN`)
- Speaker pin: `GPIO 42` (`SPK_PIN`, LEDC channel 0)
- Barcode scanner RX (ESP receives scanner TX): `GPIO 44` (`BARCODE_RX`)
- Barcode scanner TX (ESP sends to scanner RX): `GPIO 43` (`BARCODE_TX`)
- Backlight control: `GPIO 2` (`TFT_BL_PIN`)

## I2C bus
- SDA: `GPIO 19`
- SCL: `GPIO 20`
- Used for PCA9557 and GT911 touch path in this build.

## RGB display bus mapping (internal panel wiring in sketch)
- Data pins: `GPIO 8,3,46,9,1,5,6,7,15,16,4,45,48,47,21,14`
- Timing/control pins:
  - `HENABLE: GPIO 41`
  - `VSYNC: GPIO 40`
  - `HSYNC: GPIO 39`
  - `PCLK: GPIO 0`

## Basic connection notes
- Barcode scanner is UART at 9600 baud (`SERIAL_8N1`).
- Speaker is tone-driven via `ledcWriteTone(...)`.
- LED should include series resistor (220Ω recommended).

---

## 6) UI Pages and User Flow

## Page A: Welcome Screen
Purpose:
- Accept budget value.

UI elements:
- App title
- Budget input text area
- Numeric keyboard
- Start Shopping button

User action:
- Enter budget and tap Start Shopping.

System response:
- Validates and sanitizes input.
- Initializes spending values.
- Navigates to Scanning screen.

## Page B: Scanning Screen
Purpose:
- Live shopping and cart management.

UI elements:
- Budget / spent / remaining labels
- Budget progress bar
- Scan status label
- Cart list with +/-/delete controls
- Buttons: Back, Check Price, Checkout

User actions:
- Scan barcode to add item.
- Adjust quantities.
- Remove item.
- Open Check Price modal.

System response:
- Updates totals and list in real-time.
- Triggers alert when budget reached/exceeded.

## Modal: Check Price
Purpose:
- View product name and price without adding item.

Flow:
- Tap Check Price.
- Scan code.
- Result label updates with Name + Price (or Not found).

## Page C: Budget Alert Screen
Purpose:
- Warn user that budget is reached.

UI elements:
- Budget reached title
- Budget value and spent value
- Buttons: Continue, Checkout

Behavior:
- LED flashing and alert beeps while active.
- User chooses to continue shopping or checkout.

## Page D: Checkout Summary
Purpose:
- Final review before completion.

UI elements:
- Table of products, price, qty, line total
- Budget and total labels
- Buttons: Back, Complete

Behavior:
- Complete resets cart and budget.
- Shows thank-you modal and sound sequence.

## Modal: Thank You
Purpose:
- Completion feedback after checkout.

Behavior:
- Centered thank-you message.
- Auto-closes via timer and returns to Welcome.

---

## 7) Complete System Flow (Process)

## High-level flow
1. Boot hardware and initialize subsystems.
2. Build LVGL screens/styles and load Welcome.
3. User enters budget.
4. Scanning cycle runs in main loop while on Scanning screen:
   - Read barcode
   - Resolve product
   - Update cart and totals
   - Refresh UI
5. If budget reached: switch to Alert screen.
6. User either continues scanning or goes to Checkout.
7. Complete checkout resets state and returns to Welcome.

## Runtime data flow
- Barcode string -> `findProductByBarcode(...)` -> product pointer -> cart update -> `spent` update -> UI refresh.
- `budget` and `spent` feed:
  - labels (`Budget`, `Spent`, `Remaining`)
  - progress bar percentage
  - alert decision (`spent >= budget`)

---

## 8) Software Architecture and Important Functional Blocks

## Data structures
- `Product`: barcode, name, unit price.
- `CartItem`: pointer to `Product`, quantity.
- `AppScreen`: state machine for screen navigation.

## Core state
- `budget`, `spent`, `cart[]`, `cartCount`
- Screen pointers and modal pointers.
- Timers for alerts and thank-you behavior.

## Key functional groups
- UI style/theme initialization (`initUiStyles`).
- Screen builders (`buildWelcomeScreen`, `buildScanningScreen`, `buildAlertScreen`, `buildCheckoutScreen`).
- Cart logic (`addProductToCart`, `updateCartQty`, `removeProductFromCart`).
- View refresh (`refreshScanScreen`, `refreshCheckoutScreen`).
- Barcode handling (`readBarcodeLine`, `findProductByBarcode`).
- Navigation (`switchScreen`).
- Alerting/audio (`beep`, `playAlertNotificationBeep`, thank-you beep sequence).
- Display and touch drivers (`my_disp_flush`, `my_touchpad_read`).

---

## 9) Product Database (Current)

- Product catalog is currently hardcoded in `const Product products[]`.
- The list in current code contains **29 products**.
- Barcode mismatch or unknown barcode shows `Unknown barcode` in status.

---

## 10) End-to-End Build Steps (Hardware -> Software)

## Step 1: Assemble hardware
1. Connect display module.
2. Wire barcode scanner UART TX/RX and power.
3. Wire LED + resistor to LED pin.
4. Connect speaker to speaker output.
5. Verify common ground and safe supply voltage.

## Step 2: Prepare Arduino environment
1. Install Arduino IDE.
2. Install ESP32 board package.
3. Install all required libraries.
4. Select correct ESP32-S3 board and COM port.

## Step 3: Flash firmware
1. Open `BudgetTracker.ino`.
2. Verify compilation.
3. Upload to board.
4. Open Serial Monitor at `115200` to check boot logs.

## Step 4: Functional testing
1. Confirm Welcome screen loads.
2. Enter budget and transition to Scanning screen.
3. Scan known barcode -> item appears in cart.
4. Validate totals and progress bar.
5. Trigger budget alert and confirm LED/beep behavior.
6. Finish checkout and confirm reset + thank-you flow.

---

## 11) Operating Instructions (User Guide)

1. Power on device.
2. Enter budget value.
3. Tap Start Shopping.
4. Scan products while shopping.
5. Use +/-/Del for adjustments.
6. Use Check Price when needed.
7. If alert appears, choose Continue or Checkout.
8. At checkout, review summary and tap Complete.

---

## 12) Troubleshooting

## Issue: Unknown barcode
- Cause: product not in local list.
- Fix: add barcode to `products[]` and reflash.

## Issue: No scanner input
- Check UART wiring (`GPIO 44/43`) and scanner baud (`9600`).
- Confirm scanner power and common GND.

## Issue: Touch not responding
- Verify GT911/I2C wiring and initialization logs.
- Recheck board-specific touch configuration.

## Issue: No sound
- Verify speaker connection to `SPK_PIN` path.
- Check `ENABLE_SPEAKER_ALERT` is enabled.

## Issue: Build include errors in VS Code
- Usually IntelliSense includePath configuration only.
- Validate by compiling in Arduino IDE.

---

## 13) Safety and Reliability Notes

- Use stable 5V supply with enough current.
- Avoid short circuits; insulate exposed connections.
- Do not drive LED without resistor.
- Keep wiring strain-relieved for cart movement.

---

## 14) Suggested Future Improvements

- Move product database to SD card or JSON file.
- Add persistent transaction history.
- Add item categories and analytics.
- Add battery level/telemetry display.
- Add cloud sync/API integration.

---

## 15) File/Document Map

- `README.md` (this file): complete integrated project documentation.
- `PROJECT_GUIDE.md`: extended hardware sourcing/build references.
- `UI_EXPLANATION.md`: UI design/programming explanation.
- `BudgetTracker.ino`: main firmware implementation.

---

## 16) License / Usage

For educational and prototype use. If deploying commercially, validate electrical safety, enclosure quality, and production-grade testing.
