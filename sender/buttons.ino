bool isButtonPressed(Button button) {
  return digitalRead(button.pin) == 0;
}

void checkButtonPressed(Button &button) {
  if (isButtonPressed(button)) {
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
    case RED_BUTTON:
      colorMode = colorMode == COLOR_MODE_SOLID
        ? COLOR_MODE_GRADIENT
        : colorMode == COLOR_MODE_GRADIENT
        ? COLOR_MODE_WHEEL
        : COLOR_MODE_SOLID;
      cycleColorMode.value = colorMode;
      send(cycleColorMode);
      break;
    case BLUE_BUTTON:
      setBackground(VIZ_EXPLODE);
      send(background);
      backgroundTimer.reset();
      break;
    case YELLOW_BUTTON:
      setBackground(VIZ_TWINKLE);
      send(background);
      backgroundTimer.reset();
      break;
    case GREEN_BUTTON:
      setBackground(VIZ_SPIN);
      send(background);
      backgroundTimer.reset();
      break;
    case WHITE_BUTTON:
      send(strobeOn);
      break;
    default: break;
  }
}

void onButtonUnpressed(Button &button) {
  switch (button.pin) {
    case WHITE_BUTTON:
      send(strobeOff);
      break;
    default: break;
  }
}

void printButtonText(Button &button) {
  Serial.print("Button pressed: ");
  Serial.println(button.description);
}

void setBackground(int viz) {
  background.value = viz;
}
