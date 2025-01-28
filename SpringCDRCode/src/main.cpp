#include "globals.hpp"

#include <Arduino.h>
#include <ezButton.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>



#include "HX711.h"


const uint8_t DATA_PIN = 5;
const uint8_t CLOCK_PIN = 18; // Can use any pins!
const uint8_t PWM_PIN = 9;

int32_t FORCE_DETECTED;
char STATE;


// Initialize the Serial port, display, buttons, semaphores, and software 1 tasks
void setup(){
  Serial.begin(115200);
  Serial.println("Welcome!");
  Wire.begin();
  initializeDisplay();
  initializeButtons();
  initializeSemaphores();
  initializeTasks();
  adc_setup();

}

void loop() {
  delay(50);
}

Adafruit_HX711 hx711(DATA_PIN, CLOCK_PIN);

void adc_setup() {
 Serial.begin(115200);
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
  while(1){
    adc_loop();
  }
}

int MAX = 900000;
int MIN = -900000;
float percentage;

void adc_loop() {

  float weightA128 = hx711.readChannelBlocking(CHAN_A_GAIN_128);
  Serial.println("ADC LOOP");

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
      xSemaphoreGive(Slow_Button_Sem);
      break;
    case 'R':
      Serial.print("Reverse:\n");
      Serial.println(percentage);
      break;
    default:
      break;
  }
  Serial.println(percentage);
  

  // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
  /*int32_t weightB32 = hx711.readChannelBlocking(CHAN_B_GAIN_32);
  Serial.print("Channel B (Gain 32): ");
  Serial.println(weightB32);*/
  delay(2000);

}