/**
 * A Halloween project
 * Created by Doru Muntean
 * 30 oct 2019
 * ***************************************************
 */

#include "Arduino.h"

// Print logs 
#define SERIAL_LOG 115200

// SoftwareSerial to communicate with mp3 DFPlayer
#define SERIAL_COM 9600

// For controlling the mp3 DFPlayer
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// For controlling the PCA9685 servo driver
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"

/**
 * Depending on your servo make, the pulse width min and max may vary, you  want
 * these to be as small/large as possible without hitting the hard stop for max range.
 * You'll have to tweak them as necessary to match the servos you have!
 */
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

/**
 * Wemos D6 pin (GPIO14) is connected to DFPlayer RX pin
 * Wemos D5 pin (GPIO12) is connected to DFPlayer TX pin
 */
# define DFPLAYER_RX 14
# define DFPLAYER_TX 12

/**
 * Wemos D1 pin (GPIO5) is connected to PCA9685 servo driver SCL pin
 * Wemos D2 pin (GPIO4) is connected to PCA9685 servo driver SDA pin
 * called this way, it uses the default address 0x40
 */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

SoftwareSerial mySoftwareSerial(DFPLAYER_RX, DFPLAYER_TX);
DFRobotDFPlayerMini myDFPlayer;

void setupEyes();
void setupMp3Player();
void setEyeAngle(int eyeNumber, int angle);
void randomTwitch(int servoNumber,int delayInMilisec);
void printDetail(uint8_t type, int value);


void setup()
{
  // Initializes SoftwareSerial to communicate with mp3 DFPlayer
  mySoftwareSerial.begin(SERIAL_COM);

  // Printing logs
  Serial.begin(SERIAL_LOG);

  // Set up the servo pwm freq and center all the eyes
  setupEyes();

  // Initializes the mp3 DFPlayer
  setupMp3Player();
  
  myDFPlayer.volume(3);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
  myDFPlayer.play(1);  //Play the first mp3
}

void loop()
{
  static unsigned long timer = millis();
  
  if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  // Move the Eyes randomly
  Serial.println("\nMoving the eyes randomly");
  randomEyeMovement(0, random(200, 500));
  randomEyeMovement(1, random(200, 500));
  randomEyeMovement(2, random(200, 500));
  randomEyeMovement(3, random(200, 500));
  randomEyeMovement(4, random(200, 500));
//  randomEyeMovement(5, random(500, 1000));
//  randomEyeMovement(6, random(500, 1000));
//  randomEyeMovement(7, random(500, 1000));
//  randomEyeMovement(8, random(500, 1000));
//  randomEyeMovement(9, random(500, 1000));
  Serial.println("Done.\n");
}


/**
 * This will set the eye angle to the specified value
 */
void setEyeAngle(int eyeNumber, int angle) {
    int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
    Serial.println("Setting eye " + String(eyeNumber) + " to angle = " + String(angle));
    pwm.setPWM(eyeNumber, 0, pulselength);
}

/**
 * This will set the eye to a random angle (between 45 and 135 degrees)
 */
void randomEyeMovement(int eyeNumber,int delayInMilisec) {
  delay(delayInMilisec);
  setEyeAngle(eyeNumber, random(45,135));
}


/**
 * This will set up the servo pwm and center all the eyes
 */
void setupEyes() {

  // Setting up the servos
  Serial.println("\n\nSetting up the servo pwm freq...");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  Serial.println("Done.\n");
  delay(10);

  // Centering all the eyes - 9 eyes
  Serial.println("Centering all the Eyes...");
  setEyeAngle(0, 90);
  setEyeAngle(1, 90);
  setEyeAngle(2, 90);
  setEyeAngle(3, 90);
  setEyeAngle(4, 90);
  setEyeAngle(5, 90);
  setEyeAngle(6, 90);
  setEyeAngle(7, 90);
  setEyeAngle(8, 90);
  Serial.println("Done.\n");
  delay(10);
}

/**
 * Initializes the mp3 DFPlayer
 */
void setupMp3Player() {
  Serial.println();
  Serial.println(F("Initializing DFPlayer... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to initialize DFPlayer:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!\n"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online.\n"));
}

/**
 * Print mp3 DFPlayer details 
 */
void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
