#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMBANDS 7
#define PIXELS_PER_BAND 26
#define NUMPIXELS 180
#define BAND_0_PIXELS 24
#define BAND_1_PIXELS 24
#define BAND_2_PIXELS 25
#define BAND_3_PIXELS 26
#define BAND_4_PIXELS 24
#define BAND_5_PIXELS 25
#define BAND_6_PIXELS 26

int pixelsPerBand[NUMBANDS] = {
  BAND_0_PIXELS,
  BAND_1_PIXELS,
  BAND_2_PIXELS,
  BAND_3_PIXELS,
  BAND_4_PIXELS, 
  BAND_5_PIXELS,
  BAND_6_PIXELS,
};



//  {0,1,13,14,26,27,39,40,52,53,65,66,78,79,91,92,104,105,117,118,130,131,143,144},
//  {2,15,28,41,54,67,80,93,106,119,132,145},
//  {3,4,16,17,29,30,42,43,55,56,68,69,81,82,94,95,107,108,120,121,133,134,146,147},
//  {5,6,7,18,19,20,31,32,33,44,45,46,57,58,59,70,71,72,83,84,85,96,97,98,109,110,111,122,123,124,135,136,137,148,149,150},
//  {8,9,21,22,34,35,47,48,60,61,73,74,86,87,99,100,112,113,125,126,138,139,151,152},
//  {10,23,36,49,62,75,88,101,114,127,140,153},
//  {11,12,24,25,37,38,50,51,63,64,76,77,89,90,102,103,115,116,128,129,141,142,154,155}
//};



int pixelColorPerBand[NUMBANDS][3] = {
  {0, 0, 153},      
  {0, 204, 153},      
  {204, 51, 255},       
  {255, 255, 102},     
  {255, 255, 102},        
  {255, 153, 0},     
  {255, 0, 0}    
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int OUT = A0;

int strobe = 4;

int reset = 3;

int counter = 0;
int pixelNumsPerBand[NUMBANDS][PIXELS_PER_BAND];


unsigned int freqInBand[NUMBANDS]; //Store each of the 7 peaks deatected in each 
//frequency band into an array.


void setup()
{
 pixels.begin(); // This initializes the NeoPixel library.
 Serial.begin(9600);
 pinMode(strobe, OUTPUT);
 pinMode(reset, OUTPUT); 
 digitalWrite(reset,LOW); //Reset Low enables strobe pin 
 digitalWrite(strobe,HIGH); //After the first strobe leading edge, 63 Hz output is on output pin
for (int i =0; i < NUMBANDS; i++) {
  for (int j =0; j < PIXELS_PER_BAND; j++) {
    pixelNumsPerBand[i][j] = counter++;
  }
}
}


void loop()
{

 Serial.println(analogRead(A0));
 digitalWrite(reset, HIGH);
 delayMicroseconds(1); //Reset Pluse width is 100nS min
 digitalWrite(reset, LOW); //reset low enables strobe Pin
 for (int band = 0; band < NUMBANDS; band++)
 { 
   digitalWrite(strobe, LOW); //enables output 
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   unsigned int calc = analogRead(OUT)-125 ;
   freqInBand[band] = (calc > 0  && calc < 1023) ? calc : 5;
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   digitalWrite(strobe, HIGH);
   delay(10); //Output settling time is 36uS min. Could get bad data without this delay

   for (int i = 0; i < pixelsPerBand[band]; i++) {
    pixels.setPixelColor(pixelNumsPerBand[band][i],
                         pixels.Color((pixelColorPerBand[band][0] * freqInBand[band]) / 1024,
                                      (pixelColorPerBand[band][1] * freqInBand[band]) / 1024,
                                      (pixelColorPerBand[band][2] * freqInBand[band]) / 1024)
                         );
   }
   pixels.show();
 }
}
