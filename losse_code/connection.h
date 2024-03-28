#include "esp32-hal.h"
#ifndef connection_h
#define connection_h

#include <esp_now.h>
#include "WiFi.h"
#include "setup.h"
#include "led.h"

//setting the addres to communicate with
uint8_t broadcastAddress[] = {0x80,0x7D,0x3A,0xFD,0xE5,0xAB};

//turing on the green led if connection is succesful
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
 // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  setWifiLED(status);
}

//print the MAC adress of the board you are connected with
void printMACadress() {
  Serial.printf("Sending to address: { 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X };",
                broadcastAddress[0],
                broadcastAddress[1],
                broadcastAddress[2],
                broadcastAddress[3],
                broadcastAddress[4],
                broadcastAddress[5]);
  Serial.println("\n");
}


//Setting up the connenction
void connectionSetup(){
  WiFi.mode(WIFI_STA);
  printMACadress();
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

//updating the values that are going to be send to the slave module
void updateSendValues() {
  myData.GRIPPER = servoHoek;
  myData.CAT = katGear;
  myData.LIFT = lierGear;
  myData.e += 1;
  myData.f -= 3;
  myData.g *= 2;
  if (myData.e > 9999) myData.e = 255;  // overflow protection
  if (myData.f < -999)  myData.f = 2;    // overflow protection
  if (myData.g > 99999) myData.g = 2;    // overflow protection

}

//sinding the data to the slave module
void  SendData(){
  myData.id = 1;
  Serial.println("---------------");
  Serial.printf("id             value: %d \n", myData.id);
  Serial.printf("E_stop         value: %d \n", myData.E_STOP);
  Serial.printf("Servo_GRIPPER  value: %d \n", myData.GRIPPER);
  Serial.printf("Stepper_CAT    value: %d \n", myData.CAT);  //nema 17
  Serial.printf("Stepper_LIFT   value: %d \n", myData.LIFT);
  Serial.printf("JoystickL      value: %d \n", myData.e);  //nema 23
  Serial.printf("JoystickR      value: %d \n", myData.f);
  Serial.printf("Spare          value: %d \n", myData.g);
  Serial.println("---------------");

  sendTimer= millis();

}
//testing the connnection
void connectionTest(){
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}

#endif