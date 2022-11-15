// Platform.h
#ifndef PRINTER_h
#define PRINTER_h

#include <Arduino.h>
#include <Adafruit_Thermal.h>
//#include "Constants.h"


// class Printer {


// private:
// Adafruit_Thermal printer_internal; // Or Serial2, Serial3, etc.

// public:
// Printer();
void initialisePrinter(Adafruit_Thermal *printer);
void printInitialMessage(Adafruit_Thermal *printer);
void printTryCount(Adafruit_Thermal *printer, int tryCount);

void printProductInfo(Adafruit_Thermal *printer, int currentProduct);
// };


#endif