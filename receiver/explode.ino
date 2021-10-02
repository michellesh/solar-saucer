//auto scaleExplodeSpeed = scale(1, 10, 0.5, 2.0, true);
auto scaleExplodeSpeed = scale(1, 10, 0.5, 3.0, true);
auto fadeIn = scale(0, explodeLength / 2, 0, 255, true);
auto fadeOut = scale(0, explodeLength / 2, 255, 0, true);

bool isPixelOnStrand(int pixel) {
  return pixel >= 0 && pixel < STRAND_LENGTH;
}

void vizExplode(float speed) {
  int startPixel = (int)explodePixel;
  int endPixel = startPixel + explodeLength;
  int middlePixel = startPixel + (explodeLength / 2);
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for (int pixel = startPixel; pixel < middlePixel; pixel++) {
      if (isPixelOnStrand(pixel)) {
        int b = fadeIn(pixel - startPixel);
        leds[strand][pixel] = getStrandModeColor(strand, pixel).nscale8(b);
      }
    }
    for (int pixel = middlePixel; pixel < endPixel; pixel++) {
      if (isPixelOnStrand(pixel)) {
        int b = fadeOut(pixel - middlePixel);
        leds[strand][pixel] = getStrandModeColor(strand, pixel).nscale8(b);
      }
    }
  }

  explodePixel += scaleExplodeSpeed(speed);
}
