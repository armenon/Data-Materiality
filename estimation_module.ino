#include <Eventually.h>
#include "Platform.h"
#include "Printer.h"

EvtManager mgr;

float arrayOfThresholds[NUMOFPLATFORMS][NUMOFFOODS] = { { 1610.0, 100.0, 740.0 },   // Agriculture  - Hamburger (150g), Asparagus, Cheese (150g)
                                                        { 85.0, 10.0, 360.0 },      // Processing   - Hamburger (150g), Asparagus, Cheese (150g)
                                                        { 70.0, 100.0, 180.0 },     // Packaging    - Hamburger (150g), Asparagus, Cheese (150g)
                                                        { 70.0, 1900.0, 105.0 } };  // Transport    - Hamburger (150g), Asparagus, Cheese (150g)

Platform platforms[NUMOFPLATFORMS] = { Platform(DOUT1, CLK, LED_PIN_1, 15, arrayOfThresholds[0], NUMOFFOODS),
                                       Platform(DOUT2, CLK, LED_PIN_2, 16, arrayOfThresholds[1], NUMOFFOODS),
                                       Platform(DOUT3, CLK, LED_PIN_3, 10, arrayOfThresholds[2], NUMOFFOODS),
                                       Platform(DOUT4, CLK, LED_PIN_4, 9, arrayOfThresholds[3], NUMOFFOODS) };

bool btnAlreadyPressed;
int guessCount;

bool didGetAllWgtsForCurrentFoodProduct[NUMOFFOODS];

Adafruit_Thermal printer1(&Serial1);

void setup() {

  Serial.begin(9600);
  delay(500);

  pinMode(BUTTONPIN, INPUT);
  pinMode(CHGPRDCTBTNPIN, INPUT);
  pinMode(PRTINFOBTNPIN, INPUT);

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].initialisePlatform();
    didGetAllWgtsForCurrentFoodProduct[i] = false;
  }

  btnAlreadyPressed = false;
  guessCount = 0;

  set_listener();

  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    platforms[i].clearLedFeedback();
  }

  initialisePrinter(&printer1);
 // printInitialMessage(&printer1);
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
      printTryCount(&printer1, guessCount);
      guessCount = 0;
    }

    set_listener();
    return true;
  }
  return true;
}

bool change_product() {
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

bool print_product_info() {
  if (!btnAlreadyPressed) {
    Serial.println("I should be printing this");
    mgr.resetContext();
    btnAlreadyPressed = true;
    // printProductInfo(&printer1, platforms[0].getCurrentFoodProduct(), didGetAllWgtsForCurrentFoodProduct[platforms[0].getCurrentFoodProduct()]);
    printProductInfo(&printer1, platforms[0].getCurrentFoodProduct(), true);
    btnAlreadyPressed = false;
    set_listener();
    return true;
  }
  return true;
}

bool set_listener() {
  mgr.resetContext();
  mgr.addListener(new EvtPinListener(BUTTONPIN, (EvtAction)check_weights));
  mgr.addListener(new EvtPinListener(CHGPRDCTBTNPIN, (EvtAction)change_product));
  mgr.addListener(new EvtPinListener(PRTINFOBTNPIN, (EvtAction)print_product_info));
  // mgr.addListener(new EvtTimeListener(10000, false, (EvtAction)daydream));
  return true;
}

bool didGetAllWeightsCorrect() {
  for (int i = 0; i < NUMOFPLATFORMS; i++) {
    int wgtMap = platforms[i].getWeightMapping();
    if ((wgtMap < THRESHOLD_LOWERBOUND) || (wgtMap > THRESHOLD_UPPERBOUND))
      return false;
  }
  didGetAllWgtsForCurrentFoodProduct[platforms[0].getCurrentFoodProduct()] = true;
  return true;
}



USE_EVENTUALLY_LOOP(mgr)