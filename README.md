# Halloween pumpkin with multiple eyes
 This is a Halloween project in which multiple servo controlled eyes placed into a pumpkin are moving randomly and a PIR sensor triggers different scary sounds to be played.
 
 
# Hardware used
- [Wemos D1 mini](https://wiki.wemos.cc/products:d1:d1_mini)
- SG90 servo
- PCA9685 16-Channel 12-bit PWM Servo Motor Driver
- [DFPlayer mp3 player](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299)
- A small speaker
- Connecting wires

# Software used
- [Arduino IDE](https://www.arduino.cc/en/main/software)
- [Fritzing](http://fritzing.org/download/)
 
 
# Arduino IDE config 

### 1. Add [Wemos D1 mini](https://wiki.wemos.cc/products:d1:d1_mini) board support
This project uses a [Wemos D1 mini](https://wiki.wemos.cc/products:d1:d1_mini) so you need to add support for it in [Arduino IDE](https://www.arduino.cc/en/main/software).
To do that, go to Arduino IDE -> Preferences -> Additional boards Manager URLs and add these:
 - https://dl.espressif.com/dl/package_esp32_index.json
 - http://arduino.esp8266.com/stable/package_esp8266com_index.json
 
### 2. Add the additional libraries needed
To control the DFPlayer mp3 player board and the Servo Motor Driver we need to add these additional libraries in Arduino IDE:
 - DFRobotDFplayerMini
 - Adafruit PWM Servo Driver 
  
To add a library in Arduino IDE go to -> Sketch -> Include Library -> Manage Libraries -> Search for the library you need and select Install

   
# Circuit diagram 
TODO - 
You can find the circuit diagram in [halloween-project.fzz](./fritzing/halloween-project.fzz) (it opens with the [Fritzing](http://fritzing.org/download/) app)
 
 
# DFPlayer info
- The sdcard must be formatted with FAT or FAT32
- All audio files must be placed under /mp3 folder
- Audio file must be named 0001.mp3, 0002.mp3.....0255.mp3 (4 letters not 2 or 3 letters)
- Audio file names can have any characters after 4 digits, such as 0001_Bruno_Mars.mp3 

Note that beside the DFPlayer mp3 board you might also use other older boards like the [wtv020m01](https://www.iot-experiments.com/wtv020m01-mp3-to-compatible-wav/), but that one
can only play specific encoded files ([ad4 format](https://www.buildcircuit.com/how-to-convert-mp3-and-wav-files-to-ad4-format-wtv020sd-tutorial/)) and supports only non HC cards up to 2gb.