uint8_t explodeRingBrightness = 255 / 4;
auto explodeScaleSpeed = scale(1, 10, 0.5, 3.0, true);
auto explodeFadeIn = scale(0, explodeLength / 2, 0, 255, true);
auto explodeFadeOut = scale(0, explodeLength / 2, 255, 0, true);

int getOffsetPixel(int pixel) {
  return pixel < STRAND_LENGTH ? pixel : pixel - STRAND_LENGTH;
}

void advanceExplodePixel() {
  explodePixel += explodeScaleSpeed(speed);
  if (explodePixel > STRAND_LENGTH) {
    explodePixel -= STRAND_LENGTH;
  }
}

void setInnerOuterRings() {
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel] = getInnerModeColor(pixel).nscale8(explodeRingBrightness);
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel] = getOuterModeColor(pixel).nscale8(explodeRingBrightness);
  }
}

void vizExplode(float speed) {
  setInnerOuterRings();

  int startPixel = (int)explodePixel;
  int middlePixel = startPixel + (explodeLength / 2);
  int endPixel = startPixel + explodeLength;

  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for (int pixel = startPixel; pixel < middlePixel; pixel++) {
      int b = explodeFadeIn(pixel - startPixel);
      int p = getOffsetPixel(pixel);
      leds[strand][p] = getStrandModeColor(strand, p).nscale8(b);
    }
    for (int pixel = middlePixel; pixel < endPixel; pixel++) {
      int b = explodeFadeOut(pixel - middlePixel);
      int p = getOffsetPixel(pixel);
      leds[strand][p] = getStrandModeColor(strand, p).nscale8(b);
    }
  }

  advanceExplodePixel();
}
