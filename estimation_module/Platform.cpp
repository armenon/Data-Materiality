#include "Platform.h"



// struct PixelUtil {

//   template< uint8_t DATA_PIN = LedPin3>
//   void init(uint16_t num_pixels, CRGB *data) {
//     FastLED.addLeds< NEOPIXEL, DATA_PIN>(data, num_pixels);
//   }
// };

// PixelUtil p;

Platform::Platform(int loadCellDataPin, int loadCellClkPin, LED_PIN ledPin, int numberOfPixels, float threshold = 0.0) {
  thresholdValue = threshold;
  hue = 0;
  sat = 0;
  val = 0;
  pixelCount = numberOfPixels;
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, MAXLEDCOUNT);
  scale.begin(loadCellDataPin, loadCellClkPin);
}

void Platform::initialisePlatform() {
  scale.set_scale(210.0983);
  scale.tare();
}

void Platform::setLight(CHSV value) {
  for (int i = 0; i < pixelCount; i++) {
    leds[i] = value;
    FastLED.show();
    // delay(5);
  }
}

void Platform::setLedFeedback() {
  setLight(CHSV(hue, sat, val));
}