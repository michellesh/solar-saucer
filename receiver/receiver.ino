#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#include "solar_saucer_shared.h"
#include "globals.h"
#include "xy.h"
#include "secrets.h"
#include "utils.h"
#include "palettes.h"

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
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[0], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[1], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[16], NUM_STRANDS)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[17], NUM_STRANDS)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

    FastLED.addLeds<WS2801, DATA_PIN_3, DATA_PIN_4, RGB>(dotsInner, LEDS_INNER);
    FastLED.addLeds<WS2801, DATA_PIN_7, DATA_PIN_8, RGB>(dotsOuter, LEDS_OUTER);

  } else if (boardNumber == 2) {  // Green group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[2], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[3], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[4], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[5], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[6], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[7], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_7, COLOR_ORDER>(leds[8], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_8, COLOR_ORDER>(leds[9], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

  } else if (boardNumber == 3) {  // Red group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[10], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[11], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[12], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[13], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[14], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[15], STRAND_LENGTH)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
  }

  FastLED.setBrightness(BRIGHTNESS);

  chooseNextColorPalette(gTargetPalette);
  initPixelAngles();
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println(data.action);

  // SLIDER ACTIONS
  if (data.action == ACTION_SET_BRIGHTNESS) {
    Serial.print("ACTION_SET_BRIGHTNESS: ");
    Serial.println(data.value);
    brightness = (uint8_t)data.value;

  } else if (data.action == ACTION_SET_COLOR_LEFT) {
    Serial.print("ACTION_SET_COLOR_LEFT: ");
    Serial.println(data.value);
    colorLeft = CHSV(data.value, 255, 255);
    activeColor = CHSV(data.value, 255, 255);

    int paletteIndex = map(data.value, 0, 255, 0, 8);
    gCurrentPalette = *(ActivePaletteList[paletteIndex]);

  } else if (data.action == ACTION_SET_COLOR_RIGHT) {
    Serial.print("ACTION_SET_COLOR_RIGHT: ");
    Serial.println(data.value);
    colorRight = CHSV(data.value, 255, 255);
    activeColor = CHSV(data.value, 255, 255);

    int paletteIndex = map(data.value, 0, 255, 0, 8);
    gCurrentPalette = *(ActivePaletteList[paletteIndex]);

  } else if (data.action == ACTION_SPEED) {
    Serial.print("ACTION_SPEED: ");
    Serial.println(data.value);
    speed = data.value;

  // BUTTON ACTIONS
  } else if (data.action == ACTION_SET_BACKGROUND) {
    Serial.print("ACTION_SET_BACKGROUND: ");
    Serial.println(data.value);
    activeViz = data.value;
    if (activeViz == VIZ_SPIN) {
      spinAngle = 0;
    } else if (activeViz == VIZ_EXPLODE) {
      explodePixel = 0;
      exploded = false;
    }

  } else if (data.action == ACTION_CYCLE_COLOR_MODE) {
    Serial.print("ACTION_CYCLE_COLOR_MODE: ");
    Serial.println(data.value);
    colorMode = data.value;

  } else if (data.action == ACTION_STROBE_ON) {
    Serial.print("ACTION_STROBE_ON");
    strobeOn = true;

  } else if (data.action == ACTION_STROBE_OFF) {
    Serial.print("ACTION_STROBE_OFF");
    strobeOn = false;

  // AUTO ACTIONS
  } else if (data.action == ACTION_SET_PALETTE) {
    Serial.print("ACTION_SET_PALETTE: ");
    Serial.println(data.value);

    gTargetPalette = *(ActivePaletteList[data.value]);
  }
}


void loop() {
  EVERY_N_MILLISECONDS(20) {
    // Background pattern
    if (activeViz == VIZ_EXPLODE) {
      float explodeSpeed = mapf(speed, 1, 10, 0.2, 2.0);
      vizExplode(explodeSpeed);
    } else if (activeViz == VIZ_SPIN) {
      vizSpin(speed);
    } else if (activeViz == VIZ_TWINKLE) {
      EVERY_N_MILLISECONDS(10) {
        nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, 12);
      }
      vizTwinkle(mapf(speed, 1, 10, 4, 9));
    } else {
      setAllColor(CRGB(0, 0, 0));
    }

    // Strobe
    if (strobeOn) {
      setAllColorMode();
    }

    // Brightness
    setAllBrightness(brightness);

    FastLED.show();
  }
}
