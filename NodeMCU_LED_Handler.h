#ifndef ESP8266_LEDS_H_INCLUDED
#define ESP8266_LEDS_H_INCLUDED

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define ON false
#define OFF true

enum LedState {
  STATE_IDLE,
  STATE_CONNECTING,
  STATE_CONNECTED,
  STATE_FAILED
};

extern void initialiseLedHandler();
extern void initialiseLedHandler(LedState state);
extern void setLedBrightness(uint16_t newBrightness);
extern void setLedsTrace(bool trace);
extern void setLedHandlerState(LedState newState);
extern void setBoardLedState(bool enable);

#endif
