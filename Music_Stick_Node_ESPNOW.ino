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
            case 0:
                fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);
                FastLED.show();
                Serial.print(F("packetVersion: "));    Serial.println(myData.packetVersion);
                Serial.print(F("pattern: "));          Serial.println(myData.pattern);
                Serial.print(F("brightness: "));       Serial.println(myData.brightness);
                Serial.print(F("monoAverage: "));      Serial.println(myData.monoAverage);
                Serial.print(F("channelData: "));      for(int i=0; i<7; i++) Serial.printf("%hu ", myData.channelData[i]);
                Serial.println();
                break;

            //RGB, No Peak Hold, Channel Select
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                VULoop(rgbPalette, myData.pattern);
                break;
                
            //RGB, No Peak Hold, Average
            case 8:
                VULoop(rgbPalette, 0);
                break;

            //Blue, No Peak Hold, Channel Select
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
                VULoop(bluePalette, myData.pattern-8);
                break;
                
            //RGB, No Peak Hold, Average
            case 16:
                VULoop(bluePalette, 0);
                break;

            //RGB, Peak Hold, Channel Select
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
                VULoopPH(rgbPalette, myData.pattern-16);
                break;

            //RGB, Peak Hold, Average
            case 24:
                VULoopPH(rgbPalette, 0);
                break;

            //Blue, Peak Hold, Channel Select
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
                VULoopPH(bluePalette, myData.pattern-24);
                break;

            //Blue, Peak Hold, Average
            case 32:
                VULoopPH(bluePalette, 0);
                break;

            //RGB, Peak Only, Channel Select
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
            case 38:
            case 39:
                VULoopPHdot(rgbPalette, myData.pattern-32);
                break;

            //RGB, Peak Only, Average
            case 40:
                VULoopPHdot(rgbPalette, 0);
                break;

            //Best in show
            case 255:
                VULoopPH(bluePalette, 0);//Blue, Peak Hold, Average
                break;

            default:
                fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);
                FastLED.show();
                break;
            }
        }
    }

void VULoop(CRGBPalette16 palette, int channel)
{
    //set to black
    fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);

    if(channel==0)
        LEDSOn = (int)(LEDS_ON_STICK*((float)myData.monoAverage/100));
    else
        LEDSOn = (int)(LEDS_ON_STICK*((float)myData.channelData[channel-1]/100));
        
    fill_palette(Stick, LEDSOn, 0, PALETTE_STEPS, palette, myData.brightness, NOBLEND);
        
    FastLED.show();
}

void VULoopSplit(CRGBPalette16 palette, int channel)
{
    int stickLEDS = LEDS_ON_STICK/2;
    
    //set to black
    fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);

    if(channel==0)
        LEDSOn = (int)(stickLEDS*((float)myData.monoAverage/100));
    else
        LEDSOn = (int)(stickLEDS*((float)myData.channelData[channel-1]/100));
        
    fill_palette(&Stick[stickLEDS], LEDSOn, 0, PALETTE_STEPS2, palette, myData.brightness, NOBLEND);
    int j = stickLEDS-1;
    for(int i=stickLEDS; i<LEDS_ON_STICK; i++) Stick[j--]=Stick[i];
    Stick[stickLEDS-1] = CRGB(0,0,128);
    Stick[stickLEDS]   = CRGB(0,0,128);
    Stick[stickLEDS+1] = CRGB(0,0,128);
    FastLED.show();
}

void VULoopPH(CRGBPalette16 palette, int channel)
{
    //set to black
    fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);

    //determine how many LEDs shold be on
    if(channel==0)
        LEDSOn = (int)((LEDS_ON_STICK-1)*((float)myData.monoAverage/100));
    else
        LEDSOn = (int)((LEDS_ON_STICK-1)*((float)myData.channelData[channel-1]/100));

    //update the peak
    if(peakLED < LEDSOn)
        {
        //new peak LED
        peakLED = LEDSOn;
        peakTimeout = millis() + PEAK_DELAY;
        }
    else
        {
        //peak hold/delay
        if (peakTimeout < millis()) peakLED--;
        peakLED = max(0, peakLED);
        }

    int paletteIndex     = (int)(255*((float)LEDSOn/LEDS_ON_STICK));
    int peakPaletteIndex = (int)(255*((float)peakLED/LEDS_ON_STICK));
    Stick[peakLED] = ColorFromPalette(palette, peakPaletteIndex, myData.brightness, NOBLEND);
    fill_palette(Stick, LEDSOn, 0, PALETTE_STEPS, palette, myData.brightness, NOBLEND);

    FastLED.show();
}

void VULoopPHdot(CRGBPalette16 palette, int channel)
{
    //set to black
    fill_solid(Stick, LEDS_ON_STICK, CRGB::Black);

    //determine how many LEDs shold be on
    if(channel==0)
        LEDSOn = (int)((LEDS_ON_STICK-1)*((float)myData.monoAverage/100));
    else
        LEDSOn = (int)((LEDS_ON_STICK-1)*((float)myData.channelData[channel-1]/100));

    //update the peak
    if(peakLED < LEDSOn)
        {
        //new peak LED
        peakLED = LEDSOn;
        peakTimeout = millis() + 100;
        }
    else
        {
        //peak hold/delay
        if (peakTimeout < millis()) peakLED--;
        peakLED = max(0, peakLED);
        }

    int peakPaletteIndex = (int)(255*((float)peakLED/LEDS_ON_STICK));
    Stick[peakLED] = ColorFromPalette(palette, peakPaletteIndex, myData.brightness, NOBLEND);
    for(int i=0; i<3; i++) blur1d(Stick,LEDS_ON_STICK,170);

    FastLED.show();
}
