#include "setup.h"
#include "screen.h"
#include "connection.h"
#include "led.h"
#include "joystick.h"
#include "button.h"
#include <Adafruit_GFX.h>



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  portSetup();
  delay(1000);
  screenSetup();
  connectionSetup();
  joystickSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(buttonOpen) == HIGH){
    servoHoek=1000;
  }
  if (digitalRead(buttonClose) == HIGH){
    servoHoek=2000;
  }
  updateKatGear();
  updateLierGear();
  if (digitalRead(buttonStop) == HIGH){
    eStopButton();
  }
  if (sendTimer +250 < millis() ){
    batteryUpdate();
    setBatteryLED(batteryLow);
    screenUpdate();
    updateSendValues();
    sendData();
    printData();
  }

}
