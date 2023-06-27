#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "app_tasks.h"

QueueHandle_t sensor_queue;
QueueHandle_t lamp_queue;
QueueHandle_t time_queue;

void app_main(void)
{
    sensor_queue = xQueueCreate(1, sizeof(sensor_queue_data_t));
    lamp_queue = xQueueCreate(1, sizeof(lamp_queue_data_t));
    time_queue = xQueueCreate(1, sizeof(time_t));

    TaskHandle_t mqtt_task_handle;
    TaskHandle_t sensor_task_handle;
    TaskHandle_t display_task_handle;

    xTaskCreate(mqtt_task, "mqtt_task", 10000, NULL, 1, &mqtt_task_handle);
    xTaskCreate(sensor_task, "sensor_task", 10000, NULL, 1, &sensor_task_handle);
    xTaskCreate(display_task, "display_task", 10000, NULL, 1, &display_task_handle);
}