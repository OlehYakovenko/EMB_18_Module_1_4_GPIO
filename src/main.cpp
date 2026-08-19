#include <Arduino.h>

#define RED_LED_PIN 15
#define BLUE_LED_PIN 16
#define EXTERNAL_BUTTON_PIN 17
#define INTERNAL_BUTTON_PIN 0
#define shortDelayTime 200
#define longDelayTime 1000
#define BaudRate 115200
#define debounceDelay 50

uint8_t mode = 1; // 1 - зовнішня кнопка, 2 - внутрішня

uint8_t externalLastReading = HIGH, externalState = HIGH;
uint8_t internalLastReading = HIGH, internalState = HIGH;
unsigned long externalDebounceTime = 0, internalDebounceTime = 0;

unsigned long lastBlinkTime = 0;
bool ledOn = false;

bool pressed(uint8_t pin, uint8_t &lastReading, uint8_t &state, unsigned long &debounceTime) {
  uint8_t reading = digitalRead(pin);
  if (reading != lastReading) {
    debounceTime = millis();
  }
  lastReading = reading;

  if (millis() - debounceTime > debounceDelay && reading != state) {
    state = reading;
    return state == LOW;
  }
  return false;
}

void setup() {
  Serial.begin(BaudRate);
  Serial.println("Start");
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INTERNAL_BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void loop() {
  if (pressed(EXTERNAL_BUTTON_PIN, externalLastReading, externalState, externalDebounceTime)) {
    mode = 1;
    ledOn = false;
    lastBlinkTime = millis();
  }
  if (pressed(INTERNAL_BUTTON_PIN, internalLastReading, internalState, internalDebounceTime)) {
    mode = 2;
    ledOn = false;
    lastBlinkTime = millis();
  }

  unsigned long interval = (mode == 1) ? shortDelayTime : longDelayTime;
  if (millis() - lastBlinkTime >= interval) {
    lastBlinkTime = millis();
    ledOn = !ledOn;
    digitalWrite(RED_LED_PIN, ledOn);
    digitalWrite(BLUE_LED_PIN, mode == 1 ? ledOn : !ledOn);
  }
}
