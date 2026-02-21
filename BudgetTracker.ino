#include <Arduino.h>
#define LV_CONF_SKIP 1
#include <lvgl.h>
#include <Wire.h>
#include <PCA9557.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#define USE_TOUCH 1
#if USE_TOUCH
#include <TAMC_GT911.h>
#endif

static const int LED_PIN = 38;
static const int TFT_BL_PIN = 2;
static const int BARCODE_RX = 44;
static const int BARCODE_TX = 43;

#define ENABLE_SPEAKER_ALERT 1
static const int SPK_PIN = 42;
static const int SPK_CHANNEL = 0;

static const int SCREEN_W = 800;
static const int SCREEN_H = 480;

class LGFX_PCLK0_MapD : public lgfx::LGFX_Device {
 public:
  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;

  LGFX_PCLK0_MapD() {
    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;

      cfg.pin_d0  = GPIO_NUM_8;
      cfg.pin_d1  = GPIO_NUM_3;
      cfg.pin_d2  = GPIO_NUM_46;
      cfg.pin_d3  = GPIO_NUM_9;
      cfg.pin_d4  = GPIO_NUM_1;
      cfg.pin_d5  = GPIO_NUM_5;
      cfg.pin_d6  = GPIO_NUM_6;
      cfg.pin_d7  = GPIO_NUM_7;
      cfg.pin_d8  = GPIO_NUM_15;
      cfg.pin_d9  = GPIO_NUM_16;
      cfg.pin_d10 = GPIO_NUM_4;
      cfg.pin_d11 = GPIO_NUM_45;
      cfg.pin_d12 = GPIO_NUM_48;
      cfg.pin_d13 = GPIO_NUM_47;
      cfg.pin_d14 = GPIO_NUM_21;
      cfg.pin_d15 = GPIO_NUM_14;

      cfg.pin_henable = GPIO_NUM_41;
      cfg.pin_vsync = GPIO_NUM_40;
      cfg.pin_hsync = GPIO_NUM_39;
      cfg.pin_pclk = GPIO_NUM_0;
      cfg.freq_write = 15000000;
      cfg.hsync_polarity = 0;
      cfg.hsync_front_porch = 40;
      cfg.hsync_pulse_width = 48;
      cfg.hsync_back_porch = 40;
      cfg.vsync_polarity = 0;
      cfg.vsync_front_porch = 1;
      cfg.vsync_pulse_width = 31;
      cfg.vsync_back_porch = 13;
      cfg.pclk_active_neg = 1;
      cfg.de_idle_high = 0;
      cfg.pclk_idle_high = 0;

      _bus_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.memory_width = SCREEN_W;
      cfg.memory_height = SCREEN_H;
      cfg.panel_width = SCREEN_W;
      cfg.panel_height = SCREEN_H;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config_detail();
      cfg.use_psram = 1;
      _panel_instance.config_detail(cfg);
    }

    _panel_instance.setBus(&_bus_instance);
    setPanel(&_panel_instance);
  }
};

LGFX_PCLK0_MapD lcd;
PCA9557 out;
#if USE_TOUCH
TAMC_GT911 ts(19, 20, -1, -1, SCREEN_W, SCREEN_H);
#endif
HardwareSerial BarcodeSerial(2);

struct Product {
  // Barcode text read from scanner (exact match key)
  const char* barcode;
  // Human-readable product name shown in UI
  const char* name;
  // Unit price used for cart and total calculations
  float price;
};

// In-memory product catalog (local/offline lookup table)
const Product products[] = {
  {"4800888602718", "Knorr Chicken", 6.00},
  {"899600141022", "Kopiko Black Single", 64.00},
  {"4800575425033", "Krem-Top", 48.00},
  {"4801668500262", "Datu Puti Bottle Soysauce", 17.00},
  {"4808647020094", "Eden Cheese", 49.00},
  {"748485801162", "555 Carne norte", 31.00},
  {"4800024556165", "DM Tomato Sauce", 80.00},
  {"4800575120303", "Alaska Classic Condensed", 56.00},
  {"748485800097", "Argentina beef loaf", 21.00},
  {"748485800448", "Argentina Meatloaf", 29.00},
  {"4807770100697", "Bingo Vanilla", 61.00},
  {"4800361388757", "BearBrand Powdered Milk", 109.00},
  {"4806018403729", "Whatta Tops Choco", 61.00},
  {"4801668603635", "Datu Puti Refill Vinegar", 32.00},
  {"480036140334", "Milo", 28.00},
  {"4807770122262", "Nissin Wafer Choco", 48.00},
  {"480166810029", "Papa Banana Catsu", 22.00},
  {"4805358373037", "Daily Queso", 45.00},
  {"4800016022210", "Great Taste White Caramel", 64.00},
  {"480009211011", "Rebisco Bravo", 50.00},
  {"4807770101502", "Nissin BreadStix", 50.00},
  {"4800249006612", "San Marino Corned Tuna", 35.00},
  {"37000216447", "Safeguard Pure White Bar Soap", 45.00},
  {"4809012193382", "GT Papaya Soap", 68.00},
  {"4902430178259", "Perla Laundry Bar White", 18.00},
  {"4800045330058", "Pride Detergent Bar Kalamansi", 25.00},
  {"4806020497341", "Speed Laundry Bar", 26.00},
  {"4902430191821", "Ariel Bar Scrubber", 15.00},
  {"35000740038", "Colgate Total Toothpaste", 118.00},
  
};

const int PRODUCT_COUNT = sizeof(products) / sizeof(Product);

struct CartItem {
  // Pointer to an item from products[]
  const Product* product;
  // Quantity currently in cart
  int qty;
};

// Runtime shopping state
CartItem cart[64];
int cartCount = 0;
float budget = 0.0f;
float spent = 0.0f;

const Product* pendingProduct = nullptr;
int pendingQty = 1;

enum class AppScreen {
  // Budget input page
  WELCOME,
  // Main shopping/cart page
  SCANNING,
  // Budget reached warning page
  ALERT,
  // Final review/checkout page
  CHECKOUT
};

AppScreen currentScreen = AppScreen::WELCOME;

void refreshScanScreen();
void switchScreen(AppScreen next);
void hideCheckPriceModal();
void hideThankYouModal();

lv_obj_t* scrWelcome = nullptr;
lv_obj_t* scrScanning = nullptr;
lv_obj_t* scrAlert = nullptr;
lv_obj_t* scrCheckout = nullptr;

lv_obj_t* taBudget = nullptr;
lv_obj_t* kbBudget = nullptr;
lv_obj_t* lblSelfTest = nullptr;

lv_obj_t* lblBudget = nullptr;
lv_obj_t* lblSpent = nullptr;
lv_obj_t* lblRemaining = nullptr;
lv_obj_t* barBudget = nullptr;
lv_obj_t* lblScanStatus = nullptr;
lv_obj_t* listCart = nullptr;

lv_obj_t* lblAlertBudgetValue = nullptr;
lv_obj_t* lblAlertSpentValue = nullptr;
lv_obj_t* lblCheckoutBudget = nullptr;
lv_obj_t* lblCheckoutTotal = nullptr;
lv_obj_t* tblCheckoutItems = nullptr;

lv_obj_t* popup = nullptr;
lv_obj_t* lblPopupName = nullptr;
lv_obj_t* lblPopupPrice = nullptr;
lv_obj_t* lblPopupQty = nullptr;

lv_obj_t* checkPriceModal = nullptr;
lv_obj_t* lblCheckPriceResult = nullptr;
lv_obj_t* thankYouModal = nullptr;

lv_timer_t* alertTimer = nullptr;
lv_timer_t* thankYouTimer = nullptr;
lv_timer_t* thankYouBeepTimer = nullptr;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[SCREEN_W * 40];
static uint16_t flushBuf[SCREEN_W * 40];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static bool scannerUartReady = false;
static bool touchInitOk = false;
static bool touchDetectedAtBoot = false;
static uint32_t scanLedPulseUntil = 0;
static bool speakerReady = false;
static uint32_t alertBeepUntil = 0;
static uint8_t alertBeepStep = 0;
static uint8_t thankYouBeepStep = 0;

static lv_style_t styleScreen;
static lv_style_t styleCard;
static lv_style_t styleTitle;
static lv_style_t styleSubtitle;
static lv_style_t styleInput;
static lv_style_t styleInputFocus;
static lv_style_t styleBtnPrimary;
static lv_style_t styleBtnPrimaryPressed;
static lv_style_t styleBtnSecondary;
static lv_style_t styleBtnSecondaryPressed;
static lv_style_t styleBtnDanger;
static lv_style_t styleBtnDangerPressed;
static lv_style_t styleList;
static lv_style_t styleRow;
static lv_style_t styleModal;
static lv_style_t styleModalText;
static lv_style_t styleStatus;
static lv_style_t styleAlertScreen;
static lv_style_t styleAlertTitle;
static lv_style_t styleTableMain;
static lv_style_t styleTableItems;
static bool uiStylesReady = false;

// Initialize all reusable LVGL styles (called once at boot)
void initUiStyles();
// Timer callback for ascending action/thank-you tones
void thankYouBeepTimerCb(lv_timer_t* t);
// Start short ascending tone sequence for action feedback
void startThankYouBeepSequence();

// Creates visual style theme used across all screens/components.
void initUiStyles() {
  if (uiStylesReady) return;

  const lv_color_t cPurple = lv_color_hex(0x7C3AED);
  const lv_color_t cPurpleDark = lv_color_hex(0x6D28D9);
  const lv_color_t cGreen = lv_color_hex(0x166534);
  const lv_color_t cGreenDark = lv_color_hex(0x14532D);
  const lv_color_t cRedPress = lv_color_hex(0xDC2626);

  lv_style_init(&styleScreen);
  lv_style_set_bg_opa(&styleScreen, LV_OPA_COVER);
  lv_style_set_bg_color(&styleScreen, lv_color_hex(0xFFFFFF));
  lv_style_set_text_color(&styleScreen, lv_color_hex(0x1F2937));

  lv_style_init(&styleCard);
  lv_style_set_bg_opa(&styleCard, LV_OPA_COVER);
  lv_style_set_bg_color(&styleCard, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleCard, 14);
  lv_style_set_border_width(&styleCard, 2);
  lv_style_set_border_color(&styleCard, cPurple);
  lv_style_set_pad_all(&styleCard, 12);
  lv_style_set_shadow_width(&styleCard, 14);
  lv_style_set_shadow_opa(&styleCard, LV_OPA_10);
  lv_style_set_shadow_color(&styleCard, lv_color_hex(0xA78BFA));
  lv_style_set_shadow_ofs_y(&styleCard, 4);

  lv_style_init(&styleTitle);
  lv_style_set_text_color(&styleTitle, lv_color_hex(0x111827));
#if LV_FONT_MONTSERRAT_28
  lv_style_set_text_font(&styleTitle, &lv_font_montserrat_28);
#elif LV_FONT_MONTSERRAT_24
  lv_style_set_text_font(&styleTitle, &lv_font_montserrat_24);
#elif LV_FONT_MONTSERRAT_22
  lv_style_set_text_font(&styleTitle, &lv_font_montserrat_22);
#elif LV_FONT_MONTSERRAT_20
  lv_style_set_text_font(&styleTitle, &lv_font_montserrat_20);
#endif

  lv_style_init(&styleSubtitle);
  lv_style_set_text_color(&styleSubtitle, lv_color_hex(0x374151));
#if LV_FONT_MONTSERRAT_20
  lv_style_set_text_font(&styleSubtitle, &lv_font_montserrat_20);
#elif LV_FONT_MONTSERRAT_18
  lv_style_set_text_font(&styleSubtitle, &lv_font_montserrat_18);
#elif LV_FONT_MONTSERRAT_16
  lv_style_set_text_font(&styleSubtitle, &lv_font_montserrat_16);
#endif

  lv_style_init(&styleInput);
  lv_style_set_bg_opa(&styleInput, LV_OPA_COVER);
  lv_style_set_bg_color(&styleInput, lv_color_hex(0xFFFFFF));
  lv_style_set_text_color(&styleInput, lv_color_hex(0x111827));
  lv_style_set_radius(&styleInput, 12);
  lv_style_set_border_width(&styleInput, 2);
  lv_style_set_border_color(&styleInput, cPurple);
  lv_style_set_pad_left(&styleInput, 14);
  lv_style_set_pad_right(&styleInput, 14);
  lv_style_set_pad_top(&styleInput, 10);
  lv_style_set_pad_bottom(&styleInput, 10);
  lv_style_set_shadow_width(&styleInput, 0);

#if LV_FONT_MONTSERRAT_28
  lv_style_set_text_font(&styleInput, &lv_font_montserrat_28);
#elif LV_FONT_MONTSERRAT_24
  lv_style_set_text_font(&styleInput, &lv_font_montserrat_24);
#elif LV_FONT_MONTSERRAT_22
  lv_style_set_text_font(&styleInput, &lv_font_montserrat_22);
#elif LV_FONT_MONTSERRAT_20
  lv_style_set_text_font(&styleInput, &lv_font_montserrat_20);
#endif

  lv_style_init(&styleInputFocus);
  lv_style_set_border_color(&styleInputFocus, cPurpleDark);
  lv_style_set_border_width(&styleInputFocus, 3);

  lv_style_init(&styleBtnPrimary);
  lv_style_set_bg_opa(&styleBtnPrimary, LV_OPA_COVER);
  lv_style_set_bg_color(&styleBtnPrimary, cPurple);
  lv_style_set_text_color(&styleBtnPrimary, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleBtnPrimary, 12);
  lv_style_set_border_width(&styleBtnPrimary, 2);
  lv_style_set_border_color(&styleBtnPrimary, cPurple);
  lv_style_set_shadow_width(&styleBtnPrimary, 12);
  lv_style_set_shadow_opa(&styleBtnPrimary, LV_OPA_10);
  lv_style_set_shadow_color(&styleBtnPrimary, lv_color_hex(0xC4B5FD));
  lv_style_set_shadow_ofs_y(&styleBtnPrimary, 3);

  lv_style_init(&styleBtnPrimaryPressed);
  lv_style_set_bg_opa(&styleBtnPrimaryPressed, LV_OPA_COVER);
  lv_style_set_bg_color(&styleBtnPrimaryPressed, cRedPress);
  lv_style_set_translate_y(&styleBtnPrimaryPressed, 1);

  lv_style_init(&styleBtnSecondaryPressed);
  lv_style_set_bg_opa(&styleBtnSecondaryPressed, LV_OPA_COVER);
  lv_style_set_bg_color(&styleBtnSecondaryPressed, cRedPress);
  lv_style_set_text_color(&styleBtnSecondaryPressed, lv_color_hex(0xFFFFFF));
  lv_style_set_translate_y(&styleBtnSecondaryPressed, 1);

  lv_style_init(&styleBtnDangerPressed);
  lv_style_set_bg_opa(&styleBtnDangerPressed, LV_OPA_COVER);
  lv_style_set_bg_color(&styleBtnDangerPressed, cRedPress);
  lv_style_set_translate_y(&styleBtnDangerPressed, 1);

  lv_style_init(&styleBtnSecondary);
  lv_style_set_bg_opa(&styleBtnSecondary, LV_OPA_COVER);
  lv_style_set_bg_color(&styleBtnSecondary, cGreen);
  lv_style_set_text_color(&styleBtnSecondary, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleBtnSecondary, 12);
  lv_style_set_border_width(&styleBtnSecondary, 2);
  lv_style_set_border_color(&styleBtnSecondary, cPurple);

  lv_style_init(&styleBtnDanger);
  lv_style_set_bg_opa(&styleBtnDanger, LV_OPA_COVER);
  lv_style_set_bg_color(&styleBtnDanger, lv_color_hex(0xDC2626));
  lv_style_set_text_color(&styleBtnDanger, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleBtnDanger, 10);
  lv_style_set_border_width(&styleBtnDanger, 2);
  lv_style_set_border_color(&styleBtnDanger, cPurple);

  lv_style_init(&styleList);
  lv_style_set_bg_opa(&styleList, LV_OPA_COVER);
  lv_style_set_bg_color(&styleList, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleList, 14);
  lv_style_set_border_width(&styleList, 2);
  lv_style_set_border_color(&styleList, cPurple);
  lv_style_set_pad_all(&styleList, 8);

  lv_style_init(&styleRow);
  lv_style_set_bg_opa(&styleRow, LV_OPA_COVER);
  lv_style_set_bg_color(&styleRow, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleRow, 10);
  lv_style_set_border_width(&styleRow, 2);
  lv_style_set_border_color(&styleRow, cPurple);

  lv_style_init(&styleModal);
  lv_style_set_bg_opa(&styleModal, LV_OPA_COVER);
  lv_style_set_bg_color(&styleModal, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleModal, 18);
  lv_style_set_border_width(&styleModal, 2);
  lv_style_set_border_color(&styleModal, cPurple);
  lv_style_set_pad_all(&styleModal, 14);
  lv_style_set_shadow_width(&styleModal, 16);
  lv_style_set_shadow_opa(&styleModal, LV_OPA_20);
  lv_style_set_shadow_color(&styleModal, lv_color_hex(0xC4B5FD));
  lv_style_set_shadow_ofs_y(&styleModal, 6);

  lv_style_init(&styleModalText);
  lv_style_set_text_color(&styleModalText, lv_color_hex(0x111827));
  lv_style_set_text_align(&styleModalText, LV_TEXT_ALIGN_CENTER);
#if LV_FONT_MONTSERRAT_28
  lv_style_set_text_font(&styleModalText, &lv_font_montserrat_28);
#elif LV_FONT_MONTSERRAT_24
  lv_style_set_text_font(&styleModalText, &lv_font_montserrat_24);
#elif LV_FONT_MONTSERRAT_22
  lv_style_set_text_font(&styleModalText, &lv_font_montserrat_22);
#elif LV_FONT_MONTSERRAT_20
  lv_style_set_text_font(&styleModalText, &lv_font_montserrat_20);
#endif

  lv_style_init(&styleStatus);
  lv_style_set_bg_opa(&styleStatus, LV_OPA_COVER);
  lv_style_set_bg_color(&styleStatus, lv_color_hex(0xFFFFFF));
  lv_style_set_text_color(&styleStatus, cGreenDark);
  lv_style_set_radius(&styleStatus, 10);
  lv_style_set_border_width(&styleStatus, 2);
  lv_style_set_border_color(&styleStatus, cPurple);
  lv_style_set_pad_left(&styleStatus, 12);
  lv_style_set_pad_right(&styleStatus, 12);
  lv_style_set_pad_top(&styleStatus, 6);
  lv_style_set_pad_bottom(&styleStatus, 6);

  lv_style_init(&styleAlertScreen);
  lv_style_set_bg_opa(&styleAlertScreen, LV_OPA_COVER);
  lv_style_set_bg_color(&styleAlertScreen, lv_color_hex(0xFFFFFF));
  lv_style_set_text_color(&styleAlertScreen, lv_color_hex(0x7F1D1D));

  lv_style_init(&styleAlertTitle);
  lv_style_set_text_color(&styleAlertTitle, lv_color_hex(0x991B1B));

  lv_style_init(&styleTableMain);
  lv_style_set_bg_opa(&styleTableMain, LV_OPA_COVER);
  lv_style_set_bg_color(&styleTableMain, lv_color_hex(0xFFFFFF));
  lv_style_set_radius(&styleTableMain, 12);
  lv_style_set_border_width(&styleTableMain, 2);
  lv_style_set_border_color(&styleTableMain, cPurple);

  lv_style_init(&styleTableItems);
  lv_style_set_bg_opa(&styleTableItems, LV_OPA_TRANSP);
  lv_style_set_text_color(&styleTableItems, lv_color_hex(0x111827));
  lv_style_set_border_width(&styleTableItems, 1);
  lv_style_set_border_color(&styleTableItems, cPurple);

  uiStylesReady = true;
}

