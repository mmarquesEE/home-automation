/**
 * @file sensor_task.c
 * 
 * @brief Arquivo Responsável por implementar as tarefas do sensor
 * 
 * Este arquivo contém a implementação da tarefa 'sensor_task', responsável 
 * pela leitura dos dados do sensor DHT11 e o envio dos dados para a tarefa
 * principal através da fila.
 * 
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "app_tasks.h"

#include "dht_espidf.h"

#define DHT_IO GPIO_NUM_21

extern QueueHandle_t sensor_queue;  /**< Fila para comunicação com a tarefa principal */


/**
 * @brief sensor_task
 *
 * A tarefa sensor_task é responsável pela leitura dos dados do sensor DHT11 
 * e o envio dos dados para a tarefa principal através de uma fila de comunicação
 * 
 * A leitura dos dados é realizada a cada 1 segundo por meio da função `read_dht_sensor_data`
 * Caso a leitura não seja bem-sucedida: há um retorno diferente de `DHT_OK` e  é feito um atraso de 10ms antes de uma segunda tentativa 
 * Caso a leitura seja bem-sucedida: armazena valores de temperatura e umidade em uma estrutura `sensor_queue_data_t`
 * e esses vaalores são enviados para a fila `sensor_queue` utilizando a função `xQueueOverwrite`
 * 
 */

void sensor_task(void *ignore)
{

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
        xTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS);
        dht_reading_t dht_data;

        dht_result_t res = read_dht_sensor_data(DHT_IO, DHT11, &dht_data);

        if (res != DHT_OK) {
            vTaskDelay(10/portTICK_PERIOD_MS);
        } else {
            double temp = dht_data.temperature; /**< Temperatura medida pelo sensor DHT11 */
            double humi = dht_data.humidity; /**< Umidade medida pelo sensor DHT11 */

            sensor_queue_data_t sensor_queue_data;
            sensor_queue_data.temp = temp;
            sensor_queue_data.humi = humi;

            xQueueOverwrite(sensor_queue, &sensor_queue_data);
        }
    }
}

