/**
 * @file lamp_task.c
 * 
 * @brief Implementação da tarefa de controle das lâmpadas
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "app_tasks.h"


#define LAMP_1 GPIO_NUM_32                                   /**< Pino GPIO para a lâmpada 1 */
#define LAMP_2 GPIO_NUM_33                                   /**< Pino GPIO para a lâmpada 2 */
#define LAMP_3 GPIO_NUM_26                                   /**< Pino GPIO para a lâmpada 3 */
#define LAMP_4 GPIO_NUM_27                                   /**< Pino GPIO para a lâmpada 4 */
#define ESP_INTR_FLAG_DEFAULT 0                              /**< Sinalizador de interrupção padrão do ESP */

extern QueueHandle_t lamp_queue;                             /**< Fila para comunicação com a tarefa principal */
int lamp[] = {LAMP_1,LAMP_2,LAMP_3,LAMP_4};                  /**< Array com os pinos GPIO das lâmpadas */

/**
 * @brief lamp_task
 *
 * A tarefa lamp_task é responsável pelo controle das lâmpadas com base nos dados recebidos pela fila lamp_queue.
 * Ela configura a direção dos pinos GPIO correspondente às lampadas e inicializa o estado com todas apagadas.
 * No laço  infinito ela obtem os dados mais recentes da fila ´lamp_queue´ por meio de iterações que vão atualizando
 * estado.
 * Se o estado for 'o': liga a lâmpada 
 * Caso contrário: desliga a lâmpada
 * 
 * Essa tarefa é realizada periodicamente a cada 100ms para verificar atualizações
 *
 */

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

