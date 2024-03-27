#include "setup.h"
#include "screen.h"
#include "connection.h"
#include "led.h"
#include "joystick.h"
#include "button.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  portSetup();
  screenSetup();
  connectionSetup();
  joystickSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (screenTimer + 500 < millis()){
    screenUpdate();
    batteryUpdate();
    setBatteryLED(batteryLow);
  }
  if (buttonOpen == HIGH){
    servoOpen();
  }
  if (buttonClose == HIGH){
    servoClose();
  }
  updateKatGear();
  updateLierGear();
  eStopButton();
  if (sendTimer +250 < millis() ){
    updateSendValues();
    connectionTest();
    SendData();
  }

}
