const int   MY_PACKET_VERSION   = 1;    //only processes version 1 packets

volatile boolean newData=false;

// Create a struct_message called myData
// Must match the transmitter structure
struct struct_message
    {
    byte  packetVersion;
    byte  pattern;
    byte  brightness;
    byte  monoAverage;
    byte  channelData[7];
    } myData;

// How many leds in your strip?
const int LEDS_ON_STICK = 59;

//DIO pin connected to lights
const int STICK_PIN     = 13;

const int PEAK_DELAY    = 800; //milliseconds

int LEDSOn;
int peakLED;
int peakTimeout;

// Define the array of leds used by FastLED

CRGB Stick[LEDS_ON_STICK];

//VU
DEFINE_GRADIENT_PALETTE(roundabout){
      0, 255,   0,   0,
     85,   0, 255,   0,
    170,   0,   0, 255,
    255, 255,   0,   0
};
CRGBPalette16 ripplePalette = roundabout;

DEFINE_GRADIENT_PALETTE(RGB_VU){
      0,   0, 255,   0,
    125,   0, 255,   0,
    150, 255, 255,   0,
    187, 255,   0,   0,
    255, 255,   0,   0
};
CRGBPalette16 rgbPalette = RGB_VU;

DEFINE_GRADIENT_PALETTE(BLUE_VU){
      0,   0,   0, 255,
    100,   0,   0, 255,
    175, 255, 255, 255,
    255, 255, 255, 255
};
CRGBPalette16 bluePalette = BLUE_VU;

DEFINE_GRADIENT_PALETTE(RED_VU){
      0, 255,   0,   0,
    100, 255,   0,   0,
    175, 255, 255, 255,
    255, 255, 255, 255
};
CRGBPalette16 redPalette = RED_VU;

DEFINE_GRADIENT_PALETTE(GREEN_VU){
      0,   0, 255,   0,
    100,   0, 255,   0,
    175, 255, 255, 255,
    255, 255, 255, 255
};
CRGBPalette16 greenPalette = GREEN_VU;

const int PALETTE_STEPS     = (int)((float)255 / LEDS_ON_STICK);
const int PALETTE_STEPS2    = (int)((float)255 / (LEDS_ON_STICK/2));

//function declarations
void VULoop(CRGBPalette16);
