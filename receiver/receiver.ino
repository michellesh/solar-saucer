#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "xy.h"
#include "solar_saucer_shared.h"
#include "secrets.h"
#include "utils.h"
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>
#include "colors.h"

#define BRIGHTNESS      255
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2813

#define DATA_PIN_0  0
#define DATA_PIN_1  1
#define DATA_PIN_2  2
#define DATA_PIN_3  3
#define DATA_PIN_4  4
#define DATA_PIN_5  5
#define DATA_PIN_6  6
#define DATA_PIN_7  7
#define DATA_PIN_8  8

#define GPIO_0   0   // D3 data (yellow wire on dots)
#define GPIO_2   2   // D4 clock (green wire on dots)
#define GPIO_13  13  // D7 data (yellow wire on dots)
#define GPIO_15  15  // D8 clock (green wire on dots)

#define LEDS_12  50  // Strip 13
#define LEDS_13  35
#define LEDS_14  35
#define LEDS_15  35
#define LEDS_16  35
#define LEDS_17  49

#define LEDS_INNER  60
#define LEDS_OUTER  120

CRGB leds_0[50];  // Strip 1
CRGB leds_1[50];
CRGB leds_2[50];
CRGB leds_3[50];
CRGB leds_4[50];
CRGB leds_5[50];
CRGB leds_6[50];
CRGB leds_7[50];
CRGB leds_8[50];
CRGB leds_9[50];
CRGB leds_10[50];
CRGB leds_11[50];
CRGB leds_12[LEDS_12];
CRGB leds_13[LEDS_13];
CRGB leds_14[LEDS_14];
CRGB leds_15[LEDS_15];
CRGB leds_16[LEDS_16];
CRGB leds_17[LEDS_17];
CRGB *leds[] = {
  leds_0, leds_1, leds_2, leds_3, leds_4, leds_5, leds_6,
  leds_7, leds_8, leds_9, leds_10, leds_11, leds_12, leds_13,
  leds_14, leds_15, leds_16, leds_17
};
CRGB dotsInner[LEDS_INNER];
CRGB dotsOuter[LEDS_OUTER];

byte boardNumber;
bool strobeOn = false;
uint8_t brightness = BRIGHTNESS;
uint8_t activeViz = VIZ_DEFAULT;
uint8_t speed = 1;
CRGB colorLeft = CRGB::White;
CRGB colorRight = CRGB::White;
msg data;

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  boardNumber = WiFi.macAddress() == ss_macAddress1   ? 1
                : WiFi.macAddress() == ss_macAddress2 ? 2
                : WiFi.macAddress() == ss_macAddress3 ? 3
                                                      : 0;
  if (boardNumber == 0) {
    Serial.print("MAC address not found: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.print("Board number: ");
    Serial.println(boardNumber);
  }

  if (boardNumber == 1) {  // Blue group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[0], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[1], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[16], LEDS_16)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[17], LEDS_17)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

    FastLED.addLeds<WS2801, DATA_PIN_3, DATA_PIN_4, RGB>(dotsInner, LEDS_INNER);
    FastLED.addLeds<WS2801, DATA_PIN_7, DATA_PIN_8, RGB>(dotsOuter, LEDS_OUTER);

  } else if (boardNumber == 2) {  // Green group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[2], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[3], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[4], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[5], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[6], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[7], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_7, COLOR_ORDER>(leds[8], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_8, COLOR_ORDER>(leds[9], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

  } else if (boardNumber == 3) {  // Red group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[10], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[11], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[12], LEDS_12)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[13], LEDS_13)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[14], LEDS_14)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[15], LEDS_15)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
  }

  FastLED.setBrightness(BRIGHTNESS);

  chooseNextColorPalette(gTargetPalette);
}

