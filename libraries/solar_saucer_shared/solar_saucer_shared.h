#define ACTION_STROBE_ON   0
#define ACTION_STROBE_OFF  1

typedef struct msg {
  uint8_t action;
  int value1;
  int value2;
} msg;
