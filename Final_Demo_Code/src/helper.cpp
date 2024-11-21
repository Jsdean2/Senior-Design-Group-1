#include "globals.hpp"

// This function simplifies the outputting of a DAC value to the motors
void outputMotors(int value){
    dacWrite(DAC1, value);
    dacWrite(DAC2, value);
}

// This function simplifies the ramping of motors from the intial DAC value
// to the final desired DAC value
void rampMotors(int start, int end){
    for (int i = start; i < end; i += 2){
        outputMotors(i);
        delay(10);
    }
}

// This function selects the port on the I2C mux that is being used
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// This function creates the tasks necessary for the first UI - speed single button / off button UI
void Create_Speed_Off_Button_Tasks(){
    xTaskCreate(Read_Off_Button, "Read Off Button", 4000, NULL, 5, &Read_Off_Button_Handle);
    xTaskCreate(Read_Speed_Button, "Read Speed Button", 4000, NULL, 6, &Read_Speed_Button_Handle);

    xTaskCreate(Off_Button_Task, "Off Button Task", 4000, NULL, 7, &Off_Button_Task_Handle);
    xTaskCreate(Speed_Button_Task, "Speed Button Task", 4000, NULL, 8, &Speed_Button_Task_Handle);
}

// This function deletes the tasks necessary for the first UI - speed single button / off button UI
void Delete_Speed_Off_Button_Tasks(){
    vTaskDelete(Read_Off_Button_Handle);
    vTaskDelete(Read_Speed_Button_Handle);

    vTaskDelete(Off_Button_Task_Handle);
    vTaskDelete(Speed_Button_Task_Handle);
}

// This function creates the tasks necessary for the second UI - parallel two button/ off button UI
void Create_Parallel_Off_Button_Tasks(){
    xTaskCreate(Read_Off_Button, "Read Off Button", 4000, NULL, 5, &Read_Off_Button_Handle);
    xTaskCreate(Read_Slow_Button, "Read Slow Button", 4000, NULL, 6, &Read_Slow_Button_Handle);
    xTaskCreate(Read_Fast_Button, "Read Fast Button", 4000, NULL, 7, &Read_Fast_Button_Handle);

    xTaskCreate(Off_Button_Task, "Off Button Task", 4000, NULL, 8, &Off_Button_Task_Handle);
    xTaskCreate(Slow_Button_Task, "Slow Button Task", 4000, NULL, 9, &Slow_Button_Task_Handle);
    xTaskCreate(Fast_Button_Task, "Fast Button Task", 4000, NULL, 10, &Fast_Button_Task_Handle);
}

// This function deletes the tasks necessary for the second UI - parallel two button / off button UI
void Delete_Parallel_Off_Button_Tasks(){
    vTaskDelete(Read_Off_Button_Handle);
    vTaskDelete(Read_Slow_Button_Handle);
    vTaskDelete(Read_Fast_Button_Handle);

    vTaskDelete(Off_Button_Task_Handle);
    vTaskDelete(Slow_Button_Task_Handle);
    vTaskDelete(Fast_Button_Task_Handle);
}

// This function creates the tasks necessary for the third UI - perpendicular two button / off button UI
void Create_Perpendicular_Off_Button_Tasks(){
    xTaskCreate(Read_Off_Button, "Read Off Button", 4000, NULL, 5, &Read_Off_Button_Handle);
    xTaskCreate(Read_Slow_Button, "Read Slow Button", 4000, NULL, 6, &Read_Slow_Button_Handle);
    xTaskCreate(Read_Fast_Button, "Read Fast Button", 4000, NULL, 7, &Read_Fast_Button_Handle);

    xTaskCreate(Off_Button_Task, "Off Button Task", 4000, NULL, 8, &Off_Button_Task_Handle);
    xTaskCreate(Slow_Button_Task, "Slow Button Task", 4000, NULL, 9, &Slow_Button_Task_Handle);
    xTaskCreate(Fast_Button_Task, "Fast Button Task", 4000, NULL, 10, &Fast_Button_Task_Handle);
}

// This function deletes the tasks necessary for the third UI - perpendicular two button / off button UI
void Delete_Perpendicular_Off_Button_Tasks(){
    vTaskDelete(Read_Off_Button_Handle);
    vTaskDelete(Read_Slow_Button_Handle);
    vTaskDelete(Read_Fast_Button_Handle);

    vTaskDelete(Off_Button_Task_Handle);
    vTaskDelete(Slow_Button_Task_Handle);
    vTaskDelete(Fast_Button_Task_Handle);
}

// This function creates the tasks necessary for the fourth UI - the toggle switch UI
void Create_Toggle_Switch_Tasks(){
    xTaskCreate(Read_Toggle_Switch, "Read Toggle Switch", 4000, NULL, 5, &Read_Toggle_Switch_Handle);
    xTaskCreate(Toggle_Switch_Task, "Toggle Switch Task", 4000, NULL, 6, &Toggle_Switch_Task_Handle);

    pinMode(off_toggle, INPUT_PULLUP);
    pinMode(ramp_toggle, INPUT_PULLUP);
    pinMode(yellow_led_pin, OUTPUT);
}

// This function deletes the tasks necessary for the fourth UI - the toggle switch UI
void Delete_Toggle_Switch_Tasks(){
    vTaskDelete(Read_Toggle_Switch_Handle);
    vTaskDelete(Toggle_Switch_Task_Handle);
}

// This function creates the tasks necessary for the fifth UI - the slider UI
void Create_Slider_Tasks(){
    xTaskCreate(Slider_Task, "Slider Task", 4000, NULL, 5, &Slider_Task_Handle);
    pinMode(off_button_pin, INPUT_PULLUP);
    pinMode(slider_pin, INPUT);
}

// This function deletes the tasks necessary for the fifth UI - the slider UI
void Delete_Slider_Tasks(){
    vTaskDelete(Slider_Task_Handle);
}

// This function normalizes the values
// The x value is the input
// The in_min and in_max are the minimum and maximum possible input values
// The out_min and out_max are the minimum and maximum desired output values
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

