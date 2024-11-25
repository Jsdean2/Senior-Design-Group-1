#include "HX711.h"
#include <Arduino.h>
#include <iostream>


// Define the pins for the HX711 communication
const uint8_t DATA_PIN = 5;  // Can use any pins!
const uint8_t CLOCK_PIN = 18; // Can use any pins!
const uint8_t PWM_PIN = 9;
int32_t FORCE_DETECTED;
char STATE;



Adafruit_HX711 hx711(DATA_PIN, CLOCK_PIN);


///////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit HX711 Test!");

  // Initialize the HX711
  hx711.begin();

  // read and toss 3 values each
  Serial.println("Tareing....");
  for (uint8_t t=0; t<3; t++) {
    hx711.tareA(hx711.readChannelRaw(CHAN_A_GAIN_128));
    hx711.tareA(hx711.readChannelRaw(CHAN_A_GAIN_128));
    //hx711.tareB(hx711.readChannelRaw(CHAN_B_GAIN_32));
    //hx711.tareB(hx711.readChannelRaw(CHAN_B_GAIN_32));
  }
  
} 


int MAX = 900000;
int MIN = -900000;
float percentage;

void loop() {
  // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
  // since the read is blocking this will not be more than 10 or 80 SPS (L or H switch)
  float weightA128 = hx711.readChannelBlocking(CHAN_A_GAIN_128);

  if (weightA128 < 25000 && weightA128 > 0){
    STATE = 'N';
    percentage = 0;
  }

  if(weightA128 > 25000){
    STATE = 'R';
    percentage = ((weightA128 / MAX)*100);
  }

  if(weightA128 < 0 && weightA128 > -250000){
    STATE = 'N';
  }

  if(weightA128 < -25000){
    STATE = 'P';
    percentage = ((weightA128 / MIN)*100);
  }

  switch(STATE){
    case 'N':
      Serial.print("Noise Level\n");
      percentage = 0;
      break;
    case 'P':
      Serial.print("Push:\n");
      Serial.println(percentage);
      break;
    case 'R':
      Serial.print("Reverse:\n");
      Serial.println(percentage);
      break;
    default:
      break;
  }


  //Serial.print("Channel A (Gain 128): ");
  Serial.println(percentage);
  

  // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
  /*int32_t weightB32 = hx711.readChannelBlocking(CHAN_B_GAIN_32);
  Serial.print("Channel B (Gain 32): ");
  Serial.println(weightB32);*/
  delay(2000);
}

///////////////////////////////////////////////////////////////////////////////////////////////

////Green side is positive
////Red side is negativev
