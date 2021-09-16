float explodePixel = 0;
bool exploded = false;

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
  for (int strand = 0; strand < 18; strand++) {
    fadeToBlackBy(leds[strand], getNumPixels(strand), 20);
  }

  for (int strand = 0; strand < 18; strand++) {
    if (explodePixel < getNumPixels(strand)) {
      leds[strand][(int)explodePixel] = CRGB::White;
    }
  }

  if (explodePixel == 0) {
    exploded = false;
    for(uint8_t pixel = 0; pixel < LEDS_INNER; pixel++) {
      if (isCloseToInnerCorner(innerX[pixel], innerY[pixel])) {
        dotsInner[pixel] = CRGB::White;
      }
    }
  } else if (!exploded && explodePixel > 50) {
    exploded = true;
    for(uint8_t pixel = 0; pixel < LEDS_OUTER; pixel++) {
      if (isCloseToOuterCorner(outerX[pixel], outerY[pixel])) {
        dotsOuter[pixel] = CRGB::White;
      }
    }
  }

  explodePixel = explodePixel >= 75 ? 0 : explodePixel + speed;
}
