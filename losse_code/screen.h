//Koen Aben

#ifndef screen_h
#define screen_h

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include "setup.h"
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void screenSetup(){
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
  oled.print("Lieren"); // text to display
  oled.println(lierGear);// text to display

  oled.setCursor(100, 8);        // position to display
  oled.print(batteryPercent);// text to display  
  oled.println("%");// text to display

  if (servoHoek =1000){
    oled.setCursor(100, 0);        // position to display
    oled.println("Open");          // text to display
  }
  else if (servoHoek =2000) {
    oled.setCursor(90,0);         // position to display
    oled.println("closed");       // text to display
  }
  oled.display();                 // stuurt de bovenstaande info naar het schermpje
  screenTimer= millis();          // pakt de huidige tijd
}

#endif