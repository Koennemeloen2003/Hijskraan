#include "setup.h"
#include "screen.h"
#include "connection.h"
#include "led.h"
#include "joystick.h"
#include "button.h"
#include <Adafruit_SSD1306.h>



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
  Serial.print(myData.E_STOP);
  if (digitalRead(buttonOpen) == HIGH){
    servoHoek=1000;
  }
if (digitalRead(buttonClose) == HIGH){
    servoHoek=2000;
  }
  updateKatGear();
  updateLierGear();
  if (screenTimer + 500 < millis()){
    screenUpdate();
    batteryUpdate();
    setBatteryLED(batteryLow);
  }
  if (digitalRead(buttonStop) == HIGH){
    eStopButton();
  }

  if (sendTimer +250 < millis() ){
    updateSendValues();
    connectionTest();
    SendData();
  }

}
