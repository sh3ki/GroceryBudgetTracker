#include <Arduino.h>
#define LV_CONF_SKIP 1
#include <lvgl.h>
#include <Wire.h>
#include <PCA9557.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <TAMC_GT911.h>

static const int LED_PIN = 38;
static const int BARCODE_RX = 44;
static const int BARCODE_TX = 43;

#define ENABLE_SPEAKER_ALERT 1
static const int SPK_PIN = 3;
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
TAMC_GT911 ts(19, 20, -1, -1, SCREEN_W, SCREEN_H);
HardwareSerial BarcodeSerial(2);

struct Product {
  const char* barcode;
  const char* name;
  float price;
};

const Product products[] = {
  {"4800552888110", "Sample Product", 49.50},
  {"4800016644184", "Lucky Me Pancit Canton Original", 15.50},
  {"4800888120175", "Coca-Cola 1.5L", 65.00},
  {"4800024466322", "Alaska Evaporated Milk", 32.75},
};

const int PRODUCT_COUNT = sizeof(products) / sizeof(Product);

struct CartItem {
  const Product* product;
  int qty;
};

CartItem cart[64];
int cartCount = 0;
float budget = 0.0f;
float spent = 0.0f;

const Product* pendingProduct = nullptr;
int pendingQty = 1;

enum class AppScreen {
  WELCOME,
  SCANNING,
  ALERT,
  CHECKOUT
};

AppScreen currentScreen = AppScreen::WELCOME;

lv_obj_t* scrWelcome = nullptr;
lv_obj_t* scrScanning = nullptr;
lv_obj_t* scrAlert = nullptr;
lv_obj_t* scrCheckout = nullptr;

lv_obj_t* taBudget = nullptr;
lv_obj_t* kbBudget = nullptr;

lv_obj_t* lblBudget = nullptr;
lv_obj_t* lblSpent = nullptr;
lv_obj_t* lblRemaining = nullptr;
lv_obj_t* barBudget = nullptr;
lv_obj_t* lblScanStatus = nullptr;
lv_obj_t* listCart = nullptr;

lv_obj_t* lblAlertInfo = nullptr;
lv_obj_t* lblCheckoutTotal = nullptr;
lv_obj_t* taCheckoutItems = nullptr;

lv_obj_t* popup = nullptr;
lv_obj_t* lblPopupName = nullptr;
lv_obj_t* lblPopupPrice = nullptr;
lv_obj_t* lblPopupQty = nullptr;

lv_timer_t* alertTimer = nullptr;
lv_display_t* display = nullptr;
static uint8_t buf1[SCREEN_W * 40 * sizeof(lv_color_t)];
static uint16_t flushBuf[SCREEN_W * 40];

void beep(int frequency, int durationMs) {
#if ENABLE_SPEAKER_ALERT
  ledcSetup(SPK_CHANNEL, frequency, 8);
  ledcAttachPin(SPK_PIN, SPK_CHANNEL);
  ledcWrite(SPK_CHANNEL, 128);
  delay(durationMs);
  ledcWrite(SPK_CHANNEL, 0);
#else
  (void)frequency;
  delay(durationMs);
#endif
}

const Product* findProductByBarcode(const String& code) {
  for (int i = 0; i < PRODUCT_COUNT; i++) {
    if (code == products[i].barcode) {
      return &products[i];
    }
  }
  return nullptr;
}

void resetCart() {
  cartCount = 0;
  spent = 0.0f;
}

void addProductToCart(const Product* product, int qty) {
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

void refreshScanScreen() {
  if (!lblBudget) return;

  float remaining = budget - spent;
  float usedPct = (budget > 0.01f) ? (spent / budget) * 100.0f : 0.0f;
  if (usedPct < 0) usedPct = 0;
  if (usedPct > 100) usedPct = 100;

  lv_label_set_text_fmt(lblBudget, "Budget: PHP %.2f", budget);
  lv_label_set_text_fmt(lblSpent, "Spent: PHP %.2f", spent);
  lv_label_set_text_fmt(lblRemaining, "Remaining: PHP %.2f", remaining);
  lv_bar_set_value(barBudget, (int)usedPct, LV_ANIM_ON);

  if (listCart) {
    lv_obj_clean(listCart);
    for (int i = 0; i < cartCount; i++) {
      float lineTotal = cart[i].qty * cart[i].product->price;
      lv_obj_t* item = lv_label_create(listCart);
      lv_label_set_text_fmt(item, "%s  x%d  | PHP %.2f", cart[i].product->name, cart[i].qty, lineTotal);
      lv_obj_set_width(item, 720);
    }
  }
}

void refreshCheckoutScreen() {
  if (!taCheckoutItems || !lblCheckoutTotal) return;

  lv_textarea_set_text(taCheckoutItems, "");
  for (int i = 0; i < cartCount; i++) {
    char line[180];
    snprintf(line, sizeof(line), "%s x%d  - PHP %.2f\n", cart[i].product->name, cart[i].qty, cart[i].product->price * cart[i].qty);
    lv_textarea_add_text(taCheckoutItems, line);
  }

  lv_label_set_text_fmt(lblCheckoutTotal, "Total: PHP %.2f", spent);
}

void switchScreen(AppScreen next) {
  currentScreen = next;
  if (next == AppScreen::WELCOME) {
    lv_screen_load(scrWelcome);
  } else if (next == AppScreen::SCANNING) {
    refreshScanScreen();
    lv_screen_load(scrScanning);
  } else if (next == AppScreen::ALERT) {
    lv_label_set_text_fmt(lblAlertInfo, "Budget: PHP %.2f\nSpent: PHP %.2f", budget, spent);
    lv_screen_load(scrAlert);
  } else if (next == AppScreen::CHECKOUT) {
    refreshCheckoutScreen();
    lv_screen_load(scrCheckout);
  }
}

void alertTimerCb(lv_timer_t* t) {
  (void)t;
  if (currentScreen != AppScreen::ALERT) return;

  static bool on = false;
  on = !on;
  lv_color_t c = on ? lv_palette_main(LV_PALETTE_RED) : lv_palette_darken(LV_PALETTE_RED, 2);
  lv_obj_set_style_bg_color(scrAlert, c, LV_PART_MAIN);

#if ENABLE_SPEAKER_ALERT
  beep(on ? 1300 : 900, 60);
#endif
}

void hidePopup() {
  if (popup) {
    lv_obj_del(popup);
    popup = nullptr;
  }
}

void popupQtyUpdate() {
  if (lblPopupQty) {
    lv_label_set_text_fmt(lblPopupQty, "Qty: %d", pendingQty);
  }
}

void popupBtnEvent(lv_event_t* e) {
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
  (void)e;
  String input = lv_textarea_get_text(taBudget);
  float parsed = input.toFloat();
  if (parsed <= 0.0f) parsed = 2500.0f;
  budget = parsed;
  spent = 0.0f;
  lv_label_set_text(lblScanStatus, "Ready to scan");
  switchScreen(AppScreen::SCANNING);
}

void scanCheckoutBtnEvent(lv_event_t* e) {
  (void)e;
  switchScreen(AppScreen::CHECKOUT);
}

void alertContinueBtnEvent(lv_event_t* e) {
  (void)e;
  switchScreen(AppScreen::SCANNING);
}

void alertCheckoutBtnEvent(lv_event_t* e) {
  (void)e;
  switchScreen(AppScreen::CHECKOUT);
}

void checkoutCompleteBtnEvent(lv_event_t* e) {
  (void)e;
  resetCart();
  budget = 0.0f;
  lv_textarea_set_text(taBudget, "2500");
  switchScreen(AppScreen::WELCOME);
}

String readBarcodeLine() {
  String code = "";
  unsigned long start = millis();

  while (millis() - start < 150) {
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

void buildWelcomeScreen() {
  scrWelcome = lv_obj_create(nullptr);

  lv_obj_t* title = lv_label_create(scrWelcome);
  lv_label_set_text(title, "GROCERY BUDGET TRACKER");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

  lv_obj_t* lbl = lv_label_create(scrWelcome);
  lv_label_set_text(lbl, "Input budget:");
  lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 24, 54);

  taBudget = lv_textarea_create(scrWelcome);
  lv_obj_set_size(taBudget, 280, 48);
  lv_obj_align(taBudget, LV_ALIGN_TOP_LEFT, 24, 78);
  lv_textarea_set_one_line(taBudget, true);
  lv_textarea_set_text(taBudget, "2500");

  lv_obj_t* btn = lv_btn_create(scrWelcome);
  lv_obj_set_size(btn, 220, 54);
  lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 24, 140);
  lv_obj_t* btnLabel = lv_label_create(btn);
  lv_label_set_text(btnLabel, "Start Shopping");
  lv_obj_center(btnLabel);
  lv_obj_add_event_cb(btn, startBtnEvent, LV_EVENT_CLICKED, nullptr);

  kbBudget = lv_keyboard_create(scrWelcome);
  lv_obj_set_size(kbBudget, 800, 220);
  lv_obj_align(kbBudget, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_keyboard_set_textarea(kbBudget, taBudget);
}

void buildScanningScreen() {
  scrScanning = lv_obj_create(nullptr);

  lv_obj_t* title = lv_label_create(scrScanning);
  lv_label_set_text(title, "Scanning Screen");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

  lblBudget = lv_label_create(scrScanning);
  lv_obj_align(lblBudget, LV_ALIGN_TOP_LEFT, 20, 36);

  lblSpent = lv_label_create(scrScanning);
  lv_obj_align(lblSpent, LV_ALIGN_TOP_LEFT, 20, 62);

  lblRemaining = lv_label_create(scrScanning);
  lv_obj_align(lblRemaining, LV_ALIGN_TOP_LEFT, 20, 88);

  barBudget = lv_bar_create(scrScanning);
  lv_obj_set_size(barBudget, 760, 18);
  lv_obj_align(barBudget, LV_ALIGN_TOP_MID, 0, 118);
  lv_bar_set_range(barBudget, 0, 100);

  lblScanStatus = lv_label_create(scrScanning);
  lv_label_set_text(lblScanStatus, "Ready to scan");
  lv_obj_align(lblScanStatus, LV_ALIGN_TOP_LEFT, 20, 146);

  lv_obj_t* lblIndicator = lv_label_create(scrScanning);
  lv_label_set_text(lblIndicator, "Scanner: ACTIVE");
  lv_obj_align(lblIndicator, LV_ALIGN_TOP_RIGHT, -20, 146);

  listCart = lv_obj_create(scrScanning);
  lv_obj_set_size(listCart, 760, 220);
  lv_obj_align(listCart, LV_ALIGN_TOP_MID, 0, 180);
  lv_obj_set_flex_flow(listCart, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scroll_dir(listCart, LV_DIR_VER);

  lv_obj_t* btnCheckout = lv_btn_create(scrScanning);
  lv_obj_set_size(btnCheckout, 220, 52);
  lv_obj_align(btnCheckout, LV_ALIGN_BOTTOM_RIGHT, -20, -14);
  lv_obj_t* bc = lv_label_create(btnCheckout);
  lv_label_set_text(bc, "Checkout");
  lv_obj_center(bc);
  lv_obj_add_event_cb(btnCheckout, scanCheckoutBtnEvent, LV_EVENT_CLICKED, nullptr);
}

void buildAlertScreen() {
  scrAlert = lv_obj_create(nullptr);
  lv_obj_set_style_bg_color(scrAlert, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

  lv_obj_t* title = lv_label_create(scrAlert);
  lv_label_set_text(title, "BUDGET REACHED!");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 32);

  lblAlertInfo = lv_label_create(scrAlert);
  lv_obj_align(lblAlertInfo, LV_ALIGN_TOP_MID, 0, 90);

  lv_obj_t* btnContinue = lv_btn_create(scrAlert);
  lv_obj_set_size(btnContinue, 240, 56);
  lv_obj_align(btnContinue, LV_ALIGN_BOTTOM_LEFT, 30, -24);
  lv_obj_t* lc = lv_label_create(btnContinue);
  lv_label_set_text(lc, "Continue");
  lv_obj_center(lc);
  lv_obj_add_event_cb(btnContinue, alertContinueBtnEvent, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* btnCheckout = lv_btn_create(scrAlert);
  lv_obj_set_size(btnCheckout, 240, 56);
  lv_obj_align(btnCheckout, LV_ALIGN_BOTTOM_RIGHT, -30, -24);
  lv_obj_t* lq = lv_label_create(btnCheckout);
  lv_label_set_text(lq, "Checkout");
  lv_obj_center(lq);
  lv_obj_add_event_cb(btnCheckout, alertCheckoutBtnEvent, LV_EVENT_CLICKED, nullptr);
}

void buildCheckoutScreen() {
  scrCheckout = lv_obj_create(nullptr);

  lv_obj_t* title = lv_label_create(scrCheckout);
  lv_label_set_text(title, "Checkout Summary");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

  taCheckoutItems = lv_textarea_create(scrCheckout);
  lv_obj_set_size(taCheckoutItems, 760, 320);
  lv_obj_align(taCheckoutItems, LV_ALIGN_TOP_MID, 0, 48);
  lv_textarea_set_cursor_click_pos(taCheckoutItems, false);
  lv_obj_clear_flag(taCheckoutItems, LV_OBJ_FLAG_CLICK_FOCUSABLE);

  lblCheckoutTotal = lv_label_create(scrCheckout);
  lv_obj_align(lblCheckoutTotal, LV_ALIGN_BOTTOM_LEFT, 20, -18);

  lv_obj_t* btnComplete = lv_btn_create(scrCheckout);
  lv_obj_set_size(btnComplete, 220, 52);
  lv_obj_align(btnComplete, LV_ALIGN_BOTTOM_RIGHT, -20, -12);
  lv_obj_t* lb = lv_label_create(btnComplete);
  lv_label_set_text(lb, "Complete");
  lv_obj_center(lb);
  lv_obj_add_event_cb(btnComplete, checkoutCompleteBtnEvent, LV_EVENT_CLICKED, nullptr);
}

void my_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
  uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
  uint32_t count = w * h;

  uint16_t* src = (uint16_t*)px_map;
  for (uint32_t i = 0; i < count; i++) {
    uint16_t c = src[i];
    uint16_t r5 = (c >> 11) & 0x1F;
    uint16_t g6 = (c >> 5) & 0x3F;
    uint16_t b5 = c & 0x1F;

    uint16_t g6_from_b5 = (uint16_t)((b5 << 1) | (b5 >> 4));
    uint16_t b5_from_g6 = (uint16_t)(g6 >> 1);

    flushBuf[i] = (uint16_t)((r5 << 11) | (g6_from_b5 << 5) | b5_from_g6);
  }

  lcd.startWrite();
  lcd.pushImage(area->x1, area->y1, w, h, flushBuf);
  lcd.endWrite();

  lv_display_flush_ready(disp);
}

void my_touchpad_read(lv_indev_t* indev, lv_indev_data_t* data) {
  (void)indev;
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
}

void setup() {
  Serial.begin(115200);
  delay(600);

  if (!psramFound()) {
    Serial.println("PSRAM not found. Enable PSRAM in Arduino Tools.");
    while (true) {
      delay(500);
    }
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Wire.begin(19, 20);
  Wire.setClock(400000);

  out.reset();
  out.setMode(IO_OUTPUT);
  out.setState(IO0, IO_LOW);
  out.setState(IO1, IO_LOW);
  delay(20);
  out.setState(IO0, IO_HIGH);
  delay(100);
  out.setMode(IO1, IO_INPUT);

  BarcodeSerial.begin(9600, SERIAL_8N1, BARCODE_RX, BARCODE_TX);
  while (BarcodeSerial.available()) {
    BarcodeSerial.read();
  }

  lv_init();

  lcd.begin();
  lcd.setColorDepth(16);
  lcd.setRotation(0);
  ts.begin();
  ts.setRotation(ROTATION_NORMAL);

  display = lv_display_create(SCREEN_W, SCREEN_H);
  lv_display_set_flush_cb(display, my_disp_flush);
  lv_display_set_buffers(display, buf1, nullptr, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  buildWelcomeScreen();
  buildScanningScreen();
  buildAlertScreen();
  buildCheckoutScreen();

  alertTimer = lv_timer_create(alertTimerCb, 550, nullptr);

  switchScreen(AppScreen::WELCOME);
  lv_timer_handler();
}

void loop() {
  lv_tick_inc(5);
  lv_timer_handler();
  delay(5);

  if (currentScreen != AppScreen::SCANNING) return;
  if (popup != nullptr) return;

  String code = readBarcodeLine();
  if (code.length() == 0) return;

  const Product* p = findProductByBarcode(code);
  if (!p) {
    lv_label_set_text_fmt(lblScanStatus, "Unknown barcode: %s", code.c_str());
    return;
  }

  lv_label_set_text_fmt(lblScanStatus, "Scanned: %s", p->name);
  showItemPopup(p);
}
