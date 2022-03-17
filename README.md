# Music-Display-Stick-Node
Sample WiFi Display System for Straight Line of Lights

This is the code for the receiving end of my WiFi Music Visualizer System. It is being supplied as a sample from which others can create their own display nodes.

It is ESP32 based and uses the [ESP-NOW Protocol](https://www.espressif.com/en/products/software/esp-now/overview) and [FastLED LED Library](https://www.fastled.io). The function of this node is to receive ESP-NOW WiFi packets and display the music information it contains.

## Requirements
1. Anything goes with light and size
2. NO ADDED SOUND
3. Provide your own DC power source
4. Software Requirements
   - Packet Version
     - Your node should only respond to version 1 packets
   - Pattern Numbers (0 - 255 sent by TX)
     - 0 - All lights off, packet dump on local Serial Monitor
     - 1 - 254 Your pattern choices can be whatever you want them to be.
       - Depending on your hardware, pattern selection may not apply
     - 255 - Best of Show
           - If you have multiple patterns, this is the one you want to show off in the final competition
     - Any values outside this range (errors) or unhandled should set the display to black.
   - Brightness (0 - 255 sent by TX)
     - LED brightness MUST respond to all levels
     - Your power supply must be able to support full brightness
     - Startup brightness is 64
   - Soft pot
     - The soft pot is a value that can be adjusted from 0 to 255 at the Head node. It is provided to assist with developement but cannot be used as part of your show demonstration.
   - Audio Data (0-100 sent by TX)
     - 7 Audio Channels
     - 1 Average Channel
     - 0 to 100 %
     - Up to you to use the values with the numbers of lights you have.
5. Hardware Requirements
   - Compatible with Prof. Bland’s TX system
   - Test System available in SPL (20-111)
   - It does not matter what micro-controller or language you use, but…
     - Sample programs are for ESP32, written in C
     - WiFi TX uses ESP-NOW protocol
     - The Transmit system will not be modified to accommodate contestant variances.

## Software Overview

The software for the system is written in C and has been developed in the Arduino IDE.

This sample code is part of the file Music_Stick_Node_ESPNOW.h
```
const int   MY_PACKET_VERSION   = 2;    //only processes version 2 packets

//this must be volatile, as it is changed during an interrupt
volatile boolean newData=false;

// Create a struct_message called myData
// Must match the transmitter structure
struct struct_message
    {
    byte  packetVersion;
    byte  pattern;
    byte  brightness;
    byte  softpot;
    byte  monoAverage;
    byte  channelData[7];
    } myData;

// How many leds in your strip?
const int LEDS_ON_STICK   = 59;

//DIO pin connected to lights
const int STICK_PIN       = 13;

const int   PEAK_DELAY    = 800; //milliseconds

int LEDSOn;
int peakLED;
int peakTimeout;

// Define the array of leds used by FastLED
CRGB Stick[LEDS_ON_STICK];
```
The comments identify the critical constants and variables. You will probably add others as you implement your unique show version.

The main C source starts with the required library includes. The function OnDataRecv is a callback function used by ESP-NOW when data has been received. Since NewData is changed here, it must be declared as volatile.

Setup()does you a favor by printing the MAC address to the Serial Monitor. The MAC address must be added to the Music Info Transmit node in order for your node to be sent data. The rest of the initialization code sets-up ESP-NOW and FastLED.
```
#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>
#include "Music_Stick_Node_ESPNOW.h"

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
    {
    memcpy(&myData, incomingData, sizeof(myData));
    newData = true;
    }

void setup()
    {
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    //print the WiFi MAC address
    Serial.print(F("\r\n\nThis node's WiFi MAC Address is "));
    Serial.print(WiFi.macAddress());
    Serial.println(F(".\r\nYou will need to enter this information in the"));
    Serial.println(F("Head Node program array called 'memberESPs'."));
    Serial.println(F("\nMake sure to adjust the constant NUM_RECEIVERS."));

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
        {
        Serial.println(F("Error initializing ESP-NOW"));
        return;
        }
    
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);

    //FastLED
    FastLED.addLeds<WS2812B, STICK_PIN, GRB>(Stick, LEDS_ON_STICK);
    fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);
    FastLED.show();
    }
```
The code below is an abreviated Loop() function.

The main tasking is:
1. Check for NewData
2. If NewData
   - clear NewData
   - Reject the packet if the version number doesn't match
   - Process the data based on the received pattern number
     - Case 0 is only to dump the packets
     - Cases 1 to 254 can do anything you like
     - Case 255 is to be the "Best in Show" pattern, showing the pattern you think is best in your node.
     - Any patterns you do not define are to be caught by the default case and shut the display off.

```
void loop()
    {
    if (newData)
        {
        //clear flag
        newData = false;

        if (myData.packetVersion != MY_PACKET_VERSION) return;
        
        //process only the pattern numbers implemented.
        //if a pattern is not implemented, turn the display off
        //this includes pattern 0.
        switch (myData.pattern)
            {
            case 0: //required
                fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);
                FastLED.show();
                Serial.print(F("packetVersion: "));    Serial.println(myData.packetVersion);
                Serial.print(F("pattern: "));          Serial.println(myData.pattern);
                Serial.print(F("brightness: "));       Serial.println(myData.brightness);
                Serial.print(F("softpot: "));          Serial.println(myData.softpot);
                Serial.print(F("monoAverage: "));      Serial.println(myData.monoAverage);
                Serial.print(F("channelData: "));      for(int i=0; i<7; i++) Serial.printf("%hu ", myData.channelData[i]);
                Serial.println();
                break;

	    //other pattern cases to go here

            //Best in show
            case 255: //required
                VULoopPH(bluePalette, 0);//Blue, Peak Hold, Average
                break;

            default: //required
                fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);
                FastLED.show();
                break;
            }
        }
    }
```
## Helpful Resources

I HIGHLY Recommend you watch this video to learn the difference between LED Strips. There are many different types and each has their strengths and weaknesses. So watch this video first!

[* LED Strips, what's the difference?](https://www.youtube.com/watch?v=QnvircC22hU)

You might want help configuring the Arduino IDE to support the ESP32. Check out this tutorial page from Random Nerd Tutorials.

[* Setting-up Arduino IDE for ESP32](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

Once youj've installed ESP32 support in the Arduino IDE, the ESP-NOW and WIFi libraries will be automatically available. Here are some links to helpful technical detail.

[* Technical Overview of ESP-NOW](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html#esp-now)

[* Getting Started with ESP-NOW](https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/)

The FastLED library will be a big help to you. Bookmark this site.

[* FastLED Library Home](https://fastled.io/)

Scott Marley has an EXCELLENT YouTube channel filled with tutorials on the FastLED library and some very cool projects. You should take the time to watch these vids from the series, FastLED Basics". Each are shorter than twnety minutes in length.
-[* FastLED Color Names](http://fastled.io/docs/3.1/struct_c_r_g_b.html)

[* FastLED Basics Part 1](https://www.youtube.com/watch?v=4Ut4UK7612M&t=80s)

[* FastLED Basics Part 2](https://www.youtube.com/watch?v=FQpXStjJ4Vc&t=883s)

[* FastLED Basics Part 3](https://www.youtube.com/watch?v=Ukq0tH2Tnkc)

[* FastLED Basics Part 4](https://www.youtube.com/watch?v=2owTxbrmY-s&t=61s)

[* FastLED Basics Part 5](https://www.youtube.com/watch?v=fRXJQVdwrog&t=292s)

[* FastLED Basics Part 6](https://www.youtube.com/watch?v=7Dhh0IMSI4Q&t=73s)


