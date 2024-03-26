#ifndef button_h
#define button_h

#include "setup.h"

   void eStopButton(){
    if(buttonStop == HIGH && noodStop == false){                 // stopt de loop als de noodstop ingedrukt wordt
    myData.E_STOP = 1;
    noodStop = true;
    stoptimer = millis();
    }
    else if (buttonStop == HIGH && noodStop == true && stoptimer + 2000 >= millis()){
    myData.E_STOP = 0;
    noodStop = false;
    }
   }   

  void servoOpen(){
    servoHoek = 1000;
  }

  void servoClose(){
    servoHoek = 2000;
  }
#endif