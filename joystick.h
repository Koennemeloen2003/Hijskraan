#ifndef joystick_h
#define joystick_h

#include "setup.h"

void joystickSetup(){
    int midValueV= analogRead(vertLeft);
  maxVDeadzone= midValueV+15;
  minVDeadzone= midValueV-15;
  int midValueH = analogRead(horRight);
  maxHDeadzone = midValueH+15;
  minVDeadzone = midValueH-15;
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
    int temp = analogRead(horRight); 
    if(temp< minHDeadzone){
      katGear =  map(temp, 0, minVDeadzone, -650, -1);
      
    }
    else if (temp > maxHDeadzone){
      katGear= map(temp, maxHDeadzone, 4095, 1, 650);
    }
    else{
      katGear =0;
    }
}


#endif