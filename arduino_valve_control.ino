/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int GasValve1 = 2;
int GasValve2 = 4;
int GasValve3 = 7;

struct Toggle{
    int valve_time;
    char *valve;
    struct Toggle *next;
    struct Toggle *prev;
};
  
struct Toggle *gasTime1 = (struct Toggle*) malloc(sizeof(struct Toggle));
struct Toggle *gasTime2 = (struct Toggle*) malloc(sizeof(struct Toggle));
struct Toggle *gasTime3 = (struct Toggle*) malloc(sizeof(struct Toggle));
struct Toggle *current = NULL;


void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  //set toggle menu items
  setToggle();
  // set first item
  current = gasTime1;

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  int time = millis();
//  lcd.print("Press Select to run program");
  time = millis() - time;
  Serial.print("Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(RED);
}

void setToggle(){\
// initialize menu items
  gasTime1->valve = strdup("Set Gas Time 1");
  gasTime1->valve_time = 0;
  gasTime2->valve = strdup("Set Gas Time 2");
  gasTime2->valve_time = 0;
  gasTime3->valve = strdup("Set Gas Time 3");

//  Connec the toggle menu
  gasTime1->next = gasTime2;
  gasTime1->prev = gasTime3;
  gasTime2->next = gasTime3;
  gasTime2->prev = gasTime1;
  gasTime3->next = gasTime1;
  gasTime3->prev = gasTime2;
}

uint8_t i=0;
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(current->valve);
  lcd.setCursor(0,1);
  lcd.print(current->valve_time);

  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    if (buttons & BUTTON_UP) {
      current->valve_time += 30;
      delay(500);    
    }
    if (buttons & BUTTON_DOWN) {
      current->valve_time -= 15;
      delay(500);
    }
    if (buttons & BUTTON_LEFT) {
      current = current->prev;
      delay(500);
    }
    if (buttons & BUTTON_RIGHT) {
      current = current->next;
      lcd.print(current->valve);
      delay(500);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.print("SELECT ");
      lcd.setBacklight(VIOLET);
    }
  }
}
