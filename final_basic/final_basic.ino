#include <LPD8806.h>
#include <Adafruit_NeoPixel.h>
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#include "mpr121.h"
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STRIP_2_PIN 11
#define STRIP_3_PIN 5

int irqpin = 2;  // Digital 2
boolean touchStates[12]; //to keep track of the previous touch states

// Number of RGB LEDs in strand:
int nLEDs = 64;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 8;
int clockPin = 9;

LPD8806 strip1 = LPD8806(nLEDs, dataPin, clockPin);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(86, STRIP_2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(86, STRIP_3_PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  #if defined(__AVR_ATtiny85__)// && (F_CPU == 16000000L)
      clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif
//  
//  #if defined (__AVR_ATtiny85__)
//    if (F_CPU == 16000000) 
//        clock_prescale_set(clock_div_1);
//  #endif

  // Start up the LED strip
  strip1.begin();
  strip2.begin();
  strip3.begin();
  // Update the strip, to start they are all 'off'
  strip1.show();
  strip2.show(); // Initialize all pixels to 'off'
  strip3.show();
  
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH); //enable pullup resistor
  
  Serial.begin(9600);
  Wire.begin();

  mpr121_setup();
}

void loop(){
  readTouchInputs();
  // colorOn(strip3.Color(0, 0, 127), 0, strip3.numPixels(), 3);
}


void readTouchInputs(){
  if(!checkInterrupt()){
    
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); 
    
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

    for (int i=0; i < 12; i++){  // Check what electrodes were pressed
      if(touched & (1<<i)){
        if(touchStates[i] == 0){
          //pin i was just touched
          Serial.print("pin ");
          Serial.print(i);
          Serial.println(" was just touched");
          if(i == 11){
            if(touched & (1<<10)){
              colorOn(strip1.Color(127, 0, 127), 0, strip1.numPixels(), 1);
              touchStates[10] = 1;
            } else {
              colorOn(strip1.Color(127, 0, 0), 0, strip1.numPixels()/2, 1);
              colorChase(strip1.Color(127, 0, 0), 20, strip1.numPixels()/2, strip1.numPixels(), 1);
            }
          } 
          else if (i == 10) {
            if(touched & (1<<11)){
              colorOn(strip1.Color(127, 0, 127), 0, strip1.numPixels(), 1);
              touchStates[11] = 1;
            } else {
              colorOn(strip1.Color(127, 0, 0), strip1.numPixels()/2, strip1.numPixels(), 1);
              colorChase(strip1.Color(127, 0, 0), 20, 0, strip1.numPixels()/2, 1);
            }
          } 
          else if (i == 9) {
            if(touched & (1<<8)){
              colorOn(strip2.Color(127, 0, 127), 0, strip2.numPixels(), 2);
              touchStates[8] = 1;
            } else {
              colorOn(strip2.Color(0, 127, 0), 0, strip2.numPixels()/2, 2);
              colorChase(strip2.Color(0, 127, 0), 20, strip2.numPixels()/2, strip2.numPixels(), 2);
            }
          } 
          else if (i == 8) {
            if(touched & (1<<9)){
              colorOn(strip2.Color(127, 0, 127), 0, strip2.numPixels(), 2);
              touchStates[9] = 1;
            } else {
              colorOn(strip2.Color(0, 127, 0), strip2.numPixels()/2, strip2.numPixels(), 2);
              colorChase(strip2.Color(0, 127, 0), 20, 0, strip2.numPixels()/2, 2);
            }
          } 
          else if (i == 7) {
            if(touched & (1<<6)){
              colorOn(strip3.Color(127, 0, 127), 0, strip3.numPixels(), 3);
              touchStates[6] = 1;
            } else {
              colorOn(strip3.Color(0, 0, 127), 0, strip3.numPixels()/2, 3);
              colorChase(strip3.Color(0, 0, 127), 20, strip3.numPixels()/2, strip3.numPixels(), 3);
            }
          } 
          else if (i == 6) {
            if(touched & (1<<7)){
              colorOn(strip3.Color(127, 0, 127), 0, strip3.numPixels(), 3);
              touchStates[7] = 1;
            } else {
              colorOn(strip3.Color(0, 0, 127), strip3.numPixels()/2, strip3.numPixels(), 3);
              colorChase(strip3.Color(0, 0, 127), 20, 0, strip3.numPixels()/2, 3);
            }
          }

        }
      
        touchStates[i] = 1;
      }
      else{
        if(touchStates[i] == 1){
          colorOn(strip1.Color(0, 0, 0), 0, strip1.numPixels(), 1);
          colorOn(strip2.Color(0, 0, 0), 0, strip2.numPixels(), 2);
          colorOn(strip3.Color(0, 0, 0), 0, strip3.numPixels(), 3);
          Serial.print("pin ");
          Serial.print(i);
          Serial.println(" is no longer being touched");
       } 
        touchStates[i] = 0;
      }
      if(touchStates[10] == 1 & touchStates[11] == 1){
        
      }
      if(touchStates[9] == 1 & touchStates[8] == 1){
        colorOn(strip2.Color(127, 0, 127), 0, strip2.numPixels(), 2);
      }
      if(touchStates[7] == 1 & touchStates[6] == 1){
        colorOn(strip3.Color(127, 0, 127), 0, strip3.numPixels(), 3);
      }
    }
    
  }
  else{
    //Serial.print("here\n");
   // colorOn(strip.Color(0, 127, 0), 0, strip.numPixels()); 
  }

}




