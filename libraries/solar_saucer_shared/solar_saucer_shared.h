// Slider Actions
#define ACTION_SET_BRIGHTNESS    0
#define ACTION_SET_COLOR_LEFT    1
#define ACTION_SET_COLOR_RIGHT   2
#define ACTION_SPEED             3

// Button Actions
#define ACTION_CYCLE_COLOR_MODE  4
#define ACTION_SET_BACKGROUND    5
#define ACTION_STROBE_OFF        6
#define ACTION_STROBE_ON         7

// Auto Actions
#define ACTION_SET_PALETTE       8

// Visualizations
#define VIZ_DEFAULT  0
#define VIZ_TWINKLE  1
#define VIZ_EXPLODE  2
#define VIZ_SPIN     3

typedef struct msg {
  uint8_t action;
  int value;
} msg;

auto scale(float domainStart, float domainEnd, float rangeStart, float rangeEnd, bool clamp = false) {
  return [=](float value) {
    float percentage = (value - domainStart) / (domainEnd - domainStart);
    float newValue = rangeStart + (rangeEnd - rangeStart) * percentage;
    return clamp && newValue < min(rangeStart, rangeEnd)   ? min(rangeStart, rangeEnd)
           : clamp && newValue > max(rangeStart, rangeEnd) ? max(rangeStart, rangeEnd)
                                                           : newValue;
  };
}
