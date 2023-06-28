#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "app_tasks.h"

#define BUTTON_1 GPIO_NUM_34
#define BUTTON_2 GPIO_NUM_35
#define BUTTON_3 GPIO_NUM_19
#define BUTTON_4 GPIO_NUM_18

int button[] = {BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4};
extern QueueHandle_t lamp_queue;

int state = 0;
QueueHandle_t interputQueue;

static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
    int pinNumber = (int)args;
    xQueueSendFromISR(interputQueue, &pinNumber, NULL);
}

void button_task(void * ignore)
{
    for(int i=0;i<4;i++){
        gpio_set_direction(button[i], GPIO_MODE_INPUT);
        gpio_pulldown_en(button[i]);
        gpio_pullup_dis(button[i]);
        gpio_set_intr_type(button[i], GPIO_INTR_POSEDGE);
    }
    

    interputQueue = xQueueCreate(10, sizeof(int));

    gpio_install_isr_service(0);
    for(int i=0;i<4;i++){
        gpio_isr_handler_add(button[i], gpio_interrupt_handler, (void *)button[i]);
    }

    lamp_queue_data_t lamp_queue_data;
    int pinNumber;
    for(;;){
        if (xQueueReceive(interputQueue, &pinNumber, portMAX_DELAY))
        {
            xQueuePeek(lamp_queue, &lamp_queue_data, 0);
        
            char lamp_state[5];
            sprintf(lamp_state, lamp_queue_data.lamp_state);

            switch(pinNumber) {
                case BUTTON_1:
                    lamp_queue_data.lamp_state[0] = lamp_state[0]=='f'?'o':'f';
                    break;
                case BUTTON_2:
                    lamp_queue_data.lamp_state[1] = lamp_state[1]=='f'?'o':'f';
                    break;
                case BUTTON_3:
                    lamp_queue_data.lamp_state[2] = lamp_state[2]=='f'?'o':'f';
                    break;
                case BUTTON_4:
                    lamp_queue_data.lamp_state[3] = lamp_state[3]=='f'?'o':'f';
                    break;
                default:
                    break;
            }
            xQueueOverwrite(lamp_queue, &lamp_queue_data);
        }
    }
}