#include "esp32-hal-gpio.h"
#ifndef led_h
#define led_h	

#include "setup.h"

void setWifiLED(bool status) {
  if (status == ESP_NOW_SEND_SUCCESS) {  //LED_Wifi ON/OFF
    digitalWrite(ledGreen,1);
  } else {
    digitalWrite(ledGreen,0);
  }
}

void setBatteryLED(bool state){
  if (state == true){
    digitalWrite(redLed, 1);
  } else{
    digitalWrite(redLed, 0);
  }
}

//Update het persentage van de battery
void batteryUpdate(){
  currentLevel=analogRead(battery); //leest de analoge waarde vanaf de spanningsdeler in
  if (currentLevel -3164 > 0) {     // kijken of het persentage hoger wordt dan 0%  
    batteryPercent= map(currentLevel, 3164, 4095, 0, 100); //rekent het persentage van de battery uit
  }
  else{
    batteryPercent = 0;
  }
  if (batteryPercent < 10){       // als het persentage lager is dan 10 procent is dan gaat hij is lege battery modes
    batteryLow = true;
  }
  else{
    batteryLow = false;
  }
}

#endif