#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <esp_now.h>
#include <Adafruit_GFX.h>
#include "WiFi.h"

// REPLACE WITH THE RECEIVER'S MAC Address
//uint8_t broadcastAddress[] = {0x48, 0xE7, 0x29, 0xB6, 0x94, 0x50};
uint8_t broadcastAddress[] = {0x80,0x7D,0x3A,0xFD,0xE5,0xA8};

//setup of the message sent to the reciever
typedef struct struct_message {
  int id = 1;  // must be unique for each sender board
  int E_STOP;
  int GRIPPER;
  int CAT;
  int LIFT;
  int e = 255;
  int f;
  int g = 2;
} struct_message;

struct_message myData;

//Screen set-up
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 16
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define oledReset 26

//button Pins
#define buttonOpen 5
#define buttonClose 19
#define buttonStop 13 

//led pins
#define redLed 4
#define ledGreen 15

//Joysticks
#define vertRight 35
#define horRight 34
#define vertLeft 32
#define horLeft 33

//Miscellaneous 
#define battery 14

//Variables
unsigned long screenTimer= 0;
float batteryPercent;
bool batteryLow;
int maxVDeadzone;
int minVDeadzone;
int maxHDeadzone;
int minHDeadzone;
int lierGear;
int katGear;
int servoHoek;
unsigned long stoptimer;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
 // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  setWifiLED(status);
}

void setWifiLED(bool status) {
  if (status == ESP_NOW_SEND_SUCCESS) {  //LED_Wifi ON/OFF
    digitalWrite(ledGreen,1);
  } else {
    digitalWrite(ledGreen,0);
  }
}

void setup() {
  // put your setup code here, to run once:
  //Setting alle pins
  pinMode(buttonOpen, INPUT);
  pinMode(buttonClose, INPUT);
  pinMode(buttonStop, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(vertRight, INPUT);
  pinMode(horRight, INPUT);
  pinMode(vertLeft, INPUT);
  pinMode(horLeft, INPUT);
  pinMode(battery, INPUT);
  int midValueV= analogRead(vertLeft);
  maxVDeadzone= midValueV+20;
  minVDeadzone= midValueV-20;
  int midValueH = analogRead(horRight);
  maxHDeadzone = midValueH+20;
  minHDeadzone = midValueH-20;

  Serial.begin(9600); //start seriele monitor

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

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed")); // print een error uit als het upzette van het schermpje niet lukt
    while (true);
  }

  delay(2000);                  // wait for initializing
  oled.clearDisplay();          // clear display
  oled.setTextSize(0.1);        // text size
  oled.setTextColor(WHITE);     // text color
  oled.display();               // show on OLED

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


// Leest de analoge waarde van de joystick uit om zo te bepalen in welke versnelling de kat zit
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

//update het scherm met de nieuwe informatie
void screenUpdate(){

  oled.clearDisplay(); // clear display

  oled.setCursor(4, 0);        // position to display
  oled.print("Katten: "); // text to display
  oled.println(katGear);

  oled.setCursor(4, 8);        // position to display
  oled.print("Lieren: "); // text to display
  oled.println(lierGear);// text to display

  oled.setCursor(100, 8);        // position to display
  oled.print(batteryPercent);// text to display  
  oled.println("%");// text to display

  if (servoHoek < 1500){
    oled.setCursor(100, 0);        // position to display
    oled.println("Open");          // text to display
  }
  else if (servoHoek > 1500) {
    oled.setCursor(90,0);         // position to display
    oled.println("closed");       // text to display
  }
  oled.display();                 // stuurt de bovenstaande info naar het schermpje
  screenTimer= millis();          // pakt de huidige tijd
}

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

}

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

void eStopScreen(){
  int countdown = map(stoptimer + 10000 - millis(), 0, 10000, 0, 10);
  oled.clearDisplay();          // clear display
  oled.setTextSize(2);        // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(4,2);
  oled.print("E Stop   ");
  if (countdown >= 0){
    oled.println(countdown);
  }
  else{
    oled.println(0);
  }
  oled.display();
  oled.setTextSize(1);
}

void eStopButton(){
  if(myData.E_STOP < 1){                 // stopt de loop als de noodstop ingedrukt wordt
    myData.E_STOP = 1;
    SendData();
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


void loop() {
  // put your main code here, to run repeatedly:
  //Sero hoek bepalen
  if(digitalRead(buttonOpen) == 1){
    servoHoek = 1000;
  }

  if (digitalRead(buttonClose) == 1) {
    servoHoek = 2000;
    Serial.println("test");
  }

  if(screenTimer + 500 <= millis()) {     //vergetlijkt de huidige tijd met de laatste keer dat het scherm geupdate is
    batteryUpdate();                      // roept de functie om het batterypercentage te hernieuwen
    screenUpdate();                       // roept de functie aan om het scherm te hernieuwen
  }

  if(batteryLow == true){                 // zet de rooie led aan als de battarij bijna leeg is
    digitalWrite(redLed, HIGH);
  }

  if(digitalRead(buttonStop) == HIGH {
    eStopButton();
  }
  updateLierGear();                     // roept de functie aan om de versnelling van de lier te hernieuwen
  updateKatGear();                      // roept de functie aan om de versnelling van de kat te hernieuwen
  updateSendValues();

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  SendData();
}
