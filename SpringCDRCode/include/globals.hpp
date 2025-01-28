#include <Arduino.h>
#include <ezButton.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// The two DACs that are used to output to the motors controllers
#define DAC1 25                                                                                            
#define DAC2 26

#define needed 1
#define not_needed 0

// The speeds that are used for the electric assist modes
#define off 0
#define flat_surface 100
#define ramp 135

#define off 0
#define on 1

#define debouncing_time 100

// The different pins that are used on the ESP-32 for reading and outputting
#define kill_switch_pin 23
#define software_button_pin 32
#define off_button_pin 14
#define speed_button_pin 13
#define slow_button_pin 13
#define fast_button_pin 27
#define off_toggle 27
#define ramp_toggle 13
#define on_off_button_pin 27
#define slider_pin 35
#define yellow_led_pin 14

#define activated 1
#define off 0

// Buttons
extern ezButton software_button;
extern ezButton off_button;
extern ezButton speed_button;
extern ezButton fast_button;
extern ezButton slow_button;
extern ezButton on_off_button;

// Semaphores
extern SemaphoreHandle_t Software_Button_Sem;
extern SemaphoreHandle_t Off_Button_Sem;
extern SemaphoreHandle_t Speed_Button_Sem;
extern SemaphoreHandle_t Fast_Button_Sem;
extern SemaphoreHandle_t Slow_Button_Sem;
extern SemaphoreHandle_t Toggle_Switch_Sem;
extern SemaphoreHandle_t Update_Display_Sem;

// Task Handles
extern TaskHandle_t Read_Kill_Switch_Handle;
extern TaskHandle_t Read_Software_Button_Handle;
extern TaskHandle_t Read_Off_Button_Handle;
extern TaskHandle_t Read_Speed_Button_Handle;
extern TaskHandle_t Read_Fast_Button_Handle;
extern TaskHandle_t Read_Slow_Button_Handle;
extern TaskHandle_t Read_Toggle_Switch_Handle;
extern TaskHandle_t Read_Slider_Handle;

extern TaskHandle_t Software_Button_Task_Handle;
extern TaskHandle_t Speed_Button_Task_Handle;
extern TaskHandle_t Off_Button_Task_Handle;
extern TaskHandle_t Fast_Button_Task_Handle;
extern TaskHandle_t Slow_Button_Task_Handle;
extern TaskHandle_t Toggle_Switch_Task_Handle;
extern TaskHandle_t Slider_Task_Handle;
extern TaskHandle_t Update_Display_Handle;

extern volatile int speed_state;
extern volatile int prev_toggle_state;
extern volatile int current_toggle_state;
extern volatile bool system_reset;
extern volatile bool on_off_button_state;
extern volatile bool kill_switch_state;

extern Adafruit_SSD1306 display;
#define TCAADDR 0x70

void Read_Kill_Switch(void *pvParameters);
void Read_Software_Button(void *pvParameters);
void Read_Off_Button(void *pvParameters);
void Read_Speed_Button(void *pvParameters);
void Read_Slow_Button(void *pvParameters);
void Read_Fast_Button(void *pvParameters);
void Read_Toggle_Switch(void *pvParameters);

void Update_Display_Task(void *pvParameters);
void Software_Button_Task(void *pvParameters);
void Off_Button_Task(void *pvParameters);
void Speed_Button_Task(void *pvParameters);
void Slow_Button_Task(void *pvParameters);
void Fast_Button_Task(void *pvParameters);
void Toggle_Switch_Task(void *pvParameters);
void Slider_Task(void *pvParameters);

void initializeDisplay();
void initializeButtons();
void initializeSemaphores();
void initializeTasks();
void outputMotors(int value);
void rampMotors(int start, int end);
void tcaselect(uint8_t i);

void Create_Speed_Off_Button_Tasks();
void Delete_Speed_Off_Button_Tasks();

void Create_Parallel_Off_Button_Tasks();
void Delete_Parallel_Off_Button_Tasks();

void Create_Perpendicular_Off_Button_Tasks();
void Delete_Perpendicular_Off_Button_Tasks();

void Create_Toggle_Switch_Tasks();
void Delete_Toggle_Switch_Tasks();

void Create_Slider_Tasks();
void Delete_Slider_Tasks();

float floatMap(float x, float in_min, float in_max, float out_min, float out_max);

//ADC Testing
void adc_setup();
void adc_loop();