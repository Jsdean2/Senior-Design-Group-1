#include "globals.hpp"

volatile int speed_state = 0;
volatile int current_toggle_state = 0;
volatile bool system_reset = false;
volatile bool on_off_button_state = off;
volatile int led_state = LOW;

// This task reads the off button and releases the semaphore for the off button task
void Read_Off_Button(void *pvParameters){
    for (;;){
        off_button.loop();
        if (off_button.isPressed()){
            Serial.println("Off Button Pressed");
            xSemaphoreGive(Off_Button_Sem);
        }
        delay(10);
    }
}

// This task turns off the motors
void Off_Button_Task(void *pvParamters){
    for (;;){
        xSemaphoreTake(Off_Button_Sem, portMAX_DELAY);
        speed_state = off;
        system_reset = not_needed;
        outputMotors(off);
    }
}

// This tasks reads the speed button for the first UI
void Read_Speed_Button(void *pvParameters){
    for (;;){
        speed_button.loop();
        if (speed_button.isPressed()){
            Serial.println("Speed Button Pressed");
            xSemaphoreGive(Speed_Button_Sem);
        }
        delay(10);
    }
}

// This task is the state machine for the first UI and ramps the speed from
// off, flat surface, and ramp speed
void Speed_Button_Task(void *pvParameters){
    for (;;){
        xSemaphoreTake(Speed_Button_Sem, portMAX_DELAY);
        if (speed_state == off){
            speed_state = flat_surface;
            rampMotors(off, flat_surface);
        }
        else if (speed_state == flat_surface){
            speed_state = ramp;
            rampMotors(flat_surface, ramp);
        }
        else if (speed_state == ramp){
            speed_state = off;
            outputMotors(off);
        }
    }
}

// This task reads the slow button for the perpendicular and parallel UI
void Read_Slow_Button(void *pvParameters){
    for (;;){
        slow_button.loop();
        if (slow_button.isPressed()){
            Serial.println("Slow Button Pressed");
            xSemaphoreGive(Slow_Button_Sem);
        }
        delay(10);
    }
}

// This task sets the speed to slow if needed
void Slow_Button_Task(void *pvParameters){
    for (;;){
        xSemaphoreTake(Slow_Button_Sem, portMAX_DELAY);
        if (speed_state == off){
            speed_state = flat_surface;
            rampMotors(off, flat_surface);
        }
        else if (speed_state == ramp){
            speed_state = flat_surface;
            outputMotors(flat_surface);
        }
    }
}

// This task reads the fast button for the perpendicular and parallel UI
void Read_Fast_Button(void *pvParameters){
    for (;;){
        fast_button.loop();
        if (fast_button.isPressed()){
            Serial.println("Fast Button Pressed");
            xSemaphoreGive(Fast_Button_Sem);
        }
        delay(10);
    }
}

// This task sets the speed to fast if needed 
void Fast_Button_Task(void *pvParameters){
    for (;;){
        xSemaphoreTake(Fast_Button_Sem, portMAX_DELAY);
        if (speed_state == off){
            speed_state = ramp;
            rampMotors(off, ramp);
        }
        else if (speed_state == flat_surface){
            speed_state = ramp;
            rampMotors(flat_surface, ramp);
        }
    }
}

// This task reads the toggle switch UI
// Based on how the three pin toggle hardware works, the task reads
// the off pin and toggle pin on the toggle since the middle pin cannot
// be read. If neither of these are selected, that means that it is in
// flat surface mode. In order for the toggle to work when first activated by
// the software and plugged in, it must be reset to off before it is used. It must
// also be reset to off before being used as a safety precaution. 
void Read_Toggle_Switch(void *pvParameters){
    for (;;){

        // Read the off and ramp toggle switch pins
        int off_toggle_read = digitalRead(off_toggle);
        int ramp_toggle_read = digitalRead(ramp_toggle);

        // The middle LED on the toggle switch is controlled by the MCU
        digitalWrite(yellow_led_pin, led_state);    

        // Off 
        if ((off_toggle_read == HIGH) && (ramp_toggle_read == LOW)){
            current_toggle_state = off;
            system_reset = not_needed;
            led_state = LOW;

            // Only change the speed state if it is not repeated
            if (current_toggle_state != speed_state){
                speed_state = off;
                Serial.println("Off Toggle");

                // Only activate the assist if the kill switch is not activated
                if (kill_switch_state == off){
                    xSemaphoreGive(Toggle_Switch_Sem);
                }
            }
        }

        // Ramp Speed
        else if ((ramp_toggle_read == HIGH) && (off_toggle_read == LOW)){
            current_toggle_state = ramp;
            led_state = LOW;

            // Only change the speed state if it is not repeated
            if ((current_toggle_state != speed_state) && (system_reset == not_needed)){
                speed_state = ramp;
                Serial.println("Ramp Toggle");

                // Only activate the assist if the kill switch is not activated
                if (kill_switch_state == off){
                    xSemaphoreGive(Toggle_Switch_Sem);
                }    
            }
        }

        // Flat Surface
        else {
            current_toggle_state = flat_surface;
            led_state = HIGH;

            // Only change the speed state if it is not repeated
            if ((current_toggle_state != speed_state) && (system_reset == not_needed)){
                speed_state = flat_surface;
                Serial.println("Flat Surface Toggle");

                // Only activate the assist if the kill switch is not activated
                if (kill_switch_state == off){
                    xSemaphoreGive(Toggle_Switch_Sem);
                }
            }
        }

        delay(20);
    }
}

// This task handles the motor output based on the speed state set in the read toggle task
void Toggle_Switch_Task(void *pvParameters){
    for (;;){
        xSemaphoreTake(Toggle_Switch_Sem, portMAX_DELAY);
        if (speed_state == off){
            outputMotors(off);
        }
        else if (speed_state == flat_surface){
            outputMotors(flat_surface);
        }
        else if (speed_state == ramp){
            outputMotors(ramp);
        }
    }
}

// This task both reads the slider and outputs to the motor based on the speed state and slider position
void Slider_Task(void *pvParameters){
    for (;;){
        on_off_button.loop();
        
        // The slider has an on / off button on the UI
        // It determines if the slider is going to activate the electric assist or not
        if (on_off_button.isPressed()){
            on_off_button_state = !on_off_button_state;

            // The slider must be reset to the off position before the electric is able to be activated
            if (on_off_button_state == on){
                system_reset = needed;
                Serial.println("System Reset needed");
            }
            else{
                outputMotors(0);
            }
        }

        // Read the slider with the ADC
        volatile int sensor_value = analogRead(slider_pin);

        // If the slider is in the off position and a system reset is needed, it can unactivate the need for
        // a reset
        if ((system_reset == needed) && (sensor_value == 0)){
            system_reset = not_needed;
            Serial.println("System Reset not needed");
        }

        // The slider has been reset, the on/off button is in the on state, and the kill switch is not activated
        if ((on_off_button_state == on) && (system_reset == not_needed) && (kill_switch_state == off)){

            // Normalize the values between the desired DAC values
            float normalized_output = floatMap(sensor_value, 0.0, 4095.0, 60, 155);
            outputMotors(normalized_output);
            Serial.println("Output to motors");
        }
        delay(5);
    }
}