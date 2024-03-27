// Koen Aben

#ifndef setup_h
#define setup_h

#include <esp_now.h>

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

esp_now_peer_info_t peerInfo;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 16
#define oledReset 26

//button Pins
#define buttonOpen 5
#define buttonClose 19
#define buttonStop 13 

//led pins
#define redLed 4
#define ledGreen 15

//Joysticks
#define vertRight 32
#define horRight 33
#define vertLeft 35
#define horLeft 34

//Miscellaneous 
#define battery 14

//Variables
unsigned long screenTimer= 0;
float batteryPercent;
bool batteryLow;
float currentLevel;
int maxVDeadzone;
int minVDeadzone;
int maxHDeadzone;
int minHDeadzone;
int lierGear;
int katGear;
int servoHoek;
bool noodStop = false;
unsigned long stoptimer;
unsigned long sendTimer=0;

void portSetup(){
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

}

#endif