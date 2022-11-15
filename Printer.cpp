#include "Printer.h"
// Printer::Printer(){
//   // printer_internal = printer;
// }

void initialisePrinter(Adafruit_Thermal *printer) {
  Serial1.begin(19200);  // Use this instead if using hardware serial
  printer->begin();
}


void printInitialMessage(Adafruit_Thermal *printer) {
  printer->setSize('M');
  printer->println(F("Tjena!"));
  printer->setSize('S');
  printer->setFont('B');
  printer->justify('L');
  printer->println(F("My purpose is to help you figure out how much CO2 emissions were emitted for this food product placed on me."));
  printer->feed(1);
  printer->println(F("Go ahead, pick up some of those Carbon pieces. Feel them!"));
  printer->feed(2);
  delay(5000);
  printer->println(F("Now try to guess by feeling how much carbon was emitted for a particular part of this food's story and place that many carbon pieces on that particular platform!"));
  printer->feed(1);
  printer->println(F("Then press the button. I will try to help you."));

  printer->feed(2);
  printer->setDefault();  // Restore printer to defaults
  printer->sleep();
}

void printTryCount(Adafruit_Thermal *printer, int tryCount) {
  printer->wake();
  printer->setSize('M');
  printer->print("It took you ");
  printer->print(tryCount);
  printer->print(" tries to estimate this by feeling the weight of the CO2 emissions.");
  printer->print("I will try to find out more about this product now.");
  printer->feed(4);
  printer->sleep();
}

void printProductInfo(Adafruit_Thermal *printer, int currentProduct) {
  printer->wake();
  printer->setSize('S');
  printer->setFont('B');
  printer->justify('L');
  printer->print("So here's what I found out about this product");
  printer->feed(2);

  switch (currentProduct) {
    case 0:
      printer->print("This product was cultivated/reared in Sweden. And during this time, it emitted about  ");
      break;
    case 1:
      printer->print("This product was cultivated in Peru. And during this time, it emitted about  ");
      break;
    case 2:
      printer->print("This product was made in Sweden. And during this time, it emitted about  ");
      break;
    default:
      printer->print("Hmmmm.. I guess I couldn't find much info on this product");
      break;
  }


  printer->feed(2);
  printer->sleep();
}