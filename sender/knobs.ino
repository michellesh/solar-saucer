auto knobToBrightness = scale(1000, 0, 0, 255, true);
auto knobToHue = scale(1000, 0, 255, 0, true);
auto knobToSpeed = scale(1000, 0, 1, 11, true);

bool knobValueChanged(Knob knob) {
  int BUFFER = 20;
  return knob.value < (knob.prev - BUFFER) || knob.value > (knob.prev + BUFFER);
}

void checkKnobChanged(Knob &knob) {
  if (knobValueChanged(knob)) {
    onKnobChanged(knob);
    printKnobText(knob);
  }
}

void onKnobChanged(Knob &knob) {
  switch (knob.pin) {
    case KNOB_1:
      brightness.value = knobToBrightness(knob.value);
      send(brightness);
      break;
    case KNOB_2:
      colorLeft.value = knobToHue(knob2.value);
      send(colorLeft);
      break;
    case KNOB_3:
      colorRight.value = knobToHue(knob3.value);
      send(colorRight);
      break;
    case KNOB_4:
      speed.value = knobToSpeed(knob4.value);
      send(speed);
      break;
    default: break;
  }
  knob.prev = knob.value;
}

void printKnobText(Knob &knob) {
  int value = knob.pin == KNOB_1 ? brightness.value
    : knob.pin == KNOB_2 ? colorLeft.value
    : knob.pin == KNOB_3 ? colorRight.value
    : speed.value;

  Serial.print(knob.description);
  Serial.print(" changed: ");
  Serial.println(value);
}

void readKnobSetNext(Knob &knobOff, Knob &knobOn) {
  knobOff.value = analogRead(0);
  digitalWrite(knobOff.pin, LOW);
  digitalWrite(knobOn.pin, HIGH);
}

void sendKnobValues() {
  digitalWrite(knob1.pin, HIGH);
  delay(100);

  readKnobSetNext(knob1, knob2);
  delay(100);

  readKnobSetNext(knob2, knob3);
  delay(100);

  readKnobSetNext(knob3, knob4);
  delay(100);

  knob4.value = analogRead(0);
  digitalWrite(knob4.pin, LOW);

  onKnobChanged(knob1);
  delay(100);

  onKnobChanged(knob2);
  delay(100);

  onKnobChanged(knob3);
  delay(100);

  onKnobChanged(knob4);
  delay(100);
}
