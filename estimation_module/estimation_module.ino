#include <Adafruit_Thermal.h>
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

Adafruit_Thermal printer(&Serial1);  // Or Serial2, Serial3, etc.

void setup() {

  Serial.begin(9600);
  delay(500);
  Serial.println("HX711 scale demo");

  Serial1.begin(19200);  // Use this instead if using hardware serial
  printer.begin();

  pinMode(BUTTONPIN, INPUT);

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].initialisePlatform();
  }

  btnAlreadyPressed = false;
  set_listener();

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].clearLedFeedback();
  }

  // printer.boldOn();
  printer.println(F("I'm Ready!"));
  // printer.boldOff();

  printer.feed(2);

  printer.sleep();       // Tell printer to sleep
  // delay(3000L);          // Sleep for 3 seconds
  // printer.wake();        // MUST wake() before printing again, even if reset
  // printer.setDefault();  // Restore printer to defaults
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

    printer.wake();
    printer.feed(1);
    printer.println("Guess#");
    printer.feed(2);
    printer.sleep();

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