void triggerScanLedPulse(uint32_t durationMs) {
  // Turn LED on for a short scan acknowledgement pulse.
  scanLedPulseUntil = millis() + durationMs;
  digitalWrite(LED_PIN, HIGH);
}

void updateScanLedPulse() {
  // Automatically turn off scan pulse LED when duration is elapsed.
  if (currentScreen == AppScreen::ALERT) return;
  if (scanLedPulseUntil == 0) return;

  if ((int32_t)(millis() - scanLedPulseUntil) >= 0) {
    digitalWrite(LED_PIN, LOW);
    scanLedPulseUntil = 0;
  }
}

void initSpeaker() {
  // Configure PWM/tone output for alert and action beeps.
#if ENABLE_SPEAKER_ALERT
  ledcSetup(SPK_CHANNEL, 1000, 8);
  ledcAttachPin(SPK_PIN, SPK_CHANNEL);
  ledcWrite(SPK_CHANNEL, 0);
  speakerReady = true;
#endif
}

void updateAlertBeep() {
  // Stop active beep when its requested duration expires.
#if ENABLE_SPEAKER_ALERT
  if (!speakerReady || alertBeepUntil == 0) return;
  if ((int32_t)(millis() - alertBeepUntil) >= 0) {
    ledcWrite(SPK_CHANNEL, 0);
    alertBeepUntil = 0;
  }
#endif
}

static const char* kb_num_map[] = {
  ".", "0", LV_SYMBOL_BACKSPACE, "\n",
  "7", "8", "9", "\n",
  "4", "5", "6", "\n",
  "1", "2", "3", ""
};

static const lv_btnmatrix_ctrl_t kb_num_ctrl_map[] = {
  1, 1, 1,
  1, 1, 1,
  1, 1, 1,
  1, 1, 1
};

// RGB565 channel expansion helpers.
static inline uint8_t expand5to8(uint8_t v) { return (uint8_t)((v << 3) | (v >> 2)); }
static inline uint8_t expand6to8(uint8_t v) { return (uint8_t)((v << 2) | (v >> 4)); }

// Convert LVGL 16-bit color to display color order/format used by pushImage.
static inline uint16_t toMode6RBG(uint16_t c) {
  uint8_t r5 = (uint8_t)((c >> 11) & 0x1F);
  uint8_t g6 = (uint8_t)((c >> 5) & 0x3F);
  uint8_t b5 = (uint8_t)(c & 0x1F);

  uint8_t r8 = expand5to8(r5);
  uint8_t g8 = expand6to8(g6);
  uint8_t b8 = expand5to8(b5);

  return lcd.color565(r8, g8, b8);
}

static void formatMoney(float value, char* out, size_t outSize) {
  // Format float value into "PHP x.xx" with manual cent rounding.
  int32_t cents = (int32_t)(value * 100.0f + (value >= 0 ? 0.5f : -0.5f));
  bool negative = cents < 0;
  if (negative) cents = -cents;
  int32_t whole = cents / 100;
  int32_t frac = cents % 100;

  if (negative) {
    snprintf(out, outSize, "-PHP %ld.%02ld", (long)whole, (long)frac);
  } else {
    snprintf(out, outSize, "PHP %ld.%02ld", (long)whole, (long)frac);
  }
}

static String sanitizeBudgetInput(const String& raw) {
  // Keep only digits and one decimal point (max 2 decimal places).
  String outText = "";
  bool hasDot = false;
  int decimalDigits = 0;

  for (size_t i = 0; i < raw.length(); i++) {
    char c = raw.charAt(i);
    if (c >= '0' && c <= '9') {
      if (hasDot) {
        if (decimalDigits >= 2) continue;
        decimalDigits++;
      }
      outText += c;
    } else if (c == '.' && !hasDot) {
      if (outText.length() == 0) outText = "0";
      hasDot = true;
      outText += '.';
    }
  }

  return outText;
}

void budgetInputChangedEvent(lv_event_t* e) {
  // Live-clean budget input while user types.
  lv_obj_t* ta = lv_event_get_target(e);
  String raw = lv_textarea_get_text(ta);
  String cleaned = sanitizeBudgetInput(raw);
  if (cleaned != raw) {
    lv_textarea_set_text(ta, cleaned.c_str());
    lv_textarea_set_cursor_pos(ta, LV_TEXTAREA_CURSOR_LAST);
  }
}

void beep(int frequency, int durationMs) {
  // Play one short tone at requested frequency and duration.
#if ENABLE_SPEAKER_ALERT
  if (!speakerReady) return;
  ledcWriteTone(SPK_CHANNEL, frequency);
  ledcWrite(SPK_CHANNEL, 180);
  alertBeepUntil = millis() + (uint32_t)durationMs;
#else
  (void)frequency;
  (void)durationMs;
#endif
}

void playAlertNotificationBeep() {
  // Rotating alert-tone pattern while alert screen is active.
#if ENABLE_SPEAKER_ALERT
  alertBeepStep = (uint8_t)((alertBeepStep + 1) % 4);
  int tone = 1500;
  if (alertBeepStep == 0) tone = 1400;
  else if (alertBeepStep == 1) tone = 1700;
  else if (alertBeepStep == 2) tone = 2100;
  else tone = 1800;
  beep(tone, 60);
#endif
}

void thankYouBeepTimerCb(lv_timer_t* t) {
  // Emit ascending tones step-by-step using LVGL timer.
  (void)t;
#if ENABLE_SPEAKER_ALERT
  static const int tones[] = {1400, 1750, 2200};
  static const uint8_t toneCount = sizeof(tones) / sizeof(tones[0]);

  if (thankYouBeepStep < toneCount) {
    beep(tones[thankYouBeepStep], 70);
    thankYouBeepStep++;
    return;
  }
#endif

  if (thankYouBeepTimer) {
    lv_timer_del(thankYouBeepTimer);
    thankYouBeepTimer = nullptr;
  }
}

void startThankYouBeepSequence() {
  // Restart action feedback sequence from first tone.
  if (thankYouBeepTimer) {
    lv_timer_del(thankYouBeepTimer);
    thankYouBeepTimer = nullptr;
  }
  thankYouBeepStep = 0;
#if ENABLE_SPEAKER_ALERT
  static const int tones[] = {1400, 1750, 2200};
  beep(tones[0], 70);
  thankYouBeepStep = 1;
#endif
  thankYouBeepTimer = lv_timer_create(thankYouBeepTimerCb, 90, nullptr);
}

void thankYouTimerCb(lv_timer_t* t) {
  // Auto-close thank-you modal and return to welcome screen.
  (void)t;
  hideThankYouModal();
  thankYouTimer = nullptr;
  switchScreen(AppScreen::WELCOME);
}

