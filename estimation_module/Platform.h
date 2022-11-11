// Platform.h
#ifndef PLATFORM_h
#define PLATFORM_h

#include <Arduino.h>
#include "HX711.h"
#include <FastLED.h>

#define MAXLEDCOUNT 16

typedef enum {
  LED_PIN_1 = 5,
  LED_PIN_2 = 4,
  LED_PIN_3 = 3,
  LED_PIN_4 = 2
} LED_PIN;

class Platform {
private:
  float weight;
  float thresholdValue;

  unsigned int hue;
  unsigned char sat;
  unsigned char val;
  int pixelCount;

  HX711 scale;
  CRGB leds[MAXLEDCOUNT];

  void setLight(CHSV value);

public:
  Platform(int loadCellDataPin, int loadCellClkPin, LED_PIN ledPin, int numberOfPixels, float threshold);
  void initialisePlatform();
  void measureWeight();
  void setLedFeedback();
  float getWeight();
};

#endif