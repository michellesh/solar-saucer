#include "solar_saucer_shared.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#define SLIDER_1       15  // D8
#define SLIDER_2       5   // D1
#define SLIDER_3       16  // D0
#define SLIDER_4       0   // D3

#define RED_BUTTON     2   // D4
#define BLUE_BUTTON    14  // D5
#define YELLOW_BUTTON  12  // D6
#define GREEN_BUTTON   13  // D7
#define WHITE_BUTTON   4   // D2

struct Button {
  int pin;
  bool pressed;
};

struct Slider {
  int pin;
  int value;
  int prev;
};

int sliderIndex = -1;

msg strobeOn = {ACTION_STROBE_ON};
msg strobeOff = {ACTION_STROBE_OFF};

Button redButton = {RED_BUTTON, false};
Button blueButton = {BLUE_BUTTON, false};
Button yellowButton = {YELLOW_BUTTON, false};
Button greenButton = {GREEN_BUTTON, false};
Button whiteButton = {WHITE_BUTTON, false};

Slider slider1 = {SLIDER_1};
Slider slider2 = {SLIDER_2};
Slider slider3 = {SLIDER_3};
Slider slider4 = {SLIDER_4};

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_add_peer(ss_broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(ss_broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(ss_broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
  pinMode(SLIDER_1, OUTPUT);
  pinMode(SLIDER_2, OUTPUT);
  pinMode(SLIDER_3, OUTPUT);
  pinMode(SLIDER_4, OUTPUT);
  pinMode(A0, INPUT);
}

void send(msg m) {
  esp_now_send(0, (uint8_t *) &m, sizeof(m));
}

bool isButtonPressed(Button button) {
  return digitalRead(button.pin) == 0;
}

bool sliderValueChanged(Slider slider) {
  int BUFFER = 20;
  return slider.value < (slider.prev - BUFFER) || slider.value > (slider.prev + BUFFER);
}

void loop() {
  EVERY_N_MILLISECONDS(100) {
    if (sliderIndex == -1) {
      digitalWrite(slider1.pin, HIGH);
    } else if (sliderIndex == 0) {
      slider1.value = analogRead(0);
      digitalWrite(slider1.pin, LOW);
      digitalWrite(slider2.pin, HIGH);
    } else if (sliderIndex == 1) {
      slider2.value = analogRead(0);
      digitalWrite(slider2.pin, LOW);
      digitalWrite(slider3.pin, HIGH);
    } else if (sliderIndex == 2) {
      slider3.value = analogRead(0);
      digitalWrite(slider3.pin, LOW);
      digitalWrite(slider4.pin, HIGH);
    } else if (sliderIndex == 3) {
      slider4.value = analogRead(0);
      digitalWrite(slider4.pin, LOW);
      digitalWrite(slider1.pin, HIGH);
    }
    sliderIndex = sliderIndex == 3 ? 0 : sliderIndex + 1;
  }

  if (sliderValueChanged(slider1)) {
    Serial.print("Slider 1 changed ");
    Serial.println(slider1.value);

    // TODO
    //send();

    slider1.prev = slider1.value;
  }

  if (sliderValueChanged(slider2)) {
    Serial.print("Slider 2 changed ");
    Serial.println(slider2.value);

    // TODO
    //send();

    slider2.prev = slider2.value;
  }

  if (sliderValueChanged(slider3)) {
    Serial.print("Slider 3 changed ");
    Serial.println(slider3.value);

    // TODO
    //send();

    slider3.prev = slider3.value;
  }

  if (sliderValueChanged(slider4)) {
    Serial.print("Slider 4 changed ");
    Serial.println(slider4.value);

    // TODO
    //send();

    slider4.prev = slider4.value;
  }

  EVERY_N_MILLISECONDS(5) {
    if (isButtonPressed(whiteButton)) {
      if (!whiteButton.pressed) {
        Serial.println("White button pressed: STROBE ON");
        whiteButton.pressed = true;
        send(strobeOn);
      }
    } else if (whiteButton.pressed) {
      Serial.println("White button UN-pressed: STROBE OFF");
      whiteButton.pressed = false;
      send(strobeOff);
    }

    if (isButtonPressed(redButton)) {
      if (!redButton.pressed) {
        Serial.println("Red button pressed");
        redButton.pressed = true;

        // TODO
        //send();
      }
    } else {
      redButton.pressed = false;
    }

    if (isButtonPressed(yellowButton)) {
      if (!yellowButton.pressed) {
        Serial.println("Yellow button pressed");
        yellowButton.pressed = true;

        // TODO
        //send();
      }
    } else if (yellowButton.pressed) {
      //Serial.println("Strobe Off");
      yellowButton.pressed = false;
      //send(strobeOff);
    }

    if (isButtonPressed(blueButton)) {
      if (!blueButton.pressed) {
        Serial.println("Blue button pressed");
        blueButton.pressed = true;

        // TODO
        //send();
      }
    } else {
      blueButton.pressed = false;
    }

    if (isButtonPressed(greenButton)) {
      if (!greenButton.pressed) {
        Serial.println("Green button pressed");
        greenButton.pressed = true;

        // TODO
        //send();
      }
    } else {
      greenButton.pressed = false;
    }
  }
}