void mpr121_setup(void){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, TOU_THRESH);
  set_register(0x5A, ELE0_R, REL_THRESH);
 
  set_register(0x5A, ELE1_T, TOU_THRESH);
  set_register(0x5A, ELE1_R, REL_THRESH);
  
  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);
  
  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);
  
  set_register(0x5A, ELE4_T, 0x0A);
  set_register(0x5A, ELE4_R, 0x02);
  
  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);
  
  set_register(0x5A, ELE6_T, 0x08); //middle right
  set_register(0x5A, ELE6_R, 0x08);
  
  set_register(0x5A, ELE7_T, 0x08); //bottom right
  set_register(0x5A, ELE7_R, 0x08);
  
  set_register(0x5A, ELE8_T, 0x03); //top left
  set_register(0x5A, ELE8_R, 0x03);
  
  set_register(0x5A, ELE9_T, 0x04); //top right
  set_register(0x5A, ELE9_R, 0x04);
  
  set_register(0x5A, ELE10_T, 0x06); //middle left
  set_register(0x5A, ELE10_R, 0x06);
  
  set_register(0x5A, ELE11_T, 0x06); //bottom left
  set_register(0x5A, ELE11_R, 0x06);

  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  
  set_register(0x5A, ELE_CFG, 0x0C);
  
}


boolean checkInterrupt(void){
  return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}


//
//void rainbow(uint8_t wait) {
//  int i, j;
//   
//  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
//    for (i=0; i < strip1.numPixels(); i++) {
//      strip1.setPixelColor(i, Wheel( (i + j) % 384));
//    }  
//    strip1.show();   // write all the pixels out
//    delay(wait);
//  }
//}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//  
//  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
//    for (i=0; i < strip1.numPixels(); i++) {
//      // tricky math! we use each pixel as a fraction of the full 384-color wheel
//      // (thats the i / strip.numPixels() part)
//      // Then add in j which makes the colors go around per pixel
//      // the % 384 is to make the wheel cycle around
//      strip1.setPixelColor(i, Wheel( ((i * 384 / strip1.numPixels()) + j) % 384) );
//    }  
//    strip1.show();   // write all the pixels out
//    delay(wait);
//  }
//}


