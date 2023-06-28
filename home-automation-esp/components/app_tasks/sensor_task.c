#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "app_tasks.h"

#include "dht_espidf.h"

#define DHT_IO GPIO_NUM_21

extern QueueHandle_t sensor_queue;

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
            double temp = dht_data.temperature;
            double humi = dht_data.humidity;
            sensor_queue_data_t sensor_queue_data;
            sensor_queue_data.temp = temp;
            sensor_queue_data.humi = humi;

            xQueueOverwrite(sensor_queue, &sensor_queue_data);
        }
    }
}
