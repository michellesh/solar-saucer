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
  fadeToBlackBy(dotsOuter, LEDS_OUTER, 20);
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    fadeToBlackBy(leds[strand], STRAND_LENGTH, 20);
  }

  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    if (explodePixel < STRAND_LENGTH) {
      int pixel = (int)explodePixel;
      leds[strand][pixel] = getStrandGradientColor(strand, pixel);
    }
  }

  if (explodePixel == 0) {
    exploded = false;
    for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
      if (isCloseToInnerCorner(innerX[pixel], innerY[pixel])) {
        dotsInner[pixel] = getInnerGradientColor(pixel);
      }
    }
  } else if (!exploded && explodePixel > STRAND_LENGTH) {
    exploded = true;
    for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
      if (isCloseToOuterCorner(outerX[pixel], outerY[pixel])) {
        dotsOuter[pixel] = getOuterGradientColor(pixel);
      }
    }
  }

  explodePixel = explodePixel >= 75 ? 0 : explodePixel + speed;
}
