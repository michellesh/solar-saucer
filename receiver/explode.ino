auto scaleExplodeSpeed = scale(1, 10, 0.5, 3.0, true);
auto fadeIn = scale(0, explodeLength / 2, 0, 255, true);
auto fadeOut = scale(0, explodeLength / 2, 255, 0, true);

int getOffsetPixel(int pixel) {
  return pixel < STRAND_LENGTH ? pixel : pixel - STRAND_LENGTH;
}

void advanceExplodePixel() {
  explodePixel += scaleExplodeSpeed(speed);
  if (explodePixel > STRAND_LENGTH) {
    explodePixel -= STRAND_LENGTH;
  }
}

void vizExplode(float speed) {
  int startPixel = (int)explodePixel;
  int middlePixel = startPixel + (explodeLength / 2);
  int endPixel = startPixel + explodeLength;

  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for (int pixel = startPixel; pixel < middlePixel; pixel++) {
      int b = fadeIn(pixel - startPixel);
      int p = getOffsetPixel(pixel);
      leds[strand][p] = getStrandModeColor(strand, p).nscale8(b);
    }
    for (int pixel = middlePixel; pixel < endPixel; pixel++) {
      int b = fadeOut(pixel - middlePixel);
      int p = getOffsetPixel(pixel);
      leds[strand][p] = getStrandModeColor(strand, p).nscale8(b);
    }
  }

  advanceExplodePixel();
}
