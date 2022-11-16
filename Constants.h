#define calibration_factor -7050.0  //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DELAYVAL 500                // Time (in milliseconds) to pause between pixels

#define DOUT1 6
#define DOUT2 7
#define DOUT3 8
#define DOUT4 9
#define CLK 0
// #define NEOPIN1 5
// #define NEOPIN2 4
// #define NEOPIN3 3
// #define NEOPIN4 2
#define NUMPIXELS 14
#define NUMOFPLATFORMS 4
#define NUMOFFOODS 3
#define BUTTONPIN A1
#define CHGPRDCTBTNPIN A2
#define PRTINFOBTNPIN A3

#define MAXLEDCOUNT 16

#define THRESHOLD_LOWERBOUND 30
#define THRESHOLD_UPPERBOUND 43

typedef enum {
  HAMBURGER = 0,
  ASPARAGUS,
  CHEESE,
} FOOD_PRODUCTS;
