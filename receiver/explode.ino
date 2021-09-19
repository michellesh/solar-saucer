int pixelWidth = 2;
auto explodeSpeed = scale(1, 10, 0.5, 2.0, true);

float distance(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool isCloseToInnerCorner(uint16_t x, uint16_t y) {
  for(int i = 0; i < numInnerCorners; i++) {
    if (distance(innerCorners[i].x, innerCorners[i].y, x, y) < 7) {
      return true;
    }
  }
  return false;
}

bool isCloseToOuterCorner(uint16_t x, uint16_t y) {
  for(int i = 0; i < numOuterCorners; i++) {
    if (distance(outerCorners[i].x, outerCorners[i].y, x, y) < 10) {
      return true;
    }
  }
  return false;
}

void vizExplode(float speed) {
  fadeToBlackBy(dotsInner, LEDS_INNER, 20);
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    fadeToBlackBy(leds[strand], STRAND_LENGTH, 20);
  }

  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    int width = min(STRAND_LENGTH, (int)explodePixel + pixelWidth);
    for (int pixel = (int)explodePixel; pixel < width; pixel++) {
      leds[strand][pixel] = getStrandModeColor(strand, pixel);
    }
  }

  if (explodePixel == 0) {
    for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
      if (isCloseToInnerCorner(innerX[pixel], innerY[pixel])) {
        dotsInner[pixel] = getInnerModeColor(pixel);
      }
    }
  }

  explodePixel += explodeSpeed(speed);
}
