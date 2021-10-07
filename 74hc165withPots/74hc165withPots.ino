#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

#define D0  16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15

#define SR_D1  6
#define SR_D2  5
#define SR_D3  4
#define SR_D4  3
#define SR_D5  2
#define SR_D6  1
#define SR_D7  0

#define NUM_LEDS  3
#define LEDS_PIN  4

int SR_LOAD = D8;
int SR_CLOCK_ENABLE = D5;
int SR_DATA = D6;
int SR_CLOCK = D7;

int KNOB_COLOR_1 = D0;
int KNOB_BRIGHTNESS = D1;
int KNOB_COLOR_2 = D2;
int KNOB_SPEED = D3;

int BUTTON_FLASH = SR_D1;
int BUTTON_EXPLODE = SR_D2;
int BUTTON_TWINKLE = SR_D3;
int BUTTON_COLOR_MODE_GRADIENT = SR_D4;
int BUTTON_COLOR_MODE_WHEEL = SR_D5;
int BUTTON_SPIN = SR_D6;
int BUTTON_COLOR_MODE_SOLID = SR_D7;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);

  // Shift Register
  pinMode(SR_LOAD, OUTPUT);
  pinMode(SR_CLOCK_ENABLE, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_DATA, INPUT);

  // Potentiometers
  pinMode(KNOB_COLOR_1, OUTPUT);
  pinMode(KNOB_BRIGHTNESS, OUTPUT);
  pinMode(KNOB_COLOR_2, OUTPUT);
  pinMode(KNOB_SPEED, OUTPUT);
  pinMode(A0, INPUT);

  // LEDs
  FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
  // Write pulse to load pin
  digitalWrite(SR_LOAD, LOW);
  delayMicroseconds(5);
  digitalWrite(SR_LOAD, HIGH);
  delayMicroseconds(5);

  // Get data from 74HC165
  digitalWrite(SR_CLOCK, HIGH);
  digitalWrite(SR_CLOCK_ENABLE, LOW);
  byte incoming = shiftIn(SR_DATA, SR_CLOCK, LSBFIRST);
  digitalWrite(SR_CLOCK_ENABLE, HIGH);

  // Print button values
  Serial.print("Button 1: ");
  Serial.println(getButtonValue(incoming, BUTTON_FLASH));
  Serial.print("Button 2: ");
  Serial.println(getButtonValue(incoming, BUTTON_EXPLODE));
  Serial.print("Button 3: ");
  Serial.println(getButtonValue(incoming, BUTTON_TWINKLE));
  Serial.print("Button 4: ");
  Serial.println(getButtonValue(incoming, BUTTON_COLOR_MODE_GRADIENT));
  Serial.print("Button 5: ");
  Serial.println(getButtonValue(incoming, BUTTON_COLOR_MODE_WHEEL));
  Serial.print("Button 6: ");
  Serial.println(getButtonValue(incoming, BUTTON_SPIN));
  Serial.print("Button 7: ");
  Serial.println(getButtonValue(incoming, BUTTON_COLOR_MODE_SOLID));

  // Print knob values
  Serial.print("Knob 1: ");
  Serial.println(getKnobValue(KNOB_COLOR_1));
  Serial.print("Knob 2: ");
  Serial.println(getKnobValue(KNOB_BRIGHTNESS));
  Serial.print("Knob 3: ");
  Serial.println(getKnobValue(KNOB_COLOR_2));
  Serial.print("Knob 4: ");
  Serial.println(getKnobValue(KNOB_SPEED));

  showLEDs();

  Serial.println();
  delay(500);
}

int getKnobValue(int knob) {
  digitalWrite(knob, HIGH);
  delay(100);
  int knobValue = analogRead(0);
  digitalWrite(knob, LOW);
  return knobValue;
}

int getButtonValue(byte incoming, int button) {
  return bitRead(incoming, button) ? 0 : 1;
}

void printByte(byte incoming) {
  for (int i = 0; i < 8; i++) {
    bool b = bitRead(incoming, i);
    Serial.print(b);
  }
  Serial.println();
}

void showLEDs() {
  pride();
  FastLED.show();
}

// This function is taken from the FastLED example Pride2015
void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for(uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}
