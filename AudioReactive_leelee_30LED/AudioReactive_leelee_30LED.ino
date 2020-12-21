#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 30
#define BAND_0_PIXELS 6
#define BAND_1_PIXELS 3
#define BAND_2_PIXELS 5
#define BAND_3_PIXELS 6
#define BAND_4_PIXELS 4
#define BAND_5_PIXELS 2
#define BAND_6_PIXELS 4

int pixelsPerBand[7] = {
  BAND_0_PIXELS,
  BAND_1_PIXELS,
  BAND_2_PIXELS,
  BAND_3_PIXELS,
  BAND_4_PIXELS, 
  BAND_5_PIXELS,
  BAND_6_PIXELS,
};



//  {0,1,13,14,26,27},
//  {2,15,28},
//  {3,4,16,17,29},
//  {5,6,7,18,19,20},
//  {8,9,21,22},
//  {10,23},
//  {11,12,24,25}
//};



int pixelColorPerBand[7][3] = {
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
int pixelNumsPerBand[7][5];


unsigned int freqInBand[7]; //Store each of the 7 peaks deatected in each 
//frequency band into an array.


void setup()
{
 pixels.begin(); // This initializes the NeoPixel library.
 Serial.begin(9600);
 pinMode(strobe, OUTPUT);
 pinMode(reset, OUTPUT); 
 digitalWrite(reset,LOW); //Reset Low enables strobe pin 
 digitalWrite(strobe,HIGH); //After the first strobe leading edge, 63 Hz output is on output pin (
for (int i =0; i < 7; i++) {
  for (int j =0; j < 5; j++) {
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
 for (int band = 0; band < 7; band++)
 { 
   digitalWrite(strobe, LOW); //enables output 
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   unsigned int calc = analogRead(OUT)-125 ;
   freqInBand[band] = (calc > 0  && calc < 1023) ? calc : 5;
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   digitalWrite(strobe, HIGH);
   delay(10); //Output settling time is 36uS min. Could get bad data without this delay

   for (int i = 0; i < pixelsPerBand[band]; i++) {
       pixels.setPixelColor(pixelNumsPerBand[band][i], pixels.Color((pixelColorPerBand[band][0] * freqInBand[band]) / 1024, (pixelColorPerBand[band][1] * freqInBand[band]) / 1024, (pixelColorPerBand[band][2] * freqInBand[band]) / 1024));
   }
   pixels.show();
 }
}
