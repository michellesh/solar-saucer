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

int KNOB_1 = D0;
int KNOB_2 = D1;
int KNOB_3 = D2;
int KNOB_4 = D3;

int BUTTON_1 = SR_D1;
int BUTTON_2 = SR_D2;
int BUTTON_3 = SR_D3;
int BUTTON_4 = SR_D4;
int BUTTON_5 = SR_D5;
int BUTTON_6 = SR_D6;
int BUTTON_7 = SR_D7;

void setup() {
  Serial.begin(115200);

  // Shift Register
  pinMode(SR_LOAD, OUTPUT);
  pinMode(SR_CLOCK_ENABLE, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_DATA, INPUT);

  // Potentiometers
  pinMode(KNOB_1, OUTPUT);
  pinMode(KNOB_2, OUTPUT);
  pinMode(KNOB_3, OUTPUT);
  pinMode(KNOB_4, OUTPUT);
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

  // Print button values
  Serial.print("Button 1: ");
  Serial.println(getButtonValue(incoming, BUTTON_1));
  Serial.print("Button 2: ");
  Serial.println(getButtonValue(incoming, BUTTON_2));
  Serial.print("Button 3: ");
  Serial.println(getButtonValue(incoming, BUTTON_3));
  Serial.print("Button 4: ");
  Serial.println(getButtonValue(incoming, BUTTON_4));
  Serial.print("Button 5: ");
  Serial.println(getButtonValue(incoming, BUTTON_5));
  Serial.print("Button 6: ");
  Serial.println(getButtonValue(incoming, BUTTON_6));
  Serial.print("Button 7: ");
  Serial.println(getButtonValue(incoming, BUTTON_7));

  // Print knob values
  Serial.print("Knob 1: ");
  Serial.println(getKnobValue(KNOB_1));
  Serial.print("Knob 2: ");
  Serial.println(getKnobValue(KNOB_2));
  Serial.print("Knob 3: ");
  Serial.println(getKnobValue(KNOB_3));
  Serial.print("Knob 4: ");
  Serial.println(getKnobValue(KNOB_4));

  Serial.println();
  delay(500);
}

int getKnobValue(int knob) {
  digitalWrite(knob, HIGH);
  delay(100);
  int knobValue = analogRead(0);
  digitalWrite(knob, LOW);
  return knobValue;
}

int getButtonValue(byte incoming, int button) {
  return bitRead(incoming, button) ? 0 : 1;
}

void printByte(byte incoming) {
  for (int i = 0; i < 8; i++) {
    bool b = bitRead(incoming, i);
    Serial.print(b);
  }
  Serial.println();
}
