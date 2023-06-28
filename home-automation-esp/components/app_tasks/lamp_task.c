#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "app_tasks.h"

#define LAMP_1 GPIO_NUM_32
#define LAMP_2 GPIO_NUM_33
#define LAMP_3 GPIO_NUM_26
#define LAMP_4 GPIO_NUM_27
#define ESP_INTR_FLAG_DEFAULT 0

extern QueueHandle_t lamp_queue;
int lamp[] = {LAMP_1,LAMP_2,LAMP_3,LAMP_4};

void lamp_task(void * ignore)
{
    gpio_set_direction(LAMP_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAMP_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAMP_3, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAMP_4, GPIO_MODE_OUTPUT);
    
    lamp_queue_data_t lamp_queue_data;
    sprintf(lamp_queue_data.lamp_state, "%c%c%c%c", 'f','f','f','f');
    xQueueOverwrite(lamp_queue, &lamp_queue_data);

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
        xTaskDelayUntil(&xLastWakeTime, 100/portTICK_PERIOD_MS);
    
        xQueuePeek(lamp_queue, &lamp_queue_data, 100);
        for(int i=0;i<4;i++){
            uint8_t lstate = (uint8_t) lamp_queue_data.lamp_state[i];
            if (lstate == 'o') 
                gpio_set_level(lamp[i],1);
            else
                gpio_set_level(lamp[i],0);
        }
    }
}
