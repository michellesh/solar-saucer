byte getShiftRegisterState() {
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

  return incoming;
}

bool isButtonPressed(byte shiftRegisterState, Button button) {
  return !bitRead(shiftRegisterState, button.pin);
}

void checkButtonPressed(byte shiftRegisterState, Button &button) {
  if (isButtonPressed(shiftRegisterState, button)) {
    if (!button.pressed) {
      button.pressed = true;
      printButtonText(button);
      onButtonPressed(button);
    }
  } else if (button.pressed) {
    button.pressed = false;
    onButtonUnpressed(button);
  }
}

void onButtonPressed(Button &button) {
  switch (button.pin) {
    case BUTTON_COLOR_GRADIENT:
      colorMode.value = COLOR_MODE_GRADIENT;
      send(colorMode);
      break;
    case BUTTON_COLOR_WHEEL:
      colorMode.value = COLOR_MODE_WHEEL;
      send(colorMode);
      break;
    case BUTTON_COLOR_SOLID:
      colorMode.value = COLOR_MODE_SOLID;
      send(colorMode);
      break;
    case BUTTON_EXPLODE:
      background.value = VIZ_EXPLODE;
      send(background);
      backgroundTimer.reset();
      break;
    case BUTTON_TWINKLE:
      background.value = VIZ_TWINKLE;
      send(background);
      backgroundTimer.reset();
      break;
    case BUTTON_SPIN:
      background.value = VIZ_SPIN;
      send(background);
      backgroundTimer.reset();
      break;
    case BUTTON_FLASH:
      send(strobeOn);
      break;
    default: break;
  }
}

void onButtonUnpressed(Button &button) {
  switch (button.pin) {
    case BUTTON_FLASH:
      send(strobeOff);
      break;
    default: break;
  }
}

void printButtonText(Button &button) {
  Serial.print("Button pressed: ");
  Serial.println(button.description);
}
