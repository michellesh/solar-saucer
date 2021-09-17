int angle = 240;
int startAngle = 360 * 2 / 3;
int outerEndAngle = 360 / 3;

int getStrandAngle(int strand) {
  switch (strand) {
    case 0: return 360 * 6 / 9;
    case 1: return 360 * 7 / 9;
    case 2: return 360 * 7 / 9;
    case 3: return 360 * 8 / 9;
    case 4: return 360 * 8 / 9;
    case 5: return 0;
    case 6: return 0;
    case 7: return 360 * 1 / 9;
    case 8: return 360 * 1 / 9;
    case 9: return 360 * 2 / 9;
    case 10: return 360 * 2 / 9;
    case 11: return 360 * 3 / 9;
    case 12: return 360 * 3 / 9;
    case 13: return 360 * 4 / 9;
    case 14: return 360 * 4 / 9;
    case 15: return 360 * 5 / 9;
    case 16: return 360 * 5 / 9;
    case 17: return 360 * 6 / 9;
  }
}

int getInnerDotAngle(int pixel) {
  return pixel < LEDS_INNER / 3
    ? map(pixel, 0, LEDS_INNER / 3, startAngle, 360)
    : map(pixel, LEDS_INNER / 3, LEDS_INNER, 0, startAngle);
}

int getOuterDotAngle(int pixel) {
  return pixel < LEDS_OUTER / 2
    ? map(pixel, 0, LEDS_OUTER / 2, startAngle, 360)
    : map(pixel, LEDS_OUTER / 2, LEDS_OUTER, 0, outerEndAngle);
}

void vizSpin(float speed) {
  fadeToBlackBy(dotsInner, LEDS_INNER, 20);
  fadeToBlackBy(dotsOuter, LEDS_OUTER, 20);
  for (int strand = 0; strand < 18; strand++) {
    fadeToBlackBy(leds[strand], getNumPixels(strand), 20);
  }

  for (int strand = 0; strand < 18; strand++) {
    if (abs(angle - getStrandAngle(strand)) < 5) {
      for(int pixel = 0; pixel < getNumPixels(strand); pixel++) {
        leds[strand][pixel] = getStrandGradientColor(strand, pixel);
      }
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    if (abs(angle - getInnerDotAngle(pixel)) < 5) {
      dotsInner[pixel] = getInnerGradientColor(pixel);
    }
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    if (abs(angle - getOuterDotAngle(pixel)) < 5) {
      dotsOuter[pixel] = getOuterGradientColor(pixel);
    }
  }

  angle = angle < 360 ? angle + speed : 0;
}
