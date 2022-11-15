// Platform.h
#ifndef PLATFORM_h
#define PLATFORM_h

#include <Arduino.h>
#include "HX711.h"
#include <FastLED.h>
#include "Constants.h"

typedef enum {
  LED_PIN_1 = 5,
  LED_PIN_2 = 4,
  LED_PIN_3 = 3,
  LED_PIN_4 = 2
} LED_PIN;

class Platform {

private:
  float weight;
  float thresholdValues[3];
  int weightMapping;
  int numOfStoredFoods;
  int currentFoodProduct;

  unsigned int hue;
  unsigned char sat;
  unsigned char val;
  int maxPixelCount;
  int activePixelCount;
  bool shouldOverloop;

  HX711 scale;
  CRGB leds[MAXLEDCOUNT];


  void loopLight();
  void setLight(CHSV value);
  void checkWeightThreshold();
  void setWeightFeedback();


public:
  Platform(int loadCellDataPin, int loadCellClkPin, LED_PIN ledPin, int numberOfPixels, float thresholds[], int numOfFoods);
  void  initialisePlatform();
  void  measureWeight();
  void  setLedFeedback();
  void  clearLedFeedback();
  float getWeight();
  int   getWeightMapping();
  void  lightFX();
  void  changeFoodProduct();
  int   getCurrentFoodProduct();
};

#endif