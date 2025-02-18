#include "globals.hpp"

ezButton software_button(software_button_pin);
ezButton off_button(off_button_pin);
ezButton speed_button(speed_button_pin);
ezButton slow_button(slow_button_pin);
ezButton fast_button(fast_button_pin);
ezButton on_off_button(on_off_button_pin);

// Initialize Semaphore Handles
SemaphoreHandle_t Software_Button_Sem;
SemaphoreHandle_t Off_Button_Sem;
SemaphoreHandle_t Speed_Button_Sem;
SemaphoreHandle_t Fast_Button_Sem;
SemaphoreHandle_t Slow_Button_Sem;
SemaphoreHandle_t Toggle_Switch_Sem;
SemaphoreHandle_t Update_Display_Sem;
///Spring Addition
SemaphoreHandle_t PushingChair;


// Initialize Task Handles
TaskHandle_t Read_Software_Button_Handle;
TaskHandle_t Read_Off_Button_Handle;
TaskHandle_t Read_Speed_Button_Handle;
TaskHandle_t Read_Fast_Button_Handle;
TaskHandle_t Read_Slow_Button_Handle;
TaskHandle_t Read_Kill_Switch_Handle;
TaskHandle_t Read_Toggle_Switch_Handle;
TaskHandle_t Read_Slider_Handle;

TaskHandle_t Software_Button_Task_Handle;
TaskHandle_t Speed_Button_Task_Handle;
TaskHandle_t Off_Button_Task_Handle;
TaskHandle_t Fast_Button_Task_Handle;
TaskHandle_t Slow_Button_Task_Handle;
TaskHandle_t Toggle_Switch_Task_Handle;
TaskHandle_t Slider_Task_Handle;
TaskHandle_t Update_Display_Handle;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initializeDisplay(){
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
    display.printf("Speed/Off Button UI");
    display.display();

    display.display();
}

void initializeButtons(){
    // Set the debounce time of all the buttons that are going to be used
    software_button.setDebounceTime(debouncing_time);
    off_button.setDebounceTime(debouncing_time);
    speed_button.setDebounceTime(debouncing_time);
    fast_button.setDebounceTime(debouncing_time);
    slow_button.setDebounceTime(debouncing_time);

    pinMode(kill_switch_pin, INPUT_PULLUP);
}

// Initialize all of the sempahores that are going to be used throughout the program
void initializeSemaphores(){
    Software_Button_Sem = xSemaphoreCreateBinary();
    Off_Button_Sem = xSemaphoreCreateBinary();
    Speed_Button_Sem = xSemaphoreCreateBinary();
    Fast_Button_Sem = xSemaphoreCreateBinary();
    Slow_Button_Sem = xSemaphoreCreateBinary();
    Toggle_Switch_Sem = xSemaphoreCreateBinary();
    Update_Display_Sem = xSemaphoreCreateBinary();
    PushingChair = xSemaphoreCreateBinary();

}

// Initialize the tasks for the the tasks that will be used throughout the entire program
// The kill switch, the read software button, the software button task handling, and the updating
// display handle
void initializeTasks(){
    xTaskCreate(Read_Kill_Switch, "Read Kill Switch", 4000, NULL, 1, &Read_Kill_Switch_Handle);
    xTaskCreate(Read_Software_Button, "Read Software Button", 4000, NULL, 2, &Read_Software_Button_Handle);
    xTaskCreate(Software_Button_Task, "Software Button Task", 4000, NULL, 3, &Software_Button_Task_Handle);
    xTaskCreate(Update_Display_Task, "Update Display Task", 4000, NULL, 4, &Update_Display_Handle);
    Create_Speed_Off_Button_Tasks();
}