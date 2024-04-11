#ifndef joystick_h
#define joystick_h

#include "setup.h"

void joystickSetup(){
  int midValueV= analogRead(vertLeft);
  maxVDeadzone= midValueV+20;
  minVDeadzone= midValueV-20;
  int midValueH = analogRead(horRight);
  maxHDeadzone = midValueH+20;
  minHDeadzone = midValueH-20;
}

// Leest de analoge waarde van de joystick uit om zo te bepalen in welke versnelling de lier zit
void updateLierGear(){
    int temp = analogRead(vertLeft); 
        if(temp< minVDeadzone){
      lierGear =  map(temp, 0, minVDeadzone, -650, -1);

    }
    else if (temp > maxVDeadzone){
      lierGear= map(temp, maxVDeadzone, 4095, 1, 650);
    }
    else{
      lierGear =0;
    }
  
}

void updateKatGear(){
    int temp2 = analogRead(horRight); 
    if(temp2 < minHDeadzone){
      katGear =  map(temp2, 0, minHDeadzone, 650, 1);
      
    }
    else if (temp2 > maxHDeadzone){
      katGear= map(temp2, maxHDeadzone, 4095, -1, -650);
    }
    else{
      katGear =0;
    }

}


#endif