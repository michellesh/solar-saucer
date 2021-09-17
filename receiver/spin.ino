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

void initPixelAngles() {
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      int x = ledX[strand][pixel];
      int y = ledY[strand][pixel];
      pixelAngles[strand][pixel] = getPixelAngle(x, y);
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    int x = innerX[pixel];
    int y = innerY[pixel];
    pixelAnglesInner[pixel] = getPixelAngle(x, y);
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    int x = outerX[pixel];
    int y = outerY[pixel];
    pixelAnglesOuter[pixel] = getPixelAngle(x, y);
  }
}

int getPixelAngle(int x, int y) {
  int c = xyMax / 2; // both cx and cy (circle origin)
  int d = degrees(atan2(y - c, x - c)) + 90;
  return d < 0 ? 360 + d : d;
}

void vizSpin(float speed) {
  fadeToBlackBy(dotsInner, LEDS_INNER, 20);
  fadeToBlackBy(dotsOuter, LEDS_OUTER, 20);
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    fadeToBlackBy(leds[strand], STRAND_LENGTH, 20);
  }

  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      if (abs(spinAngle - pixelAngles[strand][pixel]) < 5) {
        leds[strand][pixel] = getStrandGradientColor(strand, pixel);
      }
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    if (abs(spinAngle - pixelAnglesInner[pixel]) < 5) {
      dotsInner[pixel] = getInnerGradientColor(pixel);
    }
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    if (abs(spinAngle - pixelAnglesOuter[pixel]) < 5) {
      dotsOuter[pixel] = getOuterGradientColor(pixel);
    }
  }

  spinAngle = spinAngle < 360 ? spinAngle + speed : 0;
}
