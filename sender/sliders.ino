auto sliderToBrightness = scale(1000, 50, 0, 255, true);
auto sliderToHue = scale(900, 50, 255, 0, true);
auto sliderToSpeed = scale(1000, 0, 1, 10, true);

bool sliderValueChanged(Slider slider) {
  int BUFFER = 20;
  return slider.value < (slider.prev - BUFFER) || slider.value > (slider.prev + BUFFER);
}

void checkSliderChanged(Slider &slider) {
  if (sliderValueChanged(slider)) {
    onSliderChanged(slider);
    printSliderText(slider);
  }
}

void onSliderChanged(Slider &slider) {
  switch (slider.pin) {
    case SLIDER_1:
      brightness.value = sliderToBrightness(slider.value);
      send(brightness);
      break;
    case SLIDER_2:
      setColor(slider2, colorLeft);
      send(colorLeft);
      break;
    case SLIDER_3:
      setColor(slider3, colorRight);
      send(colorRight);
      break;
    case SLIDER_4:
      speed.value = sliderToSpeed(slider4.value);
      explodeTimer.totalCycleTime = explodeFrequency(speed.value);
      send(speed);
      break;
    default: break;
  }
  slider.prev = slider.value;
}

void printSliderText(Slider &slider) {
  int value = slider.pin == SLIDER_1 ? brightness.value
    : slider.pin == SLIDER_2 ? colorLeft.value
    : slider.pin == SLIDER_3 ? colorRight.value
    : speed.value;

  Serial.print(slider.description);
  Serial.print(" changed: ");
  Serial.println(value);
}

void setColor(Slider &slider, msg &color) {
  if (slider.value < 100) { // Set to white
    color.value = 0;
    color.value2 = 0;
  } else {
    color.value = sliderToHue(slider.value);
    color.value2 = 255;
  }
}

void readSliderSetNext(Slider &sliderOff, Slider &sliderOn) {
  sliderOff.value = analogRead(0);
  digitalWrite(sliderOff.pin, LOW);
  digitalWrite(sliderOn.pin, HIGH);
}

void sendSliderValues() {
  digitalWrite(slider1.pin, HIGH);
  delay(100);

  readSliderSetNext(slider1, slider2);
  delay(100);

  readSliderSetNext(slider2, slider3);
  delay(100);

  readSliderSetNext(slider3, slider4);
  delay(100);

  slider4.value = analogRead(0);
  digitalWrite(slider4.pin, LOW);

  onSliderChanged(slider1);
  delay(100);

  onSliderChanged(slider2);
  delay(100);

  onSliderChanged(slider3);
  delay(100);

  onSliderChanged(slider4);
  delay(100);
}
