const int   MY_PACKET_VERSION   = 2;    //only processes verion 1 packets

volatile boolean newData=false;

// Must match the transmitter structure
typedef struct struct_message
    {
    byte  packetVersion;
    byte  pattern;
    byte  brightness;
    byte  monoAverage;
    byte  softpot;
    byte  channelData[7];
    } struct_message;

// Create a struct_message called myData
struct_message myData;

// How many leds in your strip?
const int LEDS_ON_STICK   = 59;

const int STICK_PIN       = 13;
const int BRIGHTNESS      = 64;

const int PEAK_DELAY      = 800; //milliseconds
int LEDSOn;
int peakLED;
int peakTimeout;

const int rStickSingleSeq[LEDS_ON_STICK][2] =
        {
         {0,0},  {1,1},  {2,2},  {3,3},  {4,4},  {5,5},
         {0,6},  {1,7},  {2,8},  {3,9}, {4,10}, {5,11},
        {0,12}, {1,13}, {2,14}, {3,15}, {4,16}, {5,17},
        {0,18}, {1,19}, {2,20}, {0,21}, {1,22}, {2,23}
        };
        
// Define the array of leds
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
