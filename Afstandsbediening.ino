#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <Adafruit_GFX.h>


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
#define vertRight 32
#define horRight 33
#define vertLeft 35
#define horLeft 34

//Miscellaneous 
#define battery 14

//Variables
unsigned long screenTimer= 0;
bool servoOpen = true;
float batteryPercent;
bool batteryLow;
float currentLevel;
int lierGear;
int katGear;

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

  Serial.begin(9600);

    // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display
  oled.setTextSize(0.1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.display();               // show on OLED

}

void updateLierGear(){
    int temp = analogRead(vertLeft)/4095*100;
    switch (temp){
      case 0 ... 15:{
        lierGear = -3;
      }
      break;
      case 16 ... 30:{
        lierGear =-2;
      }
      break;
      case 31 ... 44:{
        lierGear= -1;
      }
      break;
      case 45 ... 55:{
        lierGear = 0;
      }
      break;
      case 56 ... 70:{
        lierGear = 1;
      }
      break;
      case 71 ... 85:{
        lierGear = 2;
      }
      break;
      case 86 ... 100:{
        lierGear = 3;
      }
      break;

    }
}

void updateKatGear(){
    int temp = (analogRead(horRight)*100)/4095;
    switch (temp){
      case 0 ... 15:{
        katGear = -3;
      }
      break;
      case 16 ... 30:{
        katGear =-2;
      }
      break;
      case 31 ... 44:{
        katGear= -1;
      }
      break;
      case 45 ... 55:{
        katGear = 0;
      }
      break;
      case 56 ... 70:{
        katGear = 1;
      }
      break;
      case 71 ... 85:{
        katGear = 2;
      }
      break;
      case 86 ... 100:{
        katGear = 3;
      }
      break;

    }
}

void batteryUpdate(){
  currentLevel=analogRead(battery);
  if (currentLevel -3165 > 0) {
    batteryPercent= ((currentLevel-3165)/(4095-3165))*100;
  }
  else{
    batteryPercent = 0;
  }
  if (batteryPercent < 25){
    batteryLow = true;
  }
  else{
    batteryLow = false;
  }
}

void screenUpdate(){

  oled.clearDisplay(); // clear display

  oled.setCursor(4, 0);        // position to display
  oled.print("Katten: "); // text to display
  oled.println(katGear);

  oled.setCursor(4, 8);        // position to display
  oled.print("Lieren"); // text to display
  oled.println(lierGear);// text to display

  oled.setCursor(100, 8);        // position to display
  oled.print(batteryPercent);// text to display  
  oled.println("%");// text to display

  if (servoOpen == true){
    oled.setCursor(100, 0);        // position to display
    oled.println("Open"); // text to display
  }
  else{
    oled.setCursor(90,0);
    oled.println("closed");
  }
  oled.display();
  screenTimer= millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(screenTimer + 500 <= millis()) {
    batteryUpdate();
    screenUpdate();
  }

  if(batteryLow == true){
    digitalWrite(redLed, HIGH);
  }
  if(buttonClose == HIGH){
    servoOpen= false;
  }
  if (buttonOpen == HIGH){
    servoOpen = true;
  }
  if(buttonStop == HIGH){
    exit(0);
  }
  updateLierGear();
  updateKatGear();
}
