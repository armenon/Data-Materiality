#include "Platform.h"

Platform::Platform(int loadCellDataPin, int loadCellClkPin, LED_PIN ledPin, int numberOfPixels, float thresholds[], int numOfFoods) {

  hue = 0;
  sat = 0;
  val = 0;
  maxPixelCount = numberOfPixels;
  activePixelCount = maxPixelCount;
  numOfStoredFoods = numOfFoods;
  weight = 0.0;
  weightMapping = 0;
  shouldOverloop = false;
  currentFoodProduct = 0;

  for (int i = 0; i < numOfStoredFoods; i++) {
    thresholdValues[i] = thresholds[i];
  }

  switch (ledPin) {
    case LED_PIN_1: FastLED.addLeds<WS2812B, LED_PIN_1, RGB>(leds, MAXLEDCOUNT); break;
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

void Platform::lightFX() {
  uint8_t BeatsPerMinute = 48;
  uint8_t gHue = 0;  // rotating "base color" used by many of the patterns
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < maxPixelCount; i++) {  //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  FastLED.show();
}

void Platform::loopLight() {
  for (int i = 0; i < maxPixelCount; i++) {
    leds[i] = CHSV(215, 255, 255);
    // leds[i] = CRGB::Purple;
    FastLED.show();
    (maxPixelCount > 10) ? delay(35) : delay(50);
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
  // lightFX();
  // (maxPixelCount > 10) ? delay(40) : delay(80);
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

int Platform::getWeightMapping() {
  return weightMapping;
}

void Platform::checkWeightThreshold() {
  if (weight > (thresholdValues[currentFoodProduct] * 2)) {
    weightMapping = 100;
    return;
  }
  weightMapping = map((int)weight, 0, (int)thresholdValues[currentFoodProduct] * 2.5, 0, 100);
  // Serial.print((int)thresholdValue *-1); Serial.print("\t");
  // Serial.print((int)thresholdValue *3); Serial.print("\t");
  Serial.println(weightMapping);
  // return y;
}

void Platform::setWeightFeedback() {
  if (weightMapping < 49) {
    hue = 215;
    sat = 25;
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

void Platform::changeFoodProduct() {
  currentFoodProduct++;
  if (currentFoodProduct >= numOfStoredFoods)
    currentFoodProduct = 0;
  Serial.print("Current Food Prduct");
  Serial.println(currentFoodProduct);
  hue = 200;
  sat = 125;
  val = 255;
  setLedFeedback();
  delay(50);
  clearLedFeedback();
}