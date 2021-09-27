#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#include "solar_saucer_shared.h"
#include "secrets.h"

#define SLIDER_1       15  // D8
#define SLIDER_2       5   // D1
#define SLIDER_3       16  // D0
#define SLIDER_4       0   // D3

#define RED_BUTTON     2   // D4
#define BLUE_BUTTON    14  // D5
#define YELLOW_BUTTON  12  // D6
#define GREEN_BUTTON   13  // D7
#define WHITE_BUTTON   4   // D2

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
int colorMode = DEFAULT_COLOR_MODE;
int sliderIndex = -1;
unsigned long backgroundCycleTime = 1000 * 60 * 4; // 4 minutes
unsigned long explodeFrequency[] = {2000, 2000, 1750, 1500, 1250, 1000,
                                    900,  800,  700,  600, 500};

// Slider Actions
msg brightness = {ACTION_SET_BRIGHTNESS};
msg colorLeft = {ACTION_SET_COLOR_LEFT};
msg colorRight = {ACTION_SET_COLOR_RIGHT};
msg speed = {ACTION_SPEED};
msg data;

// Button Actions
msg background = {ACTION_SET_BACKGROUND, DEFAULT_VIZ};
msg cycleColorMode = {ACTION_CYCLE_COLOR_MODE};
msg strobeOff = {ACTION_STROBE_OFF};
msg strobeOn = {ACTION_STROBE_ON};

Button redButton = {RED_BUTTON, "CYCLE COLOR MODE", false};
Button blueButton = {BLUE_BUTTON, "EXPLODE", false};
Button yellowButton = {YELLOW_BUTTON, "TWINKLE", false};
Button greenButton = {GREEN_BUTTON, "SPIN", false};
Button whiteButton = {WHITE_BUTTON, "STROBE", false};

Slider slider1 = {SLIDER_1, "BRIGHTNESS"};
Slider slider2 = {SLIDER_2, "COLOR LEFT"};
Slider slider3 = {SLIDER_3, "COLOR RIGHT"};
Slider slider4 = {SLIDER_4, "SPEED"};

Timer backgroundTimer = {backgroundCycleTime, 0};
Timer explodeTimer = {explodeFrequency[DEFAULT_SPEED], 0};

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

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
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

  // Sender must initiate each explosion for all boards to stay in sync
  if (background.value == VIZ_EXPLODE && explodeTimer.complete()) {
    send(background);
    explodeTimer.reset();
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

  EVERY_N_MILLISECONDS(5) {
    checkButtonPressed(whiteButton);
    checkButtonPressed(yellowButton);
    checkButtonPressed(blueButton);
    checkButtonPressed(greenButton);
    checkButtonPressed(redButton);
  }
}
