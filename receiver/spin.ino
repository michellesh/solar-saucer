int pixelAngles_0[50];
int pixelAngles_1[50];
int pixelAngles_2[50];
int pixelAngles_3[50];
int pixelAngles_4[50];
int pixelAngles_5[50];
int pixelAngles_6[50];
int pixelAngles_7[50];
int pixelAngles_8[50];
int pixelAngles_9[50];
int pixelAngles_10[50];
int pixelAngles_11[50];
int pixelAngles_12[LEDS_12];
int pixelAngles_13[LEDS_13];
int pixelAngles_14[LEDS_14];
int pixelAngles_15[LEDS_15];
int pixelAngles_16[LEDS_16];
int pixelAngles_17[LEDS_17];
int *pixelAngles[] = {
  pixelAngles_0, pixelAngles_1, pixelAngles_2, pixelAngles_3, pixelAngles_4,
  pixelAngles_5, pixelAngles_6, pixelAngles_7, pixelAngles_8, pixelAngles_9,
  pixelAngles_10, pixelAngles_11, pixelAngles_12, pixelAngles_13,
  pixelAngles_14, pixelAngles_15, pixelAngles_16, pixelAngles_17
};
int pixelAnglesInner[LEDS_INNER];
int pixelAnglesOuter[LEDS_OUTER];

void initPixelAngles() {
  for (int strand = 0; strand < 18; strand++) {
    for(int pixel = 0; pixel < getNumPixels(strand); pixel++) {
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
  for (int strand = 0; strand < 18; strand++) {
    fadeToBlackBy(leds[strand], getNumPixels(strand), 20);
  }

  for (int strand = 0; strand < 18; strand++) {
    for(int pixel = 0; pixel < getNumPixels(strand); pixel++) {
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
