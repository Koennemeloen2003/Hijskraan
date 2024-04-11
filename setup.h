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
  int e = 0;
  int f = 0;
  int g = 0;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define oledReset 4
// standaard pins
/*
//button Pins
#define buttonOpen 5
#define buttonClose 13
#define buttonStop 19

//led pins
#define redLed 4
#define ledGreen 15

//Joysticks
#define vertRight 32
#define horRight 33
#define vertLeft 35
#define horLeft 34

#define battery 14
*/
/*
//pinnen sem
//button Pins
#define buttonOpen 13
#define buttonClose 15
#define buttonStop 25

//led pins
#define redLed 4
#define ledGreen 2

//Joysticks
//#define vertRight 32
#define horRight 39
#define vertLeft 36
//#define horLeft 39  

//Miscellaneous 
#define battery 32
*/

//button Pins
#define buttonOpen 35
#define buttonClose 34
#define buttonStop 12 

//led pins
#define redLed 13
#define ledGreen 15

//Joysticks
//#define vertRight 32
#define horRight 33
#define vertLeft 36
#define horLeft 39  

//Miscellaneous 
#define battery 32


//Variables
unsigned long stoptimer;
unsigned long sendTimer;
float batteryPercent;
bool batteryLow;
int maxVDeadzone;
int minVDeadzone;
int maxHDeadzone;
int minHDeadzone;
int lierGear;
int katGear;
int servoHoek;


//setting all pins to in- or output
void portSetup(){
  pinMode(buttonOpen, INPUT);
  pinMode(buttonClose, INPUT);
  pinMode(buttonStop, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  //pinMode(vertRight, INPUT);
  pinMode(horRight, INPUT);
  pinMode(vertLeft, INPUT);
//  pinMode(horLeft, INPUT);
  pinMode(battery, INPUT);

}

#endif