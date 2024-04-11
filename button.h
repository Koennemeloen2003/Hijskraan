#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
#ifndef button_h
#define button_h

#include "setup.h"
#include "screen.h"

   void eStopButton(){
    if(myData.E_STOP < 1){                 // stopt de loop als de noodstop ingedrukt wordt
      myData.E_STOP = 1;
      sendData();
      stoptimer = millis();
      eStopScreen();
      while (myData.E_STOP > 0) {
        eStopScreen();
        Serial.println("E_stop mode");
        if(digitalRead(buttonStop) == HIGH && stoptimer + 10000 < millis()){
          myData.E_STOP = 0;
      }
      }
    }

   }


#endif