// Create a 24 bit color value from R,G,B
uint8_t getNumPixels(uint8_t strandNumber) {
  return strandNumber == 12 ? LEDS_12
         : strandNumber == 13 ? LEDS_13
         : strandNumber == 14 ? LEDS_14
         : strandNumber == 15 ? LEDS_15
         : strandNumber == 16 ? LEDS_16
         : strandNumber == 17 ? LEDS_17
         : 50;
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println(data.action);

  // SLIDER ACTIONS
  if (data.action == ACTION_SET_BRIGHTNESS) {
    Serial.print("ACTION_SET_BRIGHTNESS");
    Serial.println(data.value);
    brightness = (uint8_t)data.value;

  } else if (data.action == ACTION_SET_COLOR_LEFT) {
    Serial.print("ACTION_SET_COLOR_LEFT ");
    Serial.println(data.value);
    colorLeft = CHSV(data.value, 255, 255);

    int paletteIndex = map(data.value, 0, 255, 0, 8);
    gCurrentPalette = *(ActivePaletteList[paletteIndex]);

  } else if (data.action == ACTION_SET_COLOR_RIGHT) {
    Serial.print("ACTION_SET_COLOR_RIGHT ");
    Serial.println(data.value);
    colorRight = CHSV(data.value, 255, 255);

    int paletteIndex = map(data.value, 0, 255, 0, 8);
    gCurrentPalette = *(ActivePaletteList[paletteIndex]);

  } else if (data.action == ACTION_SPEED) {
    Serial.print("ACTION_SPEED");
    Serial.println(data.value);
    speed = data.value;

  // BUTTON ACTIONS
  } else if (data.action == ACTION_SET_BACKGROUND) {
    Serial.print("ACTION_SET_BACKGROUND");
    Serial.println(data.value);
    activeViz = data.value;

  } else if (data.action == ACTION_STROBE_ON) {
    Serial.print("ACTION_STROBE_ON");
    strobeOn = true;

  } else if (data.action == ACTION_STROBE_OFF) {
    Serial.print("ACTION_STROBE_OFF");
    strobeOn = false;

  // AUTO ACTIONS
  } else if (data.action == ACTION_SET_PALETTE) {
    Serial.print("ACTION_SET_PALETTE ");
    Serial.println(data.value);

    gTargetPalette = *(ActivePaletteList[data.value]);
  }
}

void loop() {

  // Background pattern
  if (activeViz == VIZ_DEFAULT) {
    setAllColor(CRGB(0, 0, 0));
  } else if (activeViz == VIZ_EXPLODE) {
    float explodeSpeed = mapf(speed, 1, 10, 0.2, 2.0);
    vizExplode(explodeSpeed);
  } else if (activeViz == VIZ_SPIN) {
    vizSpin();
  } else if (activeViz == VIZ_TWINKLE) {
    EVERY_N_MILLISECONDS(10) {
      nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, 12);
    }
    vizTwinkle(mapf(speed, 1, 10, 4, 9));
  }

  // Strobe
  if (strobeOn) {
    setAllGradient(colorLeft, colorRight);
  }

  // Brightness
  setAllBrightness(brightness);

  FastLED.show();
}

void setAllBrightness(uint8_t b) {
  for(uint8_t strand = 0; strand < 18; strand++) {
    for(uint8_t pixel = 0; pixel < getNumPixels(strand); pixel++) {
      leds[strand][pixel].nscale8(b);
    }
  }
  for(uint8_t pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel].nscale8(b);
  }
  for(uint8_t pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel].nscale8(b);
  }
}

void setAllColor(CRGB color) {
  for(uint8_t strand = 0; strand < 18; strand++) {
    for(uint8_t pixel = 0; pixel < getNumPixels(strand); pixel++) {
      leds[strand][pixel] = color;
    }
  }
  for(uint8_t pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel] = color;
  }
  for(uint8_t pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel] = color;
  }
}

CRGB getColorAlongGradient(CRGB color1, CRGB color2, float percent) {
  return CRGB(
    color1.r + percent * (color2.r - color1.r),
    color1.g + percent * (color2.g - color1.g),
    color1.b + percent * (color2.b - color1.b)
  );
}

void setAllGradient(CRGB colorLeft, CRGB colorRight) {
  float percent;
  for(uint8_t strand = 0; strand < 18; strand++) {
    for(uint8_t pixel = 0; pixel < getNumPixels(strand); pixel++) {
      percent = (float)(ledX[strand][pixel] / (float)xyMax);
      leds[strand][pixel] = getColorAlongGradient(colorLeft, colorRight, percent);
    }
  }
  for(uint8_t pixel = 0; pixel < LEDS_INNER; pixel++) {
    percent = (float)innerX[pixel] / (float)xyMax;
    dotsInner[pixel] = getColorAlongGradient(colorLeft, colorRight, percent);
  }
  for(uint8_t pixel = 0; pixel < LEDS_OUTER; pixel++) {
    percent = (float)outerX[pixel] / (float)xyMax;
    dotsOuter[pixel] = getColorAlongGradient(colorLeft, colorRight, percent);
  }
}

void vizSpin() {
}
