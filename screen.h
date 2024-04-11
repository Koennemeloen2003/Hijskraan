//Koen Aben

#ifndef screen_h
#define screen_h

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include "setup.h"
#include "joystick.h"

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void screenSetup(){
    // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed")); // print een error uit als het upzette van het schermpje niet lukt
    while (true);
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

  oled.setCursor(100, 8);        // position to display
  oled.print(batteryPercent);// text to display  
  oled.println("%");// text to display

  if (servoHoek < 1500){
    oled.setCursor(100, 0);        // position to display
    oled.println("Open");          // text to display
  }
  else if (servoHoek >1500) {
    oled.setCursor(90,0);         // position to display
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