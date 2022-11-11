#include <FastLED.h>
#include "HX711.h"
#include <Eventually.h>
#include "Constants.h"


EvtManager mgr;

CRGB leds[NUMOFPLATFORMS][NUMPIXELS];

HX711 scale[NUMOFPLATFORMS];

float reading[NUMOFPLATFORMS] = { 0.0, 0.0, 0.0, 0.0 };

float thresholdWeight[NUMOFPLATFORMS] = { 220.0, 600.0, 140.0, 120.0 };

bool btnAlreadyPressed;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  delay(5000);
  Serial.println("HX711 scale demo");

  pinMode(BUTTONPIN, INPUT);

  scale[0].begin(DOUT1, CLK);
  Serial.println("Scale 1 initialised");
  scale[1].begin(DOUT2, CLK);
  Serial.println("Scale 2 initialised");
  scale[2].begin(DOUT3, CLK);
  Serial.println("Scale 3 initialised");
  scale[3].begin(DOUT4, CLK);
  Serial.println("Scale 4 initialised");

  FastLED.addLeds<NEOPIXEL, 5>(leds[0], NUMPIXELS);

  // tell FastLED there's 60 NEOPIXEL leds on pin 3
  FastLED.addLeds<NEOPIXEL, 4>(leds[1], NUMPIXELS);

  // tell FastLED there's 60 NEOPIXEL leds on pin 4
  FastLED.addLeds<NEOPIXEL, 3>(leds[2], NUMPIXELS);

  // tell FastLED there's 60 NEOPIXEL leds on pin 4
  FastLED.addLeds<NEOPIXEL, 2>(leds[3], NUMPIXELS);


  Serial.println("LEDS Initialised");
  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    scale[i].set_scale(210.0983);  //This value is obtained by using the SparkFun_HX711_Calibration sketch
    scale[i].tare();               //Assuming there is no weight on the scale at start up, reset the scale to 0
    Serial.println("Scale configured");
    setLight(i,CHSV(200,125,255));
  }
  Serial.println("Readings:");
  btnAlreadyPressed = false;

  set_listener();

  clearPixels();
}


bool check_weights() {

  Serial.println("Button Pressed");
  if (!btnAlreadyPressed) {

    btnAlreadyPressed = true;

    clearPixels();

    for (int i = 0; i < NUMOFPLATFORMS; i++) {
      reading[i] = scale[i].get_units();
      Serial.print("Reading");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(abs(reading[i]), 2);
      Serial.print(" \t");
      // if (abs(reading[i]) > thresholdWeight[i])
      //   turnOnLight(i);
      // else
      //   turnOffLight(i);
      display_weight_feedback(check_weight_threshold(abs(reading[i]),i),i);
    }
    Serial.println();
    btnAlreadyPressed = false;

    mgr.resetContext();
    mgr.addListener(new EvtPinListener(BUTTONPIN, (EvtAction)check_weights));
    return true;
  }
  return false;
}

bool set_listener() {
  mgr.resetContext();
  mgr.addListener(new EvtPinListener(BUTTONPIN, (EvtAction)check_weights));
  return true;
}


USE_EVENTUALLY_LOOP(mgr)

// void loop() {
//   // put your main code here, to run repeatedly:

// }

//Value Comparison Methods
int check_weight_threshold(float weight, int platformNumber){
  int y = map((int)weight, (int)thresholdWeight[platformNumber]*-1, (int)thresholdWeight[platformNumber]*3, 0, 100);
  // Serial.print((int)thresholdWeight[platformNumber]*-1); Serial.print("\t");
  // Serial.print((int)thresholdWeight[platformNumber]*3); Serial.print("\t");
  Serial.println(y);
  return y;
}

void display_weight_feedback(int mapping,int platformNumber){
  if(mapping < 49)
    setLight(platformNumber,CHSV(33,255,map(mapping,0,49,100,0)));
  else if(mapping>=49 && mapping <=55)
    setLight(platformNumber, CHSV(0,0,255));
  else
    setLight(platformNumber,CHSV(195,255,map(mapping,55,100,0,100)));
}


//LED Methods
void setLight(int stripNumber, CHSV value) {
  for (int i = 0; i < NUMPIXELS; i++) {
    leds[stripNumber][i] = value;
    FastLED.show();
    // delay(5);
  }
}

void clearPixels() {
  for (int x = 0; x < NUMOFPLATFORMS; x++) {
    setLight(x, CHSV(0,0,0));
  }
}