#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#include "solar_saucer_shared.h"
#include "secrets.h"

// ESP8266 pin number -> GPIO mapping
#define D0  16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15

// Shift register pin number -> bit index mapping
#define SR_D1  6
#define SR_D2  5
#define SR_D3  4
#define SR_D4  3
#define SR_D5  2
#define SR_D6  1
#define SR_D7  0

// ESP8266 pins used for shift register
#define SR_LOAD          D8
#define SR_CLOCK_ENABLE  D5
#define SR_DATA          D6
#define SR_CLOCK         D7

// ESP8266 pins used for sliders
#define SLIDER_1         D1
#define SLIDER_2         D0
#define SLIDER_3         D2
#define SLIDER_4         D3

// Shift register pins used for buttons
#define BUTTON_FLASH           SR_D1
#define BUTTON_EXPLODE         SR_D2
#define BUTTON_TWINKLE         SR_D3
#define BUTTON_COLOR_GRADIENT  SR_D4
#define BUTTON_COLOR_WHEEL     SR_D5
#define BUTTON_SPIN            SR_D6
#define BUTTON_COLOR_SOLID     SR_D7

// LEDs
#define LEDS_PIN  4  // D4
#define NUM_LEDS  3

struct Button {
  int pin;
  char description[20];
  bool pressed;
};

struct Slider {
  int pin;
  char description[20];
  int value;
  int prev;
};

struct Timer {
  unsigned long totalCycleTime;
  unsigned long lastCycleTime;
  void reset() {
    lastCycleTime = millis();
  };
  bool complete() {
    return (millis() - lastCycleTime) > totalCycleTime;
  };
};

int backgrounds[] = {VIZ_TWINKLE, VIZ_EXPLODE, VIZ_SPIN};
int sliderIndex = -1;
unsigned long backgroundCycleTime = 1000 * 60 * 15;  // 15 minutes

// Slider Actions
msg brightness = {ACTION_SET_BRIGHTNESS};
msg colorLeft = {ACTION_SET_COLOR_LEFT};
msg colorRight = {ACTION_SET_COLOR_RIGHT};
msg speed = {ACTION_SPEED};
msg data;

// Button Actions
msg background = {ACTION_SET_BACKGROUND, DEFAULT_VIZ};
msg colorMode = {ACTION_CYCLE_COLOR_MODE};
msg strobeOff = {ACTION_STROBE_OFF};
msg strobeOn = {ACTION_STROBE_ON};

Button buttonExplode = {BUTTON_EXPLODE, "EXPLODE", false};
Button buttonTwinkle = {BUTTON_TWINKLE, "TWINKLE", false};
Button buttonSpin = {BUTTON_SPIN, "SPIN", false};
Button buttonFlash = {BUTTON_FLASH, "FLASH", false};
Button buttonColorGradient = {BUTTON_COLOR_GRADIENT, "COLOR GRADIENT", false};
Button buttonColorWheel = {BUTTON_COLOR_WHEEL, "COLOR WHEEL", false};
Button buttonColorSolid = {BUTTON_COLOR_SOLID, "COLOR SOLID", false};

Slider slider1 = {SLIDER_1, "BRIGHTNESS"};
Slider slider2 = {SLIDER_2, "COLOR LEFT"};
Slider slider3 = {SLIDER_3, "COLOR RIGHT"};
Slider slider4 = {SLIDER_4, "SPEED"};

Timer backgroundTimer = {backgroundCycleTime, 0};

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_add_peer(ss_broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(ss_broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(ss_broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // Shift Register
  pinMode(SR_LOAD, OUTPUT);
  pinMode(SR_CLOCK_ENABLE, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_DATA, INPUT);

  // Knobs
  pinMode(SLIDER_1, OUTPUT);
  pinMode(SLIDER_2, OUTPUT);
  pinMode(SLIDER_3, OUTPUT);
  pinMode(SLIDER_4, OUTPUT);
  pinMode(A0, INPUT);

  sendSliderValues();
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));

  if (data.action == REQUEST_SLIDER_VALUES) {
    sendSliderValues();
  }
}

void cycleBackground() {
  int numBackgrounds = sizeof(backgrounds) / sizeof(backgrounds[0]);
  int currentIndex = 0;
  for (int i = 0; i < numBackgrounds; i++) {
    if (backgrounds[i] == background.value) {
      currentIndex = i;
    }
  }
  int newIndex = currentIndex < numBackgrounds - 1 ? currentIndex + 1 : 0;
  background.value = backgrounds[newIndex];
}

void loop() {
  // Every N seconds, cycle through the active background viz
  if (backgroundTimer.complete()) {
    cycleBackground();
    send(background);
    backgroundTimer.reset();
  }

  EVERY_N_MILLISECONDS(10) {
    if (sliderIndex == -1) {
      digitalWrite(slider1.pin, HIGH);
    } else if (sliderIndex == 0) {
      readSliderSetNext(slider1, slider2);
    } else if (sliderIndex == 1) {
      readSliderSetNext(slider2, slider3);
    } else if (sliderIndex == 2) {
      readSliderSetNext(slider3, slider4);
    } else if (sliderIndex == 3) {
      readSliderSetNext(slider4, slider1);
    }
    sliderIndex = sliderIndex == 3 ? 0 : sliderIndex + 1;
  }

  checkSliderChanged(slider1);
  checkSliderChanged(slider2);
  checkSliderChanged(slider3);
  checkSliderChanged(slider4);

  byte shiftRegisterState = getShiftRegisterState();
  EVERY_N_MILLISECONDS(5) {
    checkButtonPressed(shiftRegisterState, buttonFlash);
    checkButtonPressed(shiftRegisterState, buttonTwinkle);
    checkButtonPressed(shiftRegisterState, buttonExplode);
    checkButtonPressed(shiftRegisterState, buttonSpin);
    checkButtonPressed(shiftRegisterState, buttonColorGradient);
    checkButtonPressed(shiftRegisterState, buttonColorWheel);
    checkButtonPressed(shiftRegisterState, buttonColorSolid);
  }
}