void showThankYouModal() {
  // Create and show centered completion modal after checkout.
  hideThankYouModal();

  thankYouModal = lv_obj_create(scrCheckout);
  lv_obj_add_style(thankYouModal, &styleModal, LV_PART_MAIN);
  lv_obj_set_size(thankYouModal, 560, 220);
  lv_obj_center(thankYouModal);
  lv_obj_clear_flag(thankYouModal, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* msg = lv_label_create(thankYouModal);
  lv_obj_add_style(msg, &styleModalText, LV_PART_MAIN);
  lv_obj_set_width(msg, 520);
  lv_obj_set_style_text_align(msg, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_label_set_long_mode(msg, LV_LABEL_LONG_WRAP);
  lv_label_set_text(msg, "Thank you for using Grocery Budget Tracker");
  lv_obj_align(msg, LV_ALIGN_CENTER, 0, 0);

  startThankYouBeepSequence();

  thankYouTimer = lv_timer_create(thankYouTimerCb, 1400, nullptr);
  lv_timer_set_repeat_count(thankYouTimer, 1);
}

const Product* findProductByBarcode(const String& code) {
  // Linear search in local product table by barcode text.
  for (int i = 0; i < PRODUCT_COUNT; i++) {
    if (code == products[i].barcode) {
      return &products[i];
    }
  }
  return nullptr;
}

void resetCart() {
  // Clear all cart lines and reset spending tracker.
  cartCount = 0;
  spent = 0.0f;
}

int findCartIndexByProduct(const Product* product) {
  // Find cart row index by product pointer, or -1 if not present.
  for (int i = 0; i < cartCount; i++) {
    if (cart[i].product == product) {
      return i;
    }
  }
  return -1;
}

void addProductToCart(const Product* product, int qty) {
  // Add quantity to existing row, or append a new row when absent.
  for (int i = 0; i < cartCount; i++) {
    if (cart[i].product == product) {
      cart[i].qty += qty;
      spent += product->price * qty;
      return;
    }
  }

  if (cartCount < 64) {
    cart[cartCount].product = product;
    cart[cartCount].qty = qty;
    cartCount++;
    spent += product->price * qty;
  }
}

void updateCartQty(const Product* product, int delta) {
  // Increase/decrease qty; remove row if resulting qty is <= 0.
  int index = findCartIndexByProduct(product);
  if (index < 0) {
    if (delta > 0) addProductToCart(product, delta);
    return;
  }

  int oldQty = cart[index].qty;
  int newQty = oldQty + delta;

  if (newQty <= 0) {
    spent -= cart[index].product->price * oldQty;
    for (int i = index; i < cartCount - 1; i++) {
      cart[i] = cart[i + 1];
    }
    cartCount--;
  } else {
    cart[index].qty = newQty;
    spent += cart[index].product->price * delta;
  }

  if (spent < 0.0f) spent = 0.0f;
}

void removeProductFromCart(const Product* product) {
  // Remove a full product row from cart and shift remaining rows.
  int index = findCartIndexByProduct(product);
  if (index < 0) return;

  spent -= cart[index].product->price * cart[index].qty;
  for (int i = index; i < cartCount - 1; i++) {
    cart[i] = cart[i + 1];
  }
  cartCount--;

  if (spent < 0.0f) spent = 0.0f;
}

void cartQtyMinusBtnEvent(lv_event_t* e) {
  // UI event: decrement item quantity from cart list row.
  const Product* product = (const Product*)lv_event_get_user_data(e);
  if (!product) return;

  updateCartQty(product, -1);
  if (lblScanStatus) {
    lv_label_set_text_fmt(lblScanStatus, "Updated: %s", product->name);
  }
  refreshScanScreen();
}

void cartQtyPlusBtnEvent(lv_event_t* e) {
  // UI event: increment item quantity from cart list row.
  const Product* product = (const Product*)lv_event_get_user_data(e);
  if (!product) return;

  updateCartQty(product, +1);
  if (lblScanStatus) {
    lv_label_set_text_fmt(lblScanStatus, "Updated: %s", product->name);
  }
  refreshScanScreen();

  if (spent >= budget && budget > 0.01f) {
    switchScreen(AppScreen::ALERT);
  }
}

void cartDeleteBtnEvent(lv_event_t* e) {
  // UI event: delete item row from cart list.
  const Product* product = (const Product*)lv_event_get_user_data(e);
  if (!product) return;

  removeProductFromCart(product);
  if (lblScanStatus) {
    lv_label_set_text_fmt(lblScanStatus, "Removed: %s", product->name);
  }
  refreshScanScreen();
}

void refreshScanScreen() {
  // Recompute and repaint scanning screen totals, bar, and cart rows.
  if (!lblBudget) return;

  float remaining = budget - spent;
  float usedPct = (budget > 0.01f) ? (spent / budget) * 100.0f : 0.0f;
  if (usedPct < 0) usedPct = 0;
  if (usedPct > 100) usedPct = 100;

  char money[24];
  char line[64];

  formatMoney(budget, money, sizeof(money));
  snprintf(line, sizeof(line), "Budget: %s", money);
  lv_label_set_text(lblBudget, line);

  formatMoney(spent, money, sizeof(money));
  snprintf(line, sizeof(line), "Spent: %s", money);
  lv_label_set_text(lblSpent, line);

  formatMoney(remaining, money, sizeof(money));
  snprintf(line, sizeof(line), "Remaining: %s", money);
  lv_label_set_text(lblRemaining, line);

  lv_bar_set_value(barBudget, (int)usedPct, LV_ANIM_ON);

  if (listCart) {
    lv_obj_clean(listCart);
    for (int i = 0; i < cartCount; i++) {
      lv_obj_t* row = lv_obj_create(listCart);
      lv_obj_add_style(row, &styleRow, LV_PART_MAIN);
      lv_obj_set_width(row, 720);
      lv_obj_set_height(row, LV_SIZE_CONTENT);
      lv_obj_set_style_pad_all(row, 10, LV_PART_MAIN);

      float lineTotal = cart[i].qty * cart[i].product->price;
      char unitText[24];
      char totalText[24];
      char itemLine[240];
      formatMoney(cart[i].product->price, unitText, sizeof(unitText));
      formatMoney(lineTotal, totalText, sizeof(totalText));
      snprintf(itemLine, sizeof(itemLine), "%s\nPrice: %s   Qty: %d   Total: %s", cart[i].product->name, unitText, cart[i].qty, totalText);

      lv_obj_t* item = lv_label_create(row);
      lv_obj_add_style(item, &styleSubtitle, LV_PART_MAIN);
      lv_label_set_text(item, itemLine);
      lv_obj_set_width(item, 560);
      lv_obj_align(item, LV_ALIGN_LEFT_MID, 0, 0);

      lv_obj_t* btnMinus = lv_btn_create(row);
      lv_obj_add_style(btnMinus, &styleBtnSecondary, LV_PART_MAIN);
      lv_obj_add_style(btnMinus, &styleBtnSecondaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
      lv_obj_set_size(btnMinus, 44, 36);
      lv_obj_align(btnMinus, LV_ALIGN_RIGHT_MID, -120, 0);
      lv_obj_t* lbMinus = lv_label_create(btnMinus);
      lv_label_set_text(lbMinus, "-");
      lv_obj_set_style_text_color(lbMinus, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
      lv_obj_center(lbMinus);
      lv_obj_add_event_cb(btnMinus, cartQtyMinusBtnEvent, LV_EVENT_CLICKED, (void*)cart[i].product);

      lv_obj_t* btnPlus = lv_btn_create(row);
      lv_obj_add_style(btnPlus, &styleBtnSecondary, LV_PART_MAIN);
      lv_obj_add_style(btnPlus, &styleBtnSecondaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
      lv_obj_set_size(btnPlus, 44, 36);
      lv_obj_align(btnPlus, LV_ALIGN_RIGHT_MID, -60, 0);
      lv_obj_t* lbPlus = lv_label_create(btnPlus);
      lv_label_set_text(lbPlus, "+");
      lv_obj_set_style_text_color(lbPlus, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
      lv_obj_center(lbPlus);
      lv_obj_add_event_cb(btnPlus, cartQtyPlusBtnEvent, LV_EVENT_CLICKED, (void*)cart[i].product);

      lv_obj_t* btnDelete = lv_btn_create(row);
      lv_obj_add_style(btnDelete, &styleBtnDanger, LV_PART_MAIN);
      lv_obj_add_style(btnDelete, &styleBtnDangerPressed, LV_PART_MAIN | LV_STATE_PRESSED);
      lv_obj_set_size(btnDelete, 52, 36);
      lv_obj_align(btnDelete, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_t* lbDelete = lv_label_create(btnDelete);
      lv_label_set_text(lbDelete, "Del");
      lv_obj_center(lbDelete);
      lv_obj_add_event_cb(btnDelete, cartDeleteBtnEvent, LV_EVENT_CLICKED, (void*)cart[i].product);
    }
  }
}

void refreshCheckoutScreen() {
  // Rebuild checkout table and totals from current cart state.
  if (!tblCheckoutItems || !lblCheckoutTotal || !lblCheckoutBudget) return;

  lv_table_set_col_cnt(tblCheckoutItems, 4);
  lv_table_set_row_cnt(tblCheckoutItems, cartCount + 1);
  lv_table_set_cell_value(tblCheckoutItems, 0, 0, "Product Name");
  lv_table_set_cell_value(tblCheckoutItems, 0, 1, "Price");
  lv_table_set_cell_value(tblCheckoutItems, 0, 2, "Qty");
  lv_table_set_cell_value(tblCheckoutItems, 0, 3, "Total");

  for (int i = 0; i < cartCount; i++) {
    char priceText[24];
    char qtyText[12];
    char totalText[24];
    formatMoney(cart[i].product->price, priceText, sizeof(priceText));
    snprintf(qtyText, sizeof(qtyText), "%d", cart[i].qty);
    formatMoney(cart[i].product->price * cart[i].qty, totalText, sizeof(totalText));

    lv_table_set_cell_value(tblCheckoutItems, i + 1, 0, cart[i].product->name);
    lv_table_set_cell_value(tblCheckoutItems, i + 1, 1, priceText);
    lv_table_set_cell_value(tblCheckoutItems, i + 1, 2, qtyText);
    lv_table_set_cell_value(tblCheckoutItems, i + 1, 3, totalText);
  }

  char budgetText[24];
  char spentText[24];
  char budgetLine[64];
  char totalLine[64];
  formatMoney(budget, budgetText, sizeof(budgetText));
  formatMoney(spent, spentText, sizeof(spentText));
  snprintf(budgetLine, sizeof(budgetLine), "Budget: %s", budgetText);
  snprintf(totalLine, sizeof(totalLine), "Total: %s", spentText);
  lv_label_set_text(lblCheckoutBudget, budgetLine);
  lv_label_set_text(lblCheckoutTotal, totalLine);
}



void switchScreen(AppScreen next) {
  // Central screen state transition handler and pre-load refresh logic.
  currentScreen = next;
  if (next == AppScreen::WELCOME) {
    hideThankYouModal();
    hideCheckPriceModal();
    digitalWrite(LED_PIN, LOW);
    lv_scr_load(scrWelcome);
  } else if (next == AppScreen::SCANNING) {
    hideThankYouModal();
    digitalWrite(LED_PIN, LOW);
    refreshScanScreen();
    lv_scr_load(scrScanning);
  } else if (next == AppScreen::ALERT) {
    char budgetText[24];
    char spentText[24];
    formatMoney(budget, budgetText, sizeof(budgetText));
    formatMoney(spent, spentText, sizeof(spentText));
    lv_label_set_text_fmt(lblAlertBudgetValue, "%s", budgetText);
    lv_label_set_text_fmt(lblAlertSpentValue, "%s", spentText);
    scanLedPulseUntil = 0;
    hideThankYouModal();
    hideCheckPriceModal();
    lv_scr_load(scrAlert);
  } else if (next == AppScreen::CHECKOUT) {
    hideCheckPriceModal();
    digitalWrite(LED_PIN, LOW);
    refreshCheckoutScreen();
    lv_scr_load(scrCheckout);
  }
}

void alertTimerCb(lv_timer_t* t) {
  // Periodic alert behavior: LED blink + repeating alert tones.
  (void)t;
  if (currentScreen != AppScreen::ALERT) return;

  static bool on = false;
  on = !on;
  digitalWrite(LED_PIN, on ? HIGH : LOW);

#if ENABLE_SPEAKER_ALERT
  playAlertNotificationBeep();
#endif
}

void hidePopup() {
  // Close item detail popup if currently open.
  if (popup) {
    lv_obj_del(popup);
    popup = nullptr;
  }
}

void popupQtyUpdate() {
  // Refresh quantity label in item detail popup.
  if (lblPopupQty) {
    lv_label_set_text_fmt(lblPopupQty, "Qty: %d", pendingQty);
  }
}

void popupBtnEvent(lv_event_t* e) {
  // Handle popup controls: plus, minus, add, and cancel.
  lv_obj_t* target = (lv_obj_t*)lv_event_get_target(e);
  intptr_t role = (intptr_t)lv_event_get_user_data(e);

  if (role == 1) {
    pendingQty++;
    popupQtyUpdate();
  } else if (role == 2) {
    if (pendingQty > 1) pendingQty--;
    popupQtyUpdate();
  } else if (role == 3) {
    if (pendingProduct) {
      addProductToCart(pendingProduct, pendingQty);
      lv_label_set_text_fmt(lblScanStatus, "Added: %s x%d", pendingProduct->name, pendingQty);
      refreshScanScreen();
      if (spent >= budget && budget > 0.01f) {
        switchScreen(AppScreen::ALERT);
      }
    }
    (void)target;
    hidePopup();
  } else if (role == 4) {
    lv_label_set_text(lblScanStatus, "Add canceled");
    (void)target;
    hidePopup();
  }
}

void showItemPopup(const Product* product) {
  // Build popup for selecting quantity before adding product.
  hidePopup();
  pendingProduct = product;
  pendingQty = 1;

  popup = lv_obj_create(scrScanning);
  lv_obj_set_size(popup, 520, 280);
  lv_obj_center(popup);

  lv_obj_t* title = lv_label_create(popup);
  lv_label_set_text(title, "Item Detail");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

  lblPopupName = lv_label_create(popup);
  lv_label_set_text_fmt(lblPopupName, "%s", product->name);
  lv_obj_set_width(lblPopupName, 480);
  lv_obj_align(lblPopupName, LV_ALIGN_TOP_LEFT, 16, 40);

  lblPopupPrice = lv_label_create(popup);
  lv_label_set_text_fmt(lblPopupPrice, "Price: PHP %.2f", product->price);
  lv_obj_align(lblPopupPrice, LV_ALIGN_TOP_LEFT, 16, 78);

  lblPopupQty = lv_label_create(popup);
  lv_obj_align(lblPopupQty, LV_ALIGN_TOP_LEFT, 16, 116);
  popupQtyUpdate();

  lv_obj_t* btnMinus = lv_btn_create(popup);
  lv_obj_set_size(btnMinus, 70, 46);
  lv_obj_align(btnMinus, LV_ALIGN_TOP_LEFT, 180, 106);
  lv_obj_t* lbm = lv_label_create(btnMinus);
  lv_label_set_text(lbm, "-");
  lv_obj_center(lbm);
  lv_obj_add_event_cb(btnMinus, popupBtnEvent, LV_EVENT_CLICKED, (void*)2);

  lv_obj_t* btnPlus = lv_btn_create(popup);
  lv_obj_set_size(btnPlus, 70, 46);
  lv_obj_align(btnPlus, LV_ALIGN_TOP_LEFT, 260, 106);
  lv_obj_t* lbp = lv_label_create(btnPlus);
  lv_label_set_text(lbp, "+");
  lv_obj_center(lbp);
  lv_obj_add_event_cb(btnPlus, popupBtnEvent, LV_EVENT_CLICKED, (void*)1);

  lv_obj_t* btnAdd = lv_btn_create(popup);
  lv_obj_set_size(btnAdd, 160, 52);
  lv_obj_align(btnAdd, LV_ALIGN_BOTTOM_LEFT, 24, -16);
  lv_obj_t* lba = lv_label_create(btnAdd);
  lv_label_set_text(lba, "Add to Cart");
  lv_obj_center(lba);
  lv_obj_add_event_cb(btnAdd, popupBtnEvent, LV_EVENT_CLICKED, (void*)3);

  lv_obj_t* btnCancel = lv_btn_create(popup);
  lv_obj_set_size(btnCancel, 140, 52);
  lv_obj_align(btnCancel, LV_ALIGN_BOTTOM_RIGHT, -24, -16);
  lv_obj_t* lbc = lv_label_create(btnCancel);
  lv_label_set_text(lbc, "Cancel");
  lv_obj_center(lbc);
  lv_obj_add_event_cb(btnCancel, popupBtnEvent, LV_EVENT_CLICKED, (void*)4);
}

void startBtnEvent(lv_event_t* e) {
  // Start-shopping action from welcome screen.
  (void)e;
  String input = lv_textarea_get_text(taBudget);
  String cleaned = sanitizeBudgetInput(input);
  lv_textarea_set_text(taBudget, cleaned.c_str());
  float parsed = cleaned.toFloat();
  if (parsed <= 0.0f) {
    return;
  }
  startThankYouBeepSequence();
  budget = parsed;
  spent = 0.0f;
  lv_label_set_text(lblScanStatus, "Ready to scan");
  switchScreen(AppScreen::SCANNING);
}

void scanCheckoutBtnEvent(lv_event_t* e) {
  // Jump from scanning screen to checkout screen.
  (void)e;
  startThankYouBeepSequence();
  switchScreen(AppScreen::CHECKOUT);
}

void alertContinueBtnEvent(lv_event_t* e) {
  // Leave alert screen and continue shopping.
  (void)e;
  switchScreen(AppScreen::SCANNING);
}

void alertCheckoutBtnEvent(lv_event_t* e) {
  // From alert screen, proceed directly to checkout.
  (void)e;
  startThankYouBeepSequence();
  switchScreen(AppScreen::CHECKOUT);
}

void checkoutCompleteBtnEvent(lv_event_t* e) {
  // Finalize transaction and reset state for next shopper.
  (void)e;
  resetCart();
  budget = 0.0f;
  lv_textarea_set_text(taBudget, "");
  refreshCheckoutScreen();
  showThankYouModal();
}

String readBarcodeLine() {
  // Read one barcode line from UART with short timeout/debounce window.
  String code = "";
  unsigned long start = millis();
  unsigned long lastData = millis();

  while (millis() - start < 250) {
    while (BarcodeSerial.available()) {
      char c = (char)BarcodeSerial.read();
      lastData = millis();
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

    if (code.length() >= 3 && (millis() - lastData) > 30) {
      break;
    }
  }

  code.trim();
  if (code.length() >= 3) return code;
  return "";
}

void buildWelcomeScreen() {
  // Construct budget-entry page and numeric keyboard.
  scrWelcome = lv_obj_create(nullptr);
  lv_obj_add_style(scrWelcome, &styleScreen, LV_PART_MAIN);

  lv_obj_t* title = lv_label_create(scrWelcome);
  lv_obj_add_style(title, &styleTitle, LV_PART_MAIN);
  lv_label_set_text(title, "GROCERY BUDGET TRACKER");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

  lv_obj_t* lbl = lv_label_create(scrWelcome);
  lv_obj_add_style(lbl, &styleSubtitle, LV_PART_MAIN);
  lv_label_set_text(lbl, "Input budget:");
  lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 54);

  taBudget = lv_textarea_create(scrWelcome);
  lv_obj_add_style(taBudget, &styleInput, LV_PART_MAIN);
  lv_obj_add_style(taBudget, &styleInputFocus, LV_PART_MAIN | LV_STATE_FOCUSED);
  lv_obj_set_size(taBudget, 280, 48);
  lv_obj_align(taBudget, LV_ALIGN_TOP_MID, 0, 78);
  lv_obj_set_style_text_align(taBudget, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_textarea_set_one_line(taBudget, true);
  lv_textarea_set_text(taBudget, "");
  lv_obj_add_event_cb(taBudget, budgetInputChangedEvent, LV_EVENT_VALUE_CHANGED, nullptr);

  lv_obj_t* btn = lv_btn_create(scrWelcome);
  lv_obj_add_style(btn, &styleBtnPrimary, LV_PART_MAIN);
  lv_obj_add_style(btn, &styleBtnPrimaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_size(btn, 220, 54);
  lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 140);
  lv_obj_t* btnLabel = lv_label_create(btn);
  lv_obj_set_style_text_color(btnLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(btnLabel, "Start Shopping");
  lv_obj_center(btnLabel);
  lv_obj_add_event_cb(btn, startBtnEvent, LV_EVENT_CLICKED, nullptr);

  kbBudget = lv_keyboard_create(scrWelcome);
  lv_obj_add_style(kbBudget, &styleCard, LV_PART_MAIN);
  lv_obj_set_style_bg_color(kbBudget, lv_color_hex(0xFFFFFF), LV_PART_ITEMS);
  lv_obj_set_style_text_color(kbBudget, lv_color_hex(0x111827), LV_PART_ITEMS);
  lv_obj_set_style_border_width(kbBudget, 2, LV_PART_ITEMS);
  lv_obj_set_style_border_color(kbBudget, lv_color_hex(0x7C3AED), LV_PART_ITEMS);
  lv_obj_set_style_bg_color(kbBudget, lv_color_hex(0xDC2626), LV_PART_ITEMS | LV_STATE_PRESSED);
  lv_obj_set_style_text_color(kbBudget, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(kbBudget, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_FOCUSED);
  lv_obj_set_style_border_color(kbBudget, lv_color_hex(0x7C3AED), LV_PART_ITEMS | LV_STATE_FOCUSED);
  lv_obj_set_style_bg_color(kbBudget, lv_color_hex(0x7C3AED), LV_PART_ITEMS | LV_STATE_USER_1);
  lv_obj_set_style_text_color(kbBudget, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_USER_1);
  lv_obj_set_style_radius(kbBudget, 10, LV_PART_ITEMS);
  lv_obj_set_size(kbBudget, 800, 220);
  lv_obj_align(kbBudget, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_keyboard_set_map(kbBudget, LV_KEYBOARD_MODE_USER_1, kb_num_map, kb_num_ctrl_map);
  lv_keyboard_set_mode(kbBudget, LV_KEYBOARD_MODE_USER_1);
  lv_keyboard_set_textarea(kbBudget, taBudget);
  lv_btnmatrix_set_btn_ctrl(kbBudget, 2, LV_BTNMATRIX_CTRL_CUSTOM_1);
}

void buildScanningScreen() {
  // Construct live cart/scan page with totals and action buttons.
  scrScanning = lv_obj_create(nullptr);
  lv_obj_add_style(scrScanning, &styleScreen, LV_PART_MAIN);

  lv_obj_t* title = lv_label_create(scrScanning);
  lv_obj_add_style(title, &styleTitle, LV_PART_MAIN);
  lv_label_set_text(title, "Scanning Screen");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

  lblBudget = lv_label_create(scrScanning);
  lv_obj_add_style(lblBudget, &styleSubtitle, LV_PART_MAIN);
  lv_obj_align(lblBudget, LV_ALIGN_TOP_LEFT, 20, 36);

  lblSpent = lv_label_create(scrScanning);
  lv_obj_add_style(lblSpent, &styleSubtitle, LV_PART_MAIN);
  lv_obj_set_style_text_color(lblSpent, lv_color_hex(0x14532D), LV_PART_MAIN);
  lv_obj_align(lblSpent, LV_ALIGN_TOP_LEFT, 20, 62);

  lblRemaining = lv_label_create(scrScanning);
  lv_obj_add_style(lblRemaining, &styleSubtitle, LV_PART_MAIN);
  lv_obj_align(lblRemaining, LV_ALIGN_TOP_LEFT, 20, 88);

  barBudget = lv_bar_create(scrScanning);
  lv_obj_add_style(barBudget, &styleCard, LV_PART_MAIN);
  lv_obj_set_style_bg_color(barBudget, lv_color_hex(0x7C3AED), LV_PART_MAIN);
  lv_obj_set_style_bg_color(barBudget, lv_color_hex(0x166534), LV_PART_INDICATOR);
  lv_obj_set_style_pad_all(barBudget, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(barBudget, 10, LV_PART_MAIN);
  lv_obj_set_style_radius(barBudget, 10, LV_PART_INDICATOR);
  lv_obj_set_size(barBudget, 760, 18);
  lv_obj_align(barBudget, LV_ALIGN_TOP_MID, 0, 118);
  lv_bar_set_range(barBudget, 0, 100);

  lblScanStatus = lv_label_create(scrScanning);
  lv_obj_add_style(lblScanStatus, &styleStatus, LV_PART_MAIN);
  lv_label_set_text(lblScanStatus, "Ready to scan");
  lv_obj_align(lblScanStatus, LV_ALIGN_TOP_MID, 0, 146);

  listCart = lv_obj_create(scrScanning);
  lv_obj_add_style(listCart, &styleList, LV_PART_MAIN);
  lv_obj_set_size(listCart, 760, 220);
  lv_obj_align(listCart, LV_ALIGN_TOP_MID, 0, 180);
  lv_obj_set_flex_flow(listCart, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scroll_dir(listCart, LV_DIR_VER);

  lv_obj_t* btnCheckout = lv_btn_create(scrScanning);
  lv_obj_add_style(btnCheckout, &styleBtnPrimary, LV_PART_MAIN);
  lv_obj_add_style(btnCheckout, &styleBtnPrimaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_size(btnCheckout, 220, 52);
  lv_obj_align(btnCheckout, LV_ALIGN_BOTTOM_RIGHT, -20, -14);
  lv_obj_t* bc = lv_label_create(btnCheckout);
  lv_obj_set_style_text_color(bc, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(bc, "Checkout");
  lv_obj_center(bc);
  lv_obj_add_event_cb(btnCheckout, scanCheckoutBtnEvent, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* btnCheckPrice = lv_btn_create(scrScanning);
  lv_obj_add_style(btnCheckPrice, &styleBtnSecondary, LV_PART_MAIN);
  lv_obj_add_style(btnCheckPrice, &styleBtnSecondaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_size(btnCheckPrice, 220, 52);
  lv_obj_align(btnCheckPrice, LV_ALIGN_BOTTOM_RIGHT, -250, -14);
  lv_obj_t* bcp = lv_label_create(btnCheckPrice);
  lv_obj_set_style_text_color(bcp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(bcp, "Check Price");
  lv_obj_center(bcp);
  lv_obj_add_event_cb(btnCheckPrice, [](lv_event_t* e) {
    (void)e;
    startThankYouBeepSequence();
    if (checkPriceModal != nullptr) return;

    checkPriceModal = lv_obj_create(scrScanning);
    lv_obj_add_style(checkPriceModal, &styleModal, LV_PART_MAIN);
    lv_obj_set_size(checkPriceModal, 560, 260);
    lv_obj_center(checkPriceModal);
    lv_obj_clear_flag(checkPriceModal, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* title = lv_label_create(checkPriceModal);
    lv_obj_add_style(title, &styleTitle, LV_PART_MAIN);
    lv_label_set_text(title, "Check Price");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

    lv_obj_t* hint = lv_label_create(checkPriceModal);
    lv_obj_add_style(hint, &styleSubtitle, LV_PART_MAIN);
    lv_label_set_text(hint, "Scan product to check their price");
    lv_obj_align(hint, LV_ALIGN_TOP_LEFT, 18, 52);

    lblCheckPriceResult = lv_label_create(checkPriceModal);
    lv_obj_add_style(lblCheckPriceResult, &styleSubtitle, LV_PART_MAIN);
    lv_obj_set_width(lblCheckPriceResult, 520);
    lv_label_set_text(lblCheckPriceResult, "Name: -\nPrice: -");
    lv_obj_align(lblCheckPriceResult, LV_ALIGN_TOP_LEFT, 18, 90);

    lv_obj_t* btnClose = lv_btn_create(checkPriceModal);
    lv_obj_add_style(btnClose, &styleBtnSecondary, LV_PART_MAIN);
    lv_obj_add_style(btnClose, &styleBtnSecondaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_size(btnClose, 140, 48);
    lv_obj_align(btnClose, LV_ALIGN_BOTTOM_RIGHT, -18, -16);
    lv_obj_t* closeLabel = lv_label_create(btnClose);
    lv_obj_set_style_text_color(closeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_text(closeLabel, "Close");
    lv_obj_center(closeLabel);
    lv_obj_add_event_cb(btnClose, [](lv_event_t* ev) {
      (void)ev;
      hideCheckPriceModal();
    }, LV_EVENT_CLICKED, nullptr);
  }, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* btnBack = lv_btn_create(scrScanning);
  lv_obj_add_style(btnBack, &styleBtnPrimary, LV_PART_MAIN);
  lv_obj_add_style(btnBack, &styleBtnPrimaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_size(btnBack, 220, 52);
  lv_obj_align(btnBack, LV_ALIGN_BOTTOM_LEFT, 20, -14);
  lv_obj_t* bb = lv_label_create(btnBack);
  lv_obj_set_style_text_color(bb, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(bb, "Back");
  lv_obj_center(bb);
  lv_obj_add_event_cb(btnBack, [](lv_event_t* e) {
    (void)e;
    switchScreen(AppScreen::WELCOME);
  }, LV_EVENT_CLICKED, nullptr);
}

void buildAlertScreen() {
  // Construct budget-reached warning page.
  scrAlert = lv_obj_create(nullptr);
  lv_obj_add_style(scrAlert, &styleAlertScreen, LV_PART_MAIN);

  lv_obj_t* title = lv_label_create(scrAlert);
  lv_obj_add_style(title, &styleAlertTitle, LV_PART_MAIN);
  lv_label_set_text(title, "BUDGET REACHED!");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 32);

  lv_obj_t* lblBudgetTitle = lv_label_create(scrAlert);
  lv_obj_add_style(lblBudgetTitle, &styleAlertTitle, LV_PART_MAIN);
  lv_label_set_text(lblBudgetTitle, "Budget");
  lv_obj_align(lblBudgetTitle, LV_ALIGN_TOP_MID, 0, 96);

  lblAlertBudgetValue = lv_label_create(scrAlert);
  lv_obj_add_style(lblAlertBudgetValue, &styleAlertTitle, LV_PART_MAIN);
  lv_obj_set_style_text_align(lblAlertBudgetValue, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_label_set_text(lblAlertBudgetValue, "PHP 0.00");
  lv_obj_align(lblAlertBudgetValue, LV_ALIGN_TOP_MID, 0, 126);

  lv_obj_t* lblSpentTitle = lv_label_create(scrAlert);
  lv_obj_add_style(lblSpentTitle, &styleAlertTitle, LV_PART_MAIN);
  lv_label_set_text(lblSpentTitle, "Spent");
  lv_obj_align(lblSpentTitle, LV_ALIGN_TOP_MID, 0, 194);

  lblAlertSpentValue = lv_label_create(scrAlert);
  lv_obj_add_style(lblAlertSpentValue, &styleAlertTitle, LV_PART_MAIN);
  lv_obj_set_style_text_align(lblAlertSpentValue, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_label_set_text(lblAlertSpentValue, "PHP 0.00");
  lv_obj_align(lblAlertSpentValue, LV_ALIGN_TOP_MID, 0, 224);

#if LV_FONT_MONTSERRAT_40
  lv_obj_set_style_text_font(lblAlertBudgetValue, &lv_font_montserrat_40, LV_PART_MAIN);
  lv_obj_set_style_text_font(lblAlertSpentValue, &lv_font_montserrat_40, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_36
  lv_obj_set_style_text_font(lblAlertBudgetValue, &lv_font_montserrat_36, LV_PART_MAIN);
  lv_obj_set_style_text_font(lblAlertSpentValue, &lv_font_montserrat_36, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_32
  lv_obj_set_style_text_font(lblAlertBudgetValue, &lv_font_montserrat_32, LV_PART_MAIN);
  lv_obj_set_style_text_font(lblAlertSpentValue, &lv_font_montserrat_32, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_28
  lv_obj_set_style_text_font(lblAlertBudgetValue, &lv_font_montserrat_28, LV_PART_MAIN);
  lv_obj_set_style_text_font(lblAlertSpentValue, &lv_font_montserrat_28, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_24
  lv_obj_set_style_text_font(lblAlertBudgetValue, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_font(lblAlertSpentValue, &lv_font_montserrat_24, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_20
  lv_obj_set_style_text_font(lblAlertBudgetValue, &lv_font_montserrat_20, LV_PART_MAIN);
  lv_obj_set_style_text_font(lblAlertSpentValue, &lv_font_montserrat_20, LV_PART_MAIN);
#endif

  lv_obj_t* btnContinue = lv_btn_create(scrAlert);
  lv_obj_add_style(btnContinue, &styleBtnSecondary, LV_PART_MAIN);
  lv_obj_add_style(btnContinue, &styleBtnSecondaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(btnContinue, lv_color_hex(0xFEE2E2), LV_PART_MAIN);
  lv_obj_set_style_text_color(btnContinue, lv_color_hex(0x7F1D1D), LV_PART_MAIN);
  lv_obj_set_size(btnContinue, 240, 56);
  lv_obj_align(btnContinue, LV_ALIGN_BOTTOM_LEFT, 30, -24);
  lv_obj_t* lc = lv_label_create(btnContinue);
  lv_obj_set_style_text_color(lc, lv_color_hex(0x7F1D1D), LV_PART_MAIN);
  lv_label_set_text(lc, "Continue");
  lv_obj_center(lc);
  lv_obj_add_event_cb(btnContinue, alertContinueBtnEvent, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* btnCheckout = lv_btn_create(scrAlert);
  lv_obj_add_style(btnCheckout, &styleBtnPrimary, LV_PART_MAIN);
  lv_obj_add_style(btnCheckout, &styleBtnPrimaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(btnCheckout, lv_color_hex(0x991B1B), LV_PART_MAIN);
  lv_obj_set_size(btnCheckout, 240, 56);
  lv_obj_align(btnCheckout, LV_ALIGN_BOTTOM_RIGHT, -30, -24);
  lv_obj_t* lq = lv_label_create(btnCheckout);
  lv_obj_set_style_text_color(lq, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(lq, "Checkout");
  lv_obj_center(lq);
  lv_obj_add_event_cb(btnCheckout, alertCheckoutBtnEvent, LV_EVENT_CLICKED, nullptr);
}

void buildCheckoutScreen() {
  // Construct checkout summary table and final action buttons.
  scrCheckout = lv_obj_create(nullptr);
  lv_obj_add_style(scrCheckout, &styleScreen, LV_PART_MAIN);

  lv_obj_t* title = lv_label_create(scrCheckout);
  lv_obj_add_style(title, &styleTitle, LV_PART_MAIN);
  lv_label_set_text(title, "Checkout Summary");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

  tblCheckoutItems = lv_table_create(scrCheckout);
  lv_obj_add_style(tblCheckoutItems, &styleTableMain, LV_PART_MAIN);
  lv_obj_add_style(tblCheckoutItems, &styleTableItems, LV_PART_ITEMS);
  lv_obj_set_size(tblCheckoutItems, 760, 300);
  lv_obj_align(tblCheckoutItems, LV_ALIGN_TOP_MID, 0, 48);
  lv_table_set_col_width(tblCheckoutItems, 0, 330);
  lv_table_set_col_width(tblCheckoutItems, 1, 150);
  lv_table_set_col_width(tblCheckoutItems, 2, 70);
  lv_table_set_col_width(tblCheckoutItems, 3, 190);

  lblCheckoutBudget = lv_label_create(scrCheckout);
  lv_obj_add_style(lblCheckoutBudget, &styleSubtitle, LV_PART_MAIN);
  lv_obj_align(lblCheckoutBudget, LV_ALIGN_BOTTOM_LEFT, 20, -50);

  lblCheckoutTotal = lv_label_create(scrCheckout);
  lv_obj_add_style(lblCheckoutTotal, &styleTitle, LV_PART_MAIN);
  lv_obj_align(lblCheckoutTotal, LV_ALIGN_BOTTOM_LEFT, 20, -18);
#if LV_FONT_MONTSERRAT_28
  lv_obj_set_style_text_font(lblCheckoutTotal, &lv_font_montserrat_28, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_24
  lv_obj_set_style_text_font(lblCheckoutTotal, &lv_font_montserrat_24, LV_PART_MAIN);
#elif LV_FONT_MONTSERRAT_20
  lv_obj_set_style_text_font(lblCheckoutTotal, &lv_font_montserrat_20, LV_PART_MAIN);
#endif

  lv_obj_t* btnComplete = lv_btn_create(scrCheckout);
  lv_obj_add_style(btnComplete, &styleBtnSecondary, LV_PART_MAIN);
  lv_obj_add_style(btnComplete, &styleBtnSecondaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_size(btnComplete, 220, 52);
  lv_obj_align(btnComplete, LV_ALIGN_BOTTOM_RIGHT, -20, -12);
  lv_obj_t* lb = lv_label_create(btnComplete);
  lv_obj_set_style_text_color(lb, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(lb, "Complete");
  lv_obj_center(lb);
  lv_obj_add_event_cb(btnComplete, checkoutCompleteBtnEvent, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* btnBack = lv_btn_create(scrCheckout);
  lv_obj_add_style(btnBack, &styleBtnPrimary, LV_PART_MAIN);
  lv_obj_add_style(btnBack, &styleBtnPrimaryPressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_size(btnBack, 220, 52);
  lv_obj_align(btnBack, LV_ALIGN_BOTTOM_RIGHT, -250, -12);
  lv_obj_t* lbb = lv_label_create(btnBack);
  lv_obj_set_style_text_color(lbb, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(lbb, "Back");
  lv_obj_center(lbb);
  lv_obj_add_event_cb(btnBack, [](lv_event_t* e) {
    (void)e;
    switchScreen(AppScreen::SCANNING);
  }, LV_EVENT_CLICKED, nullptr);
}

void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
  // LVGL display callback: flush rendered area to RGB panel.
  uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
  uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
  uint32_t count = w * h;

  uint16_t* src = (uint16_t*)&color_p->full;
  for (uint32_t i = 0; i < count; i++) {
    flushBuf[i] = toMode6RBG(src[i]);
  }

  lcd.startWrite();
  lcd.pushImage(area->x1, area->y1, w, h, flushBuf);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t* indev, lv_indev_data_t* data) {
  // LVGL input callback: read GT911 touch and map coordinates.
  (void)indev;
#if USE_TOUCH
  ts.read();
  bool touched = ts.isTouched;

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    int32_t rawX = ts.points[0].x;
    int32_t rawY = ts.points[0].y;

    int32_t touchX = map(rawX, SCREEN_W, 0, 0, SCREEN_W - 1);
    int32_t touchY = map(rawY, SCREEN_H, 0, 0, SCREEN_H - 1);

    if (touchX < 0) touchX = 0;
    if (touchX >= SCREEN_W) touchX = SCREEN_W - 1;
    if (touchY < 0) touchY = 0;
    if (touchY >= SCREEN_H) touchY = SCREEN_H - 1;

    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
  }
#else
  data->state = LV_INDEV_STATE_REL;
#endif
}

void setup() {
  // One-time initialization: power, IO, peripherals, LVGL, screens.
  Serial.begin(115200);
  delay(800);

  pinMode(TFT_BL_PIN, OUTPUT);
  digitalWrite(TFT_BL_PIN, HIGH);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  initSpeaker();

  Serial.println("\n[BOOT] BudgetTracker starting...");
  Serial.printf("[BOOT] psramFound=%d\n", psramFound());
#if ENABLE_SPEAKER_ALERT
  Serial.printf("[BOOT] Speaker: %s (PIN=%d)\n", speakerReady ? "READY" : "NOT READY", SPK_PIN);
#endif

  if (!psramFound()) {
    Serial.println("[ERR] PSRAM not found. Enable PSRAM in Arduino Tools.");
    while (true) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(TFT_BL_PIN, HIGH);
      delay(220);
      digitalWrite(LED_PIN, LOW);
      digitalWrite(TFT_BL_PIN, LOW);
      delay(220);
    }
  }

  Wire.begin(19, 20);
  Wire.setClock(400000);
  Serial.println("[BOOT] I2C up (19/20)");

  out.reset();
  out.setMode(IO_OUTPUT);
  out.setState(IO0, IO_LOW);
  out.setState(IO1, IO_LOW);
  delay(20);
  out.setState(IO0, IO_HIGH);
  delay(100);
  out.setMode(IO1, IO_INPUT);
  Serial.println("[BOOT] PCA9557 timing sequence done");

  BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
  while (BarcodeSerial.available()) {
    BarcodeSerial.read();
  }
  scannerUartReady = true;
  Serial.printf("[BOOT] UART2 scanner ready (RX=%d TX=%d)\n", BARCODE_RX, BARCODE_TX);
  Serial.println("[BOOT] Scanner test: trigger one barcode to see 'Scanned:' status on screen");

  lv_init();
  initUiStyles();
  Serial.println("[BOOT] LVGL initialized");

  lcd.begin();
  lcd.setColorDepth(16);
  lcd.setRotation(0);
  lcd.fillScreen(lcd.color565(0, 0, 0));
  Serial.println("[BOOT] LCD initialized");
#if USE_TOUCH
  Wire.begin(19, 20);
  Wire.setClock(400000);
  ts.begin();
  ts.setRotation(ROTATION_NORMAL);
  touchInitOk = true;
  for (int i = 0; i < 3; i++) {
    ts.read();
    if (ts.isTouched) {
      touchDetectedAtBoot = true;
      break;
    }
    delay(20);
  }
  Serial.println("[BOOT] Touch initialized");
#endif

  lv_disp_draw_buf_init(&draw_buf, buf1, nullptr, SCREEN_W * 40);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_W;
  disp_drv.ver_res = SCREEN_H;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.antialiasing = 1;
  lv_disp_drv_register(&disp_drv);

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  buildWelcomeScreen();
  buildScanningScreen();
  buildAlertScreen();
  buildCheckoutScreen();


  alertTimer = lv_timer_create(alertTimerCb, 275, nullptr);

  switchScreen(AppScreen::WELCOME);
  lv_timer_handler();
  Serial.println("[BOOT] UI ready");
}

void loop() {
  // Main runtime loop: service LVGL, scanner input, and alert timing.
  lv_timer_handler();
  delay(5);
  updateScanLedPulse();
  updateAlertBeep();

  if (currentScreen != AppScreen::SCANNING) return;

  String code = readBarcodeLine();
  if (code.length() == 0) return;
  triggerScanLedPulse(90);

  if (checkPriceModal != nullptr) {
    const Product* cp = findProductByBarcode(code);
    if (!lblCheckPriceResult) return;

    if (!cp) {
      lv_label_set_text(lblCheckPriceResult, "Name: Not found\nPrice: -");
    } else {
      char priceText[24];
      formatMoney(cp->price, priceText, sizeof(priceText));
      lv_label_set_text_fmt(lblCheckPriceResult, "Name: %s\nPrice: %s", cp->name, priceText);
    }
    return;
  }

  const Product* p = findProductByBarcode(code);
  if (!p) {
    lv_label_set_text_fmt(lblScanStatus, "Unknown barcode: %s", code.c_str());
    return;
  }

  addProductToCart(p, 1);
  lv_label_set_text_fmt(lblScanStatus, "Added: %s x1", p->name);
  refreshScanScreen();

  if (spent >= budget && budget > 0.01f) {
    switchScreen(AppScreen::ALERT);
  }
}

void hideCheckPriceModal() {
  // Safely close check-price modal and clear references.
  if (checkPriceModal) {
    lv_obj_del(checkPriceModal);
    checkPriceModal = nullptr;
    lblCheckPriceResult = nullptr;
  }
}

void hideThankYouModal() {
  // Safely close thank-you modal and its timer.
  if (thankYouTimer) {
    lv_timer_del(thankYouTimer);
    thankYouTimer = nullptr;
  }
  if (thankYouModal) {
    lv_obj_del(thankYouModal);
    thankYouModal = nullptr;
  }
}
