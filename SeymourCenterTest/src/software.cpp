#include "globals.hpp"

volatile int kill_count = 0;
volatile int software_state = 0;
volatile int display_state = 0;
volatile bool kill_switch_state = off;

// This task reads the red kill switch that is on the wheelchair
// When the kill switch has been activated, it will immediately kill all output
// to the motors and reset the speed states and reset states to needed so that
// the motors cannot be activated until the kill switch and UIs have been reset
// to their off states
void Read_Kill_Switch(void *pvParameters){
    for(;;){
        // Read the kill switch
        volatile int kill_switch = digitalRead(kill_switch_pin);

        // If the kill switch reads 0, it has been activated
        if (kill_switch == 0){
            kill_count++;
        }
        // The kill switch has not been activated
        else{
            kill_count = 0;
            kill_switch_state = off;
        }

        // If the kill switch reads 10 consecutive values of activated, it will kill
        // the output to the motors and reset all of the speed states
        if (kill_count >= 10){
            kill_switch_state = activated;
            outputMotors(off);
            speed_state = off;
            on_off_button_state = off;
            system_reset = needed;
            Serial.println("Kill Motors");
        }
        delay(10);
    }
}

// This task reads the software button that is on the board. This button allows you to
// select the UI that is desired.
void Read_Software_Button(void *pvParameters){
    for (;;){
        software_button.loop();
        if (software_button.isPressed()){
            Serial.println("Software Button Pressed");
            xSemaphoreGive(Software_Button_Sem);
        }
        delay(10);
    }
}

// This task is a state machine that iterates through the different UI's that have been 
// added. It creates the current UI tasks as well as deletes the previous UI's tasks. It also
// gives the sempahore to the display task so that the display is updated
void Software_Button_Task(void *pvParameters){
    for (;;){
        xSemaphoreTake(Software_Button_Sem, portMAX_DELAY);
        switch (software_state){
            // Delete the speed single button / off button UI tasks
            // Create the parallel two button / off button UI tasks
            // Update display
            case 0:
                Delete_Speed_Off_Button_Tasks();
                Create_Parallel_Off_Button_Tasks();
                speed_state = off;
                software_state = 1;
                xSemaphoreGive(Update_Display_Sem);
                break;

            // Delete the paralle two button / off button UI tasks
            // Create the perependicular two button / off button UI tasks
            // Update display
            case 1:
                Delete_Parallel_Off_Button_Tasks();
                Create_Perpendicular_Off_Button_Tasks();
                outputMotors(0);
                speed_state = off;
                software_state = 2;
                xSemaphoreGive(Update_Display_Sem);
                break;

            // Delete the perpendicular two button / off button UI tasks
            // Create the toggle switch UI tasks
            // Update display
            case 2:
                Delete_Perpendicular_Off_Button_Tasks();
                Create_Toggle_Switch_Tasks();
                outputMotors(0);
                speed_state = off;
                system_reset = needed;
                software_state = 3;
                Serial.println("Toggle");
                xSemaphoreGive(Update_Display_Sem);
                break;

            // Delete the toggle switch UI tasks
            // Create the slider UI tasks
            // Update display
            case 3:
                Delete_Toggle_Switch_Tasks();
                Create_Slider_Tasks();
                outputMotors(0);
                speed_state = off;
                on_off_button_state = off;
                system_reset = needed;
                software_state = 4;
                Serial.println("Slider");
                xSemaphoreGive(Update_Display_Sem);
                break;

            // Delete the speed single button / off button UI tasks
            // Create the parallel two button / off button UI tasks
            // Update display
            case 4:
                Delete_Slider_Tasks();
                Create_Speed_Off_Button_Tasks();
                speed_state = off;
                software_state = 0;
                xSemaphoreGive(Update_Display_Sem);
                break;

            // The default software state is 0
            default:
                software_state = 0;
                speed_state = off;
                break;
        }
    }
}

// This task is responsible for updating the display. Similar to the software button
// task, it also is a state machine that works in tandem with the software button task.
// It updates the display to reflect the UI that has been selected. The weird spacing in
// some of the outputs is due to the way that the display can fit words. We wanted the word
// to look pretty on the screen
void Update_Display_Task(void *pvParameters){
    for (;;){
        xSemaphoreTake(Update_Display_Sem, portMAX_DELAY);
        tcaselect(2);
        switch(display_state){
            case 0:
                display.clearDisplay();
                display.setCursor(0, 10);
                display.printf("Fast/Slow/Off Button Parallel");
                display.display();
                display_state = 1;
                break;

            case 1:
                display.clearDisplay();
                display.setCursor(0, 10);
                display.printf("Fast/Slow/Off ButtonHorizontal");
                display.display();
                display_state = 2;
                break;

            case 2:
                display.clearDisplay();
                display.setCursor(0, 10);
                display.printf("Toggle    Switch");
                display.display();
                display_state = 3;
                break;

            case 3:
                display.clearDisplay();
                display.setCursor(0, 10);
                display.printf("Slider");
                display.display();
                display_state = 4;
                break;

            case 4:
                display.clearDisplay();
                display.setCursor(0, 10);
                display.printf("Speed/Off Button UI");
                display.display();
                display_state = 0;
                break;

            default:
                display.clearDisplay();
                display.setCursor(0, 10);
                display.printf("Speed/Off Button UI");
                display.display();
                display_state = 0;
                break;
        }
    }
}