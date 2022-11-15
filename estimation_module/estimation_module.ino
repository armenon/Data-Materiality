#include <Adafruit_Thermal.h>
#include <FastLED.h>
#include "HX711.h"
#include <Eventually.h>
#include "Platform.h"

EvtManager mgr;
float arrayOfThresholds[4][2] = { { 220.0, 120.0 }, { 600.0, 200.0 }, { 180.0, 200.0 }, { 100.0, 100.0 } };

Platform platforms[NUMOFPLATFORMS] = { Platform(DOUT1, CLK, LED_PIN_1, 15, arrayOfThresholds[0], 2),
                                       Platform(DOUT2, CLK, LED_PIN_2, 16, arrayOfThresholds[1], 2),
                                       Platform(DOUT3, CLK, LED_PIN_3, 10, arrayOfThresholds[2], 2),
                                       Platform(DOUT4, CLK, LED_PIN_4, 9, arrayOfThresholds[3], 2) };

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
  pinMode(CHGPRDCTBTNPIN, INPUT);

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].initialisePlatform();
  }

  btnAlreadyPressed = false;
  guessCount = 0;
  // randomSeed(analogRead(0));
  set_listener();

  // printInitialMessage();
  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].clearLedFeedback();
  }
}


bool check_weights() {
  if (!btnAlreadyPressed) {
    mgr.resetContext();
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
      printer.setSize('M');
      printer.print("So it took you about ");
      //  printer.print("This is your ");
      printer.print(guessCount);
      printer.print(" tries to figure this out.");
      printer.feed(4);
      printer.sleep();
      guessCount = 0;
    }

    set_listener();
    return true;
  }
  return true;
}

bool change_product() {
  //  Serial.println("Changing products");
  if (!btnAlreadyPressed) {
    mgr.resetContext();
   
    btnAlreadyPressed = true;
    for (int i = 0; i < NUMOFPLATFORMS; i++) {
      platforms[i].changeFoodProduct();
    }
    btnAlreadyPressed = false;
    guessCount = 0;
    set_listener();
    return true;
  }
  return true;
}

bool set_listener() {
  mgr.resetContext();
  mgr.addListener(new EvtPinListener(BUTTONPIN, (EvtAction)check_weights));
  mgr.addListener(new EvtPinListener(CHGPRDCTBTNPIN, (EvtAction)change_product));
  // mgr.addListener(new EvtTimeListener(10000, false, (EvtAction)daydream));
  return true;
}

bool didGetAllWeightsCorrect() {
  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    int wgtMap = platforms[i].getWeightMapping();
    if ((wgtMap < 49) || (wgtMap > 55))
      return false;
  }
  return true;
}

// bool daydream() {
//   Serial.println("Daydreaming");
//   printer.wake();
//   printer.println(F("Sigh... I'm bored"));
//   printer.feed(2);
//   printer.sleep();

//   switch (rand() % 4) {
//     case 0:platforms[0].lightFX();break;
//     case 1:platforms[1].lightFX();break;
//     case 2:platforms[2].lightFX();break;
//     case 3:platforms[3].lightFX();break;
//     default:platforms[0].lightFX();
//   }

//   // set_listener();
//   return false;
// }

void printInitialMessage() {
  printer.setSize('M');
  printer.println(F("Tjena!"));
  printer.setSize('S');
  printer.setFont('B');
  printer.justify('L');
  printer.println(F("My purpose is to help you figure out how much CO2 emissions were emitted for this food product placed on me."));
  printer.feed(1);
  printer.println(F("Go ahead, pick up some of those Carbon pieces. Feel them!"));
  printer.feed(2);
  delay(5000);
  printer.println(F("Now try to guess by feeling how much carbon was emitted for a particular part of this food's story and place that many carbon pieces on that particular platform!"));
  printer.feed(1);
  printer.println(F("Then press the button. I will try to help you."));

  printer.feed(2);
  printer.setDefault();  // Restore printer to defaults
  printer.sleep();
}

USE_EVENTUALLY_LOOP(mgr)