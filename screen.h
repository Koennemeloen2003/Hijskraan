//Koen Aben

#ifndef screen_h
#define screen_h

#include <Wire.h> //I2C communication library
#include <Adafruit_GFX.h> // supports various OLED graphics functions
#include <Adafruit_SSD1305.h> // OLED 128x32 display driver named SSD1305
#include "setup.h"
#include "joystick.h"

Adafruit_SSD1305 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void screenSetup(){
    // initialize OLED display with address 0x3C for 128x64
  if ( ! oled.begin(0x3C) ) {
    Serial.println("Unable to initialize OLED");
    while (1) yield(); // if we get here, then there is an OLED problem
  }

  delay(2000);                  // wait for initializing
  oled.clearDisplay();          // clear display
  oled.setTextSize(0.1);        // text size
  oled.setTextColor(WHITE);     // text color
  oled.display();               // show on OLED
}


void screenUpdate(){

  oled.clearDisplay(); // clear display

  oled.setCursor(4, 0);        // position to display
  oled.print("Katten: "); // text to display
  oled.println(katGear);

  oled.setCursor(4, 8);        // position to display
  oled.print("Lieren: "); // text to display
  oled.println(lierGear);// text to display

  oled.setCursor(90, 8);        // position to display
  oled.print(batteryPercent);// text to display  
  oled.println("%");// text to display

  if (servoHoek < 1500){
    oled.setCursor(100, 0);        // position to display
    oled.println("Open");          // text to display
  }
  else if (servoHoek >1500) {
    oled.setCursor(80,0);         // position to display
    oled.println("Closed");       // text to display
  }
  
  oled.display();                 // stuurt de bovenstaande info naar het schermpje
}

void eStopScreen(){
  int countdown = map(stoptimer + 10000 - millis(), 0, 10000, 0, 10);
  oled.clearDisplay();          // clear display
  oled.setTextSize(2);        // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(4,2);
  oled.print("E Stop   ");
  if (countdown >= 0){
    oled.println(countdown);
  }
  else{
    oled.println(0);
  }
  oled.display();
  oled.setTextSize(1);
}

#endif