#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266_LEDs.h>
#include <Ticker.h>  //Ticker Library

const uint16_t FLASH_TIME_PERIOD = 2000; // 2000ms per flash cycle
const uint8_t BOARD_LED = 16;
const uint8_t WIFI_LED = 2;

Ticker ledHandler;

LedState currentState;
bool isLedOn = false;
uint16_t ledBrightness = 1010;
bool traceEnabled = false;


void _ledIsr();
void _setLedTime(int milliseconds);
uint32_t _getLedTimeOn();
uint32_t _getLedTimeOff();


void initialiseLedHandler() {
  initialiseLedHandler(STATE_IDLE);
}

void initialiseLedHandler(LedState state) {
  pinMode(WIFI_LED, OUTPUT);
  pinMode(BOARD_LED, OUTPUT);
  setBoardLedState(OFF);

  setLedHandlerState(state);

  _ledIsr();
  if (Serial && traceEnabled) {
    Serial.println(F("LED Handler initialised."));
  }
}

void _ledIsr() {
  // LEDs are active low on NodeMCU (?).
  if (isLedOn) {
    // do the switching off
    analogWrite(WIFI_LED, 1023);
    isLedOn = false;
    _setLedTime(_getLedTimeOff());
  } else {
    // do the switching on
    analogWrite(WIFI_LED, ledBrightness);
    isLedOn = true;
    _setLedTime(_getLedTimeOn());
  }
}

// FYI, the broghtness values are inverted for ESP 8266.
// 1023 is low brightness, and 0 is max brightness.
void setLedBrightness(uint16_t newBrightness) {
  if ((newBrightness >= 0) && (newBrightness < 1024)) {
    ledBrightness = newBrightness;
  } else if (Serial && traceEnabled) {
    Serial.println(F("Invalid brightness value. Brightness value must be between 0 and 1024."));
  }
}

void setLedsTrace(bool trace) {
  traceEnabled = trace;
}

void setLedHandlerState(LedState newState) {
  currentState = newState;
  if (Serial && traceEnabled) {
    Serial.print(F("LED time on: "));
    Serial.println(_getLedTimeOn());
    Serial.print(F("LED time off: "));
    Serial.println(_getLedTimeOff());
  }
}

void _setLedTime(int milliseconds) {
  if (ledHandler.active()) {
    ledHandler.detach();
  }

  ledHandler.once_ms(milliseconds, _ledIsr);
}

uint32_t _getLedTimeOn() {
  switch(currentState) {
    case STATE_FAILED:
    case STATE_IDLE:
    return (uint32_t)(FLASH_TIME_PERIOD / 2);

    case STATE_CONNECTING:
    return 200;

    case STATE_CONNECTED:
    return 50;
  }

  return 0;
}

uint32_t _getLedTimeOff() {
  return FLASH_TIME_PERIOD - _getLedTimeOn();
}

void setBoardLedState(bool enable) {
  digitalWrite(BOARD_LED, enable);
}
