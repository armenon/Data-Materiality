#include <Adafruit_Thermal.h>
#include <FastLED.h>
#include "HX711.h"
#include <Eventually.h>
#include "Platform.h"


EvtManager mgr;

Platform platforms[NUMOFPLATFORMS] = { Platform(DOUT1, CLK, LED_PIN_1, 16, 220.0),
                                       Platform(DOUT2, CLK, LED_PIN_2, 16, 600.0),
                                       Platform(DOUT3, CLK, LED_PIN_3, 10, 180.0),
                                       Platform(DOUT4, CLK, LED_PIN_4, 10, 100.0) };

bool btnAlreadyPressed;
int guessCount;

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
  guessCount = 0;
  set_listener();

  printInitialMessage();
  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].clearLedFeedback();
  }
}


bool check_weights() {
  if (!btnAlreadyPressed) {
    btnAlreadyPressed = true;
    guessCount++;

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
    if (didGetAllWeightsCorrect()) {
      printer.wake();
      printer.setSize('L');
      printer.print("So it took you about ");
      printer.print(guessCount);
      printer.print(" tries to figure it out.");
      printer.feed(2);
      printer.sleep();
    }


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

bool didGetAllWeightsCorrect() {
  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    int wgtMap = platforms[i].getWeightMapping();
    // Serial.print("Looping ");
    if ((wgtMap < 49) || (wgtMap > 55))
      return false;
  }
  return true;
}

void printInitialMessage() {
  printer.setSize('M');
  printer.println(F("Tjena!"));
  printer.setSize('S');
  printer.setFont('B');
  printer.justify('L');
  printer.println(F("My purpose is to help you figure out how much CO2 emissions were emitted for this food product placed on me."));
  printer.feed(1);
  printer.println(F("Go ahead, pick up some of those Carbon pieces. Feel them!"));
  printer.feed(1);
  delay(5000);
  printer.println(F("Now try to guess by feeling how much carbon was emitted for a particular part of this food's story and place that many carbon pieces on that particular platform!"));
  printer.feed(1);
  printer.println(F("Then press the button. I will try to help you."));

  printer.feed(2);
  printer.setDefault();  // Restore printer to defaults
  printer.sleep();
}

USE_EVENTUALLY_LOOP(mgr)