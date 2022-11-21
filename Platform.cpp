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
    case LED_PIN_1: FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds, MAXLEDCOUNT); break;
    case LED_PIN_2: FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds, MAXLEDCOUNT); break;
    case LED_PIN_3: FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds, MAXLEDCOUNT); break;
    case LED_PIN_4: FastLED.addLeds<WS2812B, LED_PIN_4, GRB>(leds, MAXLEDCOUNT); break;
    default: FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds, MAXLEDCOUNT); break;
  }
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 500);  

  scale.begin(loadCellDataPin, loadCellClkPin);
}

void Platform::initialisePlatform() {
  scale.set_scale(210.0983);
  scale.tare();
  hue = 200;sat = 125;val = 255;
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
  // Serial.print(hue); // Serial.print("\t"); // Serial.print(sat); // Serial.print("\t"); // Serial.println(val);
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
    leds[i] = CHSV(43, 255, 255);
    FastLED.show();
    (maxPixelCount > 10) ? delay(35) : delay(60);
  }
  if (shouldOverloop) {
   // Serial.println("Överlooping");
    for (int i = maxPixelCount; i >= 0; i--) {
      leds[i] = CRGB::Black;
      FastLED.show();
    }
    for (int i = maxPixelCount; i >= 0; i--) {
      leds[i] = CRGB::White;
      FastLED.show();
    }
    for (int i = 0; i < activePixelCount; i++) {
      leds[i] = CHSV(215, 255, 255); 
      FastLED.show();
      (maxPixelCount > 10) ? delay(35) : delay(60);
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

int Platform::getWeightMapping() {
  return weightMapping;
}

void Platform::checkWeightThreshold() {
  // Serial.print((int)thresholdValues[currentFoodProduct]); Serial.print("\t"); Serial.print((int)thresholdValues[currentFoodProduct] *2); Serial.print("\t"); 
  Serial.print(weightMapping); Serial.print("\t");
  if (weight > (thresholdValues[currentFoodProduct] *2)) {
    weightMapping = 100;
    return;
  }
  weightMapping = map((int)weight, 0, (int)thresholdValues[currentFoodProduct] * 3, 0, 100);
}

void Platform::setWeightFeedback() {
  if (weightMapping < THRESHOLD_LOWERBOUND) {
    hue = 33; sat = 255; val = 255;
    activePixelCount = map(weightMapping, 0, THRESHOLD_LOWERBOUND, 1, maxPixelCount);
    shouldOverloop = false;
  } else if (weightMapping >= THRESHOLD_LOWERBOUND && weightMapping <= THRESHOLD_UPPERBOUND) {
    hue = 1; sat = 0; val = 255;
    activePixelCount = maxPixelCount;
    shouldOverloop = false;
  } else {
    hue = 215; sat = 255; val = 255;
    activePixelCount = map(weightMapping, THRESHOLD_UPPERBOUND, 100, 1, maxPixelCount);
    shouldOverloop = true;
  }
}

void Platform::changeFoodProduct() {
  currentFoodProduct++;
  if (currentFoodProduct >= numOfStoredFoods)
    currentFoodProduct = 0;
 // Serial.print("Current Food Prduct");Serial.println(currentFoodProduct);
  switch(currentFoodProduct) {
    case HAMBURGER: hue = 0; sat = 255; val = 255; break;
    case ASPARAGUS: hue = 100; sat = 255; val = 255; break;
    case CHEESE: hue = 50; sat = 255; val = 255; break;
    default:  hue = 200; sat = 125; val = 255; break;
  }
  
  setLedFeedback();
  delay(50);
  clearLedFeedback();
}

int Platform::getCurrentFoodProduct(){
  return currentFoodProduct;
}