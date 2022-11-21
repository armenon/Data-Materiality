#include "Printer.h"
// Printer::Printer(){
//   // printer_internal = printer;
// }

enum {
  AGRICULTURE = 1,
  PROCESSING,
  PACKAGING,
  TRANSPORT
};

void initialisePrinter(Adafruit_Thermal *printer) {
  Serial1.begin(19200);  // Use this instead if using hardware serial
  printer->begin();
  printer->feed(1);
  printer->setDefault();
  printer->sleep();
  delay(500);
}


void printInitialMessage(Adafruit_Thermal *printer) {
  printer->wake();
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

  printer->feed(4);
  printer->setDefault();  // Restore printer to defaults
  printer->sleep();
}

void printTryCount(Adafruit_Thermal *printer, int tryCount) {
  printer->wake();
  printer->setFont('B');
  printer->setSize('M');
  printer->print("It took you ");
  printer->print(tryCount);
  printer->print(" tries to estimate this by feeling the weight of the CO2 emissions.");
  printer->print("I will try to find out more about this product now.");
  printer->feed(4);
  printer->setDefault();
  printer->sleep();
}

void printProductInfo(Adafruit_Thermal *printer, int currentProduct, bool isAllWgtsCorrect) {
  printer->wake();
  printer->setSize('S');
  printer->setFont('B');
  printer->justify('L');


  if (isAllWgtsCorrect) {
    printer->println("So here's what I found out -");
    printer->feed(2);

    switch (currentProduct) {
      case 0:
        printer->println("A McDonalds hamburger contains beef, bread and lettuce that were produced in Sweden. The main source of emissions comes from cow husbandry which requires a lot of land and water use.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Agriculture\t\t 1.61kg CO2e");
        printer->feed(1);

        printer->setSize('S');
        printer->println("The energy used to process meat, make the bread emits some of CO2 depending on how it is produced.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Processsing\t\t 0.085kg CO2e");
        printer->feed(1);

        printer->setSize('S');
        printer->println("These components are then packaged for transport, usually in plastic coverings.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Packaging\t\t 0.07kg CO2e");
        printer->feed(1);

        printer->setSize('S');
        printer->println("The burger components were then transported to its current location using trucks that run on biodiesel");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Transport\t\t 0.07kg CO2e");
        break;
      case 1:
        printer->println(F("Asparagus (which comes originally from the Middle East) is a healthy nutritious plant with a wide range of nutrients and minerals and high vitamin content."));
        printer->feed(1);
        printer->setSize('M');
        printer->println("Agriculture\t\t 0.1kg CO2e");
        printer->feed(1);

        printer->setSize('S');
        printer->println(F("Asparagus does not require much processing apart from some cleaning after harvest."));
        printer->feed(1);
        printer->setSize('M');
        printer->println("Processing\t\t 0.01kg CO2e");

        printer->setSize('S');
        printer->println(F("The asparagus was packaged loose or in bundles in cartons and baskets made of wood or plastic. They are often covered at the bottom (especially in plastic) to prevent them from drying out."));
        printer->feed(1);
        printer->setSize('M');
        printer->println("Packaging\t\t 0.1kg CO2e");
        printer->feed(1);

        printer->setSize('S');
        printer->println(F("Asparagus is available locally in Sweden between May and June. During other months, it needs to be brought in from other parts of the world. It is usually in season in South America from June to December. However, due to its relatively short shelf life of 1-2 days, asparagus is often transported by air freight. And it needs to be refrigerated to improve shelf life."));
        printer->feed(1);
        printer->setSize('M');
        printer->println("Transport\t\t 1.9kg CO2e");
        break;
      case 2:
        printer->println("Arla Cheese uses milk produced in Sweden. This milk comes from cows, whose husbandry requires a lot of land and water, which results in large amounts of emissions.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Agriculture\t\t 0.74kg CO2e");
        printer->feed(1);

        printer->setSize('S');
        printer->println("The milk requires processing in order to be turned into cheese, which takes both time and energy.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Processing\t\t 0.36kg CO2e");

        printer->setSize('S');
        printer->println("The cheese so made, is then packaged in plastic covering to keep the cheese fresh and prolong its shelf life.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Packaging\t\t 0.18kg CO2e");

        printer->setSize('S');
        printer->println("The cheese was then transported to its current location, using diesel trucks alongwith other Arla dairy products.");
        printer->feed(1);
        printer->setSize('M');
        printer->println("Transport\t\t 0.105kg CO2e");
        break;
      default:
        printer->print("Hmmmm.. I don't know much about this product yet");
        break;
    }

  } else {
    String p = "Hmmmm.. I don't know much about this product yet";
    printer->print(p);
  }
  printer->feed(4);
  printer->setDefault();
  printer->sleep();
}

// String asparagusInfo(int section) {
//   switch (section) {
//     case AGRICULTURE:
//       return ("Asparagus (which comes originally from the Middle East) is a healthy nutritious plant with a wide range of nutrients and minerals and high vitamin content.");
//     case PROCESSING:
//     return;
//     case PACKAGING:
//     return;
//     case
//   }
//   return
// }