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

  Serial.begin(9600); //start seriele monitor

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
    int temp = analogRead(vertLeft)*100/4095; // verandert de analoge input van de joystick naar een persentage
    switch (temp){                            // kijkt in welke versnelling de lier zit op bassis van het persentage
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


// Leest de analoge waarde van de joystick uit om zo te bepalen in welke versnelling de kat zit
void updateKatGear(){
    int temp = (analogRead(horRight)*100)/4095;     // verandert de analoge input van de joystick naar een persentage
    switch (temp){                                  // kijkt in welke versnelling de kat zit op bassis van het persentage
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

//Update het persentage van de battery
void batteryUpdate(){
  currentLevel=analogRead(battery); //leest de analoge waarde vanaf de spanningsdeler in
  if (currentLevel -3164 > 0) {     // kijken of het persentage hoger wordt dan 0%
    batteryPercent= ((currentLevel-3164)*100/(4095-3164));  //rekent het persentage van de battery uit
  }
  else{
    batteryPercent = 0;
  }
  if (batteryPercent < 25){       // als het persentage lager is dan 25 procent is dan gaat hij is lege battery modes
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
  oled.print("Lieren"); // text to display
  oled.println(lierGear);// text to display

  oled.setCursor(100, 8);        // position to display
  oled.print(batteryPercent);// text to display  
  oled.println("%");// text to display

  if (servoOpen == true){
    oled.setCursor(100, 0);        // position to display
    oled.println("Open");          // text to display
  }
  else{
    oled.setCursor(90,0);         // position to display
    oled.println("closed");       // text to display
  }
  oled.display();                 // stuurt de bovenstaande info naar het schermpje
  screenTimer= millis();          // pakt de huidige tijd
}

void loop() {
  // put your main code here, to run repeatedly:
  if(screenTimer + 500 <= millis()) {     //vergetlijkt de huidige tijd met de laatste keer dat het scherm geupdate is
    batteryUpdate();                      // roept de functie om het batterypercentage te hernieuwen
    screenUpdate();                       // roept de functie aan om het scherm te hernieuwen
  }

  if(batteryLow == true){                 // zet de rooie led aan als de battarij bijna leeg is
    digitalWrite(redLed, HIGH);
  }
  if(buttonClose == HIGH){                // sluit de grijper als de knop ingedrukt is
    servoOpen= false;
  }
  if (buttonOpen == HIGH){                // opent de servo als de knop ingedrukt wordt
    servoOpen = true;
  }
  if(buttonStop == HIGH){                 // stopt de loop als de noodstop ingedrukt wordt
    exit(0);
  }
  updateLierGear();                     // roept de functie aan om de versnelling van de lier te hernieuwen
  updateKatGear();                      // roept de functie aan om de versnelling van de kat te hernieuwen
}
