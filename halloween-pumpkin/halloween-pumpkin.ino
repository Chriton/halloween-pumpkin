/**
 * A Halloween project
 * Created by Doru Muntean
 * 30 oct 2019
 * 
 * https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/examples/FullFunction/FullFunction.ino
 * ***************************************************
 */

#include "Arduino.h"

// For controlling the mp3 DFPlayer
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// For controlling the PCA9685 servo driver
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"

// Print logs 
#define SERIAL_LOG 115200

// SoftwareSerial to communicate with mp3 DFPlayer
#define SERIAL_COM 9600

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

// Wemos D7 pin (GPIO13) is connected to PIR sensor
# define PIR_PIN 13

/**
 * Wemos D1 pin (GPIO5) is connected to PCA9685 servo driver SCL pin
 * Wemos D2 pin (GPIO4) is connected to PCA9685 servo driver SDA pin
 * called this way, it uses the default address 0x40
 */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

SoftwareSerial mySoftwareSerial(DFPLAYER_RX, DFPLAYER_TX);
DFRobotDFPlayerMini myDFPlayer;
int humanPresent = 0;

unsigned long interval = 4000; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.

void setupEyes();
void setupMp3Player();
void setEyeAngle(int eyeNumber, int angle);
void moveEyes();
void randomTwitch(int servoNumber, int delayInMilisec);


void setup()
{
  // Setup the PIR sensor
  pinMode(PIR_PIN, INPUT);
  
  // Initializes SoftwareSerial to communicate with the mp3 DFPlayer
  mySoftwareSerial.begin(SERIAL_COM);

  // Printing logs
  Serial.begin(SERIAL_LOG);

  // Set up the servo pwm freq and center all the eyes
  setupEyes();

  // Initializes the mp3 DFPlayer
  setupMp3Player();

  // Set the mp3 DFPlayer volume value (0 is min and 30 is max)
  myDFPlayer.volume(5);  
}

void loop()
{
  humanPresent = digitalRead(PIR_PIN);  
    
  if (humanPresent == HIGH) {
    Serial.println("Human is present! Starting to play the sounds...");

    unsigned long currentMillis = millis();
    // check if "interval" time has passed (4000 milliseconds)
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      myDFPlayer.next();
      previousMillis = millis();
    }
      
  } else {
    Serial.println("Human NOT present! Stopping the sounds...");
    //myDFPlayer.stop();
  }

  // Move the eyes randomly
  moveEyes();
  //delay(2000);
}

  
/**
 * Set the eye angle to the specified value
 */
void setEyeAngle(int eyeNumber, int angle) {
    int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
    Serial.println("Setting eye " + String(eyeNumber) + " to angle = " + String(angle));
    pwm.setPWM(eyeNumber, 0, pulselength);
}

/**
 * Set the eye to a random angle (between 45 and 135 degrees)
 */
void randomEyeMovement(int eyeNumber, int delayInMilisec) {
  setEyeAngle(eyeNumber, random(45, 135));
  delay(delayInMilisec);
}


/**
 * Set up the servo pwm freq and center all the eyes
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
  // TODO - replace with for loop
  setEyeAngle(0, 90);
  delay(100);
  setEyeAngle(1, 90);
  delay(100);
  setEyeAngle(2, 90);
  delay(100);
  setEyeAngle(3, 90);
  delay(100);
  setEyeAngle(4, 90);
  delay(100);
  setEyeAngle(5, 90);
  delay(100);
  setEyeAngle(6, 90);
  delay(100);
  setEyeAngle(7, 90);
  delay(100);
  setEyeAngle(8, 90);
  delay(100);
  Serial.println("Done.\n");
}

/**
 * Move the eyes randomly
 */
void moveEyes() {
    
    Serial.println("\nMoving the eyes randomly");
    randomEyeMovement(0, random(100, 200));
    randomEyeMovement(1, random(100, 200));
    randomEyeMovement(2, random(200, 500));
    
    randomEyeMovement(3, random(100, 200));
    randomEyeMovement(4, random(100, 200));
    randomEyeMovement(5, random(200, 500));
    
    randomEyeMovement(6, random(100, 200));
    randomEyeMovement(7, random(100, 200));
    randomEyeMovement(8, random(200, 500));
    Serial.println("Done.\n");
}

/**
 * Initialise the mp3 DFPlayer
 */
void setupMp3Player() {
  Serial.println();
  Serial.println(F("Initializing DFPlayer... (May take 3~5 seconds)"));

  // Use softwareSerial to communicate with mp3 DFPlayer
  if (!myDFPlayer.begin(mySoftwareSerial)) { 
    Serial.println(F("Unable to initialize DFPlayer:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!\n"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online.\n"));
}
