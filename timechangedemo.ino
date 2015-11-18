#include <CapacitiveSensor.h>

//int led = 8;                                          //change '42' to any desired pin...

int const rPotPin = A2;
int const gPotPin = A1;
int const bPotPin = A0;

int const rOutPin = 9;
int const gOutPin = 10;
int const bOutPin = 11;

int rVal;
int gVal;
int bVal;

long time = 0;
int state = HIGH;

boolean yes;
boolean previous = false;

int debounce = 200;

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

// To add more sensors...
//CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
//CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

void setup()                    
{
   Serial.begin(9600);
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);  //Calibrate the sensor... 
   //pinMode(led, OUTPUT);
   pinMode(rOutPin, OUTPUT);
   pinMode(gOutPin, OUTPUT);
   pinMode(bOutPin, OUTPUT);
}

void loop()                    
{
    
    long total1 =  cs_4_2.capacitiveSensor(30);

    //Serial.println(time);
    
    if (total1 > 1000){yes = true;}
    else {yes = false;}    
    
    // to toggle the state of state
    if(yes == true && previous  == false && millis() - time>debounce){

      //reset timer here
       if(state == LOW){
         state = HIGH;

       }
       else 
         state = LOW;
         time = millis();     
       
    }
       

    if(state == HIGH){

       //green
       analogWrite(rOutPin, 255);
       analogWrite(gOutPin, 0);
       analogWrite(bOutPin, 0);
       
       if(1000 < total1) {

          if ((millis() - time) > 6000) {

             analogWrite(rOutPin, 102);
             analogWrite(gOutPin, 0);
             analogWrite(bOutPin, 204);
          }

          else if ((millis() - time) > 4500) {

             analogWrite(rOutPin, 0);
             analogWrite(gOutPin, 0);
             analogWrite(bOutPin, 255);
          }

          else if ((millis() - time) > 3000) {

             analogWrite(rOutPin, 0);
             analogWrite(gOutPin, 255);
             analogWrite(bOutPin, 0);
          }

          else if ((millis() - time) > 1500) {

             //maize
             analogWrite(rOutPin, 251);
             analogWrite(gOutPin, 236);
             analogWrite(bOutPin, 93);
          }
        
       }
    }

    if(state == LOW){
      
       analogWrite(rOutPin, 0);
       analogWrite(gOutPin, 0);
       analogWrite(bOutPin, 128);
       
       if(1000 < total1) {

          if ((millis() - time) > 6000) {

             analogWrite(rOutPin, 102);
             analogWrite(gOutPin, 0);
             analogWrite(bOutPin, 204);
          }

          else if ((millis() - time) > 4500) {

             analogWrite(rOutPin, 0);
             analogWrite(gOutPin, 0);
             analogWrite(bOutPin, 255);
          }

          else if ((millis() - time) > 3000) {

             analogWrite(rOutPin, 0);
             analogWrite(gOutPin, 255);
             analogWrite(bOutPin, 0);
          }

          else if ((millis() - time) > 1500) {

             //maize
             analogWrite(rOutPin, 251);
             analogWrite(gOutPin, 236);
             analogWrite(bOutPin, 93);
          }
        
       }
       
    }
    
    //digitalWrite(led, state);
    previous = yes;

    delay(10);


}
