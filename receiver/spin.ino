int angle = 240;
int startAngle = 360 * 2 / 3;
int outerEndAngle = 360 / 3;

int getStrandAngle(int strand) {
  int c = xyMax / 2; // both cx and cy (circle origin)
  return degrees(atan2(ledY[strand][0] - c, ledX[strand][0] - c)) + 90;
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
