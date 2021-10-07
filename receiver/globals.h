#define BRIGHTNESS      255
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2813
#define NUM_STRANDS     18
#define STRAND_LENGTH   47

// When we installed the strands, the first pixel of strand 18
// didn't line up with the first pixel of strand 1, so start
// strand 18 at an offset
#define STRAND_18_OFFSET  14

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

#define LEDS_INNER  74
#define LEDS_OUTER  111

CRGB leds_0[STRAND_LENGTH];  // Strip 1
CRGB leds_1[STRAND_LENGTH];
CRGB leds_2[STRAND_LENGTH];
CRGB leds_3[STRAND_LENGTH];
CRGB leds_4[STRAND_LENGTH];
CRGB leds_5[STRAND_LENGTH];
CRGB leds_6[STRAND_LENGTH];
CRGB leds_7[STRAND_LENGTH];
CRGB leds_8[STRAND_LENGTH];
CRGB leds_9[STRAND_LENGTH];
CRGB leds_10[STRAND_LENGTH];
CRGB leds_11[STRAND_LENGTH];
CRGB leds_12[STRAND_LENGTH];
CRGB leds_13[STRAND_LENGTH];
CRGB leds_14[STRAND_LENGTH];
CRGB leds_15[STRAND_LENGTH];
CRGB leds_16[STRAND_LENGTH];
CRGB leds_17[STRAND_LENGTH];
CRGB *leds[] = {
  leds_0, leds_1, leds_2, leds_3, leds_4, leds_5, leds_6,
  leds_7, leds_8, leds_9, leds_10, leds_11, leds_12, leds_13,
  leds_14, leds_15, leds_16, leds_17
};
CRGB dotsInner[LEDS_INNER];
CRGB dotsOuter[LEDS_OUTER];

int pixelAngles_0[STRAND_LENGTH];
int pixelAngles_1[STRAND_LENGTH];
int pixelAngles_2[STRAND_LENGTH];
int pixelAngles_3[STRAND_LENGTH];
int pixelAngles_4[STRAND_LENGTH];
int pixelAngles_5[STRAND_LENGTH];
int pixelAngles_6[STRAND_LENGTH];
int pixelAngles_7[STRAND_LENGTH];
int pixelAngles_8[STRAND_LENGTH];
int pixelAngles_9[STRAND_LENGTH];
int pixelAngles_10[STRAND_LENGTH];
int pixelAngles_11[STRAND_LENGTH];
int pixelAngles_12[STRAND_LENGTH];
int pixelAngles_13[STRAND_LENGTH];
int pixelAngles_14[STRAND_LENGTH];
int pixelAngles_15[STRAND_LENGTH];
int pixelAngles_16[STRAND_LENGTH];
int pixelAngles_17[STRAND_LENGTH];
int *pixelAngles[] = {
  pixelAngles_0, pixelAngles_1, pixelAngles_2, pixelAngles_3, pixelAngles_4,
  pixelAngles_5, pixelAngles_6, pixelAngles_7, pixelAngles_8, pixelAngles_9,
  pixelAngles_10, pixelAngles_11, pixelAngles_12, pixelAngles_13,
  pixelAngles_14, pixelAngles_15, pixelAngles_16, pixelAngles_17
};
int pixelAnglesInner[LEDS_INNER];
int pixelAnglesOuter[LEDS_OUTER];

uint8_t brightness = BRIGHTNESS;
uint8_t activeViz = DEFAULT_VIZ;
uint8_t speed = DEFAULT_SPEED;
int colorMode = DEFAULT_COLOR_MODE;
CRGB activeColor = DEFAULT_COLOR;
CRGB colorLeft = DEFAULT_COLOR;
CRGB colorRight = DEFAULT_COLOR;

bool strobeOn = false;
int spinAngle = 0;
int explodeLength = 20;
float explodePixel = 0;

byte boardNumber;
msg data;
