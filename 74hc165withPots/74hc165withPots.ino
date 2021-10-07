#define D0  16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15

#define SR_D0  7
#define SR_D1  6
#define SR_D2  5
#define SR_D3  4
#define SR_D4  3
#define SR_D5  2
#define SR_D6  1
#define SR_D7  0

int SR_LOAD = D8;
int SR_CLOCK_ENABLE = D5;
int SR_DATA = D6;
int SR_CLOCK = D7;

int KNOB_RED = D0;
int KNOB_WHITE = D1;

int BUTTON_YELLOW = SR_D4;
int BUTTON_BLUE = SR_D5;

void setup() {
  Serial.begin(115200);

  // Shift Register
  pinMode(SR_LOAD, OUTPUT);
  pinMode(SR_CLOCK_ENABLE, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_DATA, INPUT);

  // Potentiometers
  pinMode(KNOB_WHITE, OUTPUT);
  pinMode(KNOB_RED, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // Write pulse to load pin
  digitalWrite(SR_LOAD, LOW);
  delayMicroseconds(5);
  digitalWrite(SR_LOAD, HIGH);
  delayMicroseconds(5);

  // Get data from 74HC165
  digitalWrite(SR_CLOCK, HIGH);
  digitalWrite(SR_CLOCK_ENABLE, LOW);
  byte incoming = shiftIn(SR_DATA, SR_CLOCK, LSBFIRST);
  digitalWrite(SR_CLOCK_ENABLE, HIGH);

  // Print to serial monitor
  for (int i = 0; i < 8; i++) {
    bool b = bitRead(incoming, i);
    Serial.print(b);
  }
  Serial.println();

  Serial.print("Yellow button: ");
  if (bitRead(incoming, BUTTON_YELLOW)) {
    Serial.println("OFF");
  } else {
    Serial.println("ON");
  }
  Serial.print("Blue button: ");
  if (bitRead(incoming, BUTTON_BLUE)) {
    Serial.println("OFF");
  } else {
    Serial.println("ON");
  }

  // Potentiometers
  digitalWrite(KNOB_WHITE, HIGH);
  delay(100);
  int knobWhiteValue = analogRead(0);
  digitalWrite(KNOB_WHITE, LOW);

  digitalWrite(KNOB_RED, HIGH);
  delay(100);
  int knobRedValue = analogRead(0);
  digitalWrite(KNOB_RED, LOW);

  // Print pot values
  Serial.print("White knob: ");
  Serial.println(knobWhiteValue);
  Serial.print("Red knob: ");
  Serial.println(knobRedValue);

  Serial.println();
  delay(500);
}
