#include "Platform.h"

Platform::Platform(int loadCellDataPin, int loadCellClkPin, LED_PIN ledPin, int numberOfPixels, float threshold = 0.0) {
  thresholdValue = threshold;
  hue = 0;
  sat = 0;
  val = 0;
  maxPixelCount = numberOfPixels;
  activePixelCount = maxPixelCount;
  weight = 0.0;
  weightMapping = 0;
  shouldOverloop = false;

  switch (ledPin) {
    case LED_PIN_1: FastLED.addLeds<WS2812B, LED_PIN_1>(leds, MAXLEDCOUNT); break;
    case LED_PIN_2: FastLED.addLeds<WS2812B, LED_PIN_2>(leds, MAXLEDCOUNT); break;
    case LED_PIN_3: FastLED.addLeds<WS2812B, LED_PIN_3>(leds, MAXLEDCOUNT); break;
    case LED_PIN_4: FastLED.addLeds<WS2812B, LED_PIN_4>(leds, MAXLEDCOUNT); break;
    default: FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds, MAXLEDCOUNT); break;
  }
  FastLED.setBrightness(255);

  scale.begin(loadCellDataPin, loadCellClkPin);
}

void Platform::initialisePlatform() {
  scale.set_scale(210.0983);
  scale.tare();
  hue = 200;
  sat = 125;
  val = 255;
  // loopLight();
  setLedFeedback();
  // Serial.println("Scale configured");
}

void Platform::clearLedFeedback() {
  val = 0;
  setLedFeedback();
}

void Platform::setLedFeedback() {
  setLight(CHSV(hue, sat, val));
}

void Platform::setLight(CHSV value) {
  // Serial.print(hue);
  // Serial.print("\t");
  // Serial.print(sat);
  // Serial.print("\t");
  // Serial.println(val);
  for (int i = 0; i < activePixelCount; i++) {
    leds[i] = value;
    FastLED.show();
    // delay(1);
  }
}

void Platform::loopLight() {
  for (int i = 0; i < maxPixelCount; i++) {
    leds[i] = CHSV(195, 255, 255);
    FastLED.show();
    (maxPixelCount > 10) ? delay(25) : delay(40);
  }
  if (shouldOverloop) {
    Serial.println("Överlooping");
    for (int i = maxPixelCount; i >= 0; i--) {
      leds[i] = CRGB::Black;
      FastLED.show();
    }
    for (int i = 0; i < maxPixelCount; i++) {
      leds[i] = CRGB::DarkOrange;
      FastLED.show();
      (maxPixelCount > 10) ? delay(25) : delay(40);
    }
  }
  for (int i = maxPixelCount; i >= 0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

void Platform::measureWeight() {
  weight = abs(scale.get_units());
  checkWeightThreshold();
  setWeightFeedback();
  loopLight();
}

float Platform::getWeight() {
  return weight;
}

int Platform:: getWeightMapping(){
  return weightMapping;
}

void Platform::checkWeightThreshold() {
  weightMapping = map((int)weight, 0, (int)thresholdValue * 2, 0, 100);
  // Serial.print((int)thresholdValue *-1); Serial.print("\t");
  // Serial.print((int)thresholdValue *3); Serial.print("\t");
  // Serial.println(weightMapping);
  // return y;
}

void Platform::setWeightFeedback() {
  if (weightMapping < 49) {
    hue = 195;
    sat = 255;
    val = 255;
    activePixelCount = map(weightMapping, 0, 49, 1, maxPixelCount);
    shouldOverloop = false;
  } else if (weightMapping >= 49 && weightMapping <= 55) {
    hue = 0;
    sat = 0;
    val = 255;
    activePixelCount = maxPixelCount;
    shouldOverloop = false;
  } else {
    hue = 33;
    sat = 255;
    val = 255;
    activePixelCount = map(weightMapping, 55, 100, 1, maxPixelCount);
    shouldOverloop = true;
  }
}