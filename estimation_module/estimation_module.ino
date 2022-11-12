#include <FastLED.h>
#include "HX711.h"
#include <Eventually.h>
#include "Constants.h"
#include "Platform.h"


EvtManager mgr;

Platform platforms[NUMOFPLATFORMS] = { Platform(DOUT1, CLK, LED_PIN_1, 16, 220.0),
                                       Platform(DOUT2, CLK, LED_PIN_2, 16, 600.0),
                                       Platform(DOUT3, CLK, LED_PIN_3, 10, 140.0),
                                       Platform(DOUT4, CLK, LED_PIN_4, 10, 120.0) };

bool btnAlreadyPressed;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  delay(500);
  Serial.println("HX711 scale demo");

  pinMode(BUTTONPIN, INPUT);

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].initialisePlatform();
  }

  btnAlreadyPressed = false;
  set_listener();

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].clearLedFeedback();
  }
}


bool check_weights() {
  if (!btnAlreadyPressed) {
    btnAlreadyPressed = true;

    for (int i = 0; i < NUMOFPLATFORMS; i++) {
      platforms[i].clearLedFeedback();
    }

    for (int i = 0; i < NUMOFPLATFORMS; i++) {
      platforms[i].measureWeight();
      Serial.print("Reading");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(platforms[i].getWeight());
      Serial.print(" \t");
      platforms[i].setLedFeedback();
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

