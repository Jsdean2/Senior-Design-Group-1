#include "globals.hpp"

#include "HX711.h"

#include <Arduino.h>
#include <ezButton.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


//Definition ADC_Testing
volatile int calibration = -7050.0;

const int DOUT = 18;
const int CLK = 4;

int j = 1;

// Initialize the Serial port, display, buttons, semaphores, and software 1 tasks
void setup(){
  Serial.begin(115200);
  Wire.begin();
  initializeDisplay();
  //initializeButtons();
  initializeSemaphores();
  initializeTasks();
  adcsetup();

  while(j = 1){
    adcloop();
  }

}

void loop() {
  delay(50);
}

/////ADC Testing///////////

//HX711 scale;

float readchamp;
float testing0;
int i = 0;
int Josh = 0;

Adafruit_HX711 hx711(DOUT, CLK);

void adcsetup() {

  initializeDisplay2();

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
    hx711.tareB(hx711.readChannelRaw(CHAN_B_GAIN_32));
    hx711.tareB(hx711.readChannelRaw(CHAN_B_GAIN_32));
  } 
  
}

void adcloop(){

  initializeDisplay3();
  // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
  // since the read is blocking this will not be more than 10 or 80 SPS (L or H switch)
  int32_t weightA128 = hx711.readChannelBlocking(CHAN_A_GAIN_128);
  Serial.print("Channel A (Gain 128): ");
  Serial.println(weightA128);

  // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
  int32_t weightB32 = hx711.readChannelBlocking(CHAN_B_GAIN_32);
  Serial.print("Channel B (Gain 32): ");
  Serial.println(weightB32);
  delay(2000);

}


void initializeDisplay2(){
    // The display on the board is connected to port 2 on the I2C mux
    tcaselect(2);

    // Initialize display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
        for (;;);
    }
    delay(2000);

    // Clear the display and output the first software - speed / off button
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);

    display.clearDisplay();
    display.setCursor(0, 10);
    display.printf(" testing");
    display.display();

    display.display();
}

void initializeDisplay3(){
    // The display on the board is connected to port 2 on the I2C mux
    tcaselect(2);

    // Initialize display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
        for (;;);
    }
    delay(2000);

    // Clear the display and output the first software - speed / off button
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);

    display.clearDisplay();
    display.setCursor(0, 10);
    display.printf("%2.9f Reading Josh: %d", readchamp, Josh);
    display.display();

    display.display();
}


void initializeDisplay4(){
    // The display on the board is connected to port 2 on the I2C mux
    tcaselect(2);

    // Initialize display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
        for (;;);
    }
    delay(2000);

    // Clear the display and output the first software - speed / off button
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);

    display.clearDisplay();
    display.setCursor(0, 10);
    display.printf(" ADC HX711 Test %d", calibration);
    display.display();

    display.display();
}