//i made this one it just sets from the start to the end to be on.
//It will be good for when we have the strip around two seperate touch sensors
//and need to light them individually
// Fill the dots progressively along the strip.
void colorOn(uint32_t c, int pixstart, int pixend, int stripNo) {
  if(stripNo == 1){
    int i;
    for (i=pixstart; i < pixend; i++) {
        strip1.setPixelColor(i, c);
    }
    strip1.show();
  }
  else if (stripNo == 2) {
    int i;
    for (i=pixstart; i < pixend; i++) {
        strip2.setPixelColor(i, c);
    }
    strip2.show();
  }
  else if (stripNo == 3) {
    int i;
    for (i=pixstart; i < pixend; i++) {
        strip3.setPixelColor(i, c);
    }
    strip3.show();
  }
}
//
//// Fill the dots progressively along the strip.
//void colorWipe(uint32_t c, uint8_t wait, bool copperStrip) {
//  if(copperStrip){
//      int i;
//    
//      for (i=0; i < strip.numPixels(); i++) {
//          strip.setPixelColor(i, c);
//          strip.show();
//          delay(wait);
//      }
//  }
//  else{
//     int i;
//    
//      for (i=0; i < strip2.numPixels(); i++) {
//          strip2.setPixelColor(i, c);
//          strip2.show();
//          delay(wait);
//      }
//  }
//}
//
// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait, int beginnum, int endnum, int stripNo) {
  int i;
  if(stripNo == 1){
    // Start by turning all pixels off:
    for(i=beginnum; i<endnum; i++) strip1.setPixelColor(i, 0);
    
    // Then display one pixel at a time:
    for(i=beginnum; i<endnum; i++) {
      strip1.setPixelColor(i, c); // Set new pixel 'on'
      strip1.show();              // Refresh LED states
      strip1.setPixelColor(i, 0); // Erase pixel, but don't refresh!
      delay(wait);
    }
    
    strip1.show(); // Refresh to turn off last pixel
  }
  if(stripNo == 2){
    // Start by turning all pixels off:
    for(i=beginnum; i<endnum; i++) strip2.setPixelColor(i, 0);
    
    // Then display one pixel at a time:
    for(i=beginnum; i<endnum; i++) {
      strip2.setPixelColor(i, c); // Set new pixel 'on'
      strip2.show();              // Refresh LED states
      strip2.setPixelColor(i, 0); // Erase pixel, but don't refresh!
      delay(wait);
    }
    
    strip2.show(); // Refresh to turn off last pixel
  }
  if(stripNo == 3){
    // Start by turning all pixels off:
    for(i=beginnum; i<endnum; i++) strip3.setPixelColor(i, 0);
    
    // Then display one pixel at a time:
    for(i=beginnum; i<endnum; i++) {
      strip3.setPixelColor(i, c); // Set new pixel 'on'
      strip3.show();              // Refresh LED states
      strip3.setPixelColor(i, 0); // Erase pixel, but don't refresh!
      delay(wait);
    }
    
    strip3.show(); // Refresh to turn off last pixel
  }
}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//     
//      delay(wait);
//     
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
//    for (int q=0; q < 3; q++) {
//        for (int i=0; i < strip.numPixels(); i=i+3) {
//          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
//        }
//        strip.show();
//       
//        delay(wait);
//       
//        for (int i=0; i < strip.numPixels(); i=i+3) {
//          strip.setPixelColor(i+q, 0);        //turn every third pixel off
//        }
//    }
//  }
//}
///* Helper functions */
//
////Input a value 0 to 384 to get a color value.
////The colours are a transition r - g -b - back to r
//
//uint32_t Wheel(uint16_t WheelPos)
//{
//  byte r, g, b;
//  switch(WheelPos / 128)
//  {
//    case 0:
//      r = 127 - WheelPos % 128;   //Red down
//      g = WheelPos % 128;      // Green up
//      b = 0;                  //blue off
//      break; 
//    case 1:
//      g = 127 - WheelPos % 128;  //green down
//      b = WheelPos % 128;      //blue up
//      r = 0;                  //red off
//      break; 
//    case 2:
//      b = 127 - WheelPos % 128;  //blue down 
//      r = WheelPos % 128;      //red up
//      g = 0;                  //green off
//      break; 
//  }
//  return(strip.Color(r,g,b));
//}
