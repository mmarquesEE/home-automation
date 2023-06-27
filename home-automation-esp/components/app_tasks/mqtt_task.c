#include <stdio.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "app_tasks.h"
#include "mqtt_client.h"
#include "esp_log.h"

#include "wifi_conn.h"

const char * TAG = "mqtt_task";

extern QueueHandle_t sensor_queue;
extern QueueHandle_t lamp_queue;
extern QueueHandle_t time_queue;

esp_mqtt_client_handle_t client;

static void publisher_task(void *ignore)
{
    sensor_queue_data_t sensor_queue_data;
    lamp_queue_data_t lamp_queue_data;
    
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
        xTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS);
        
        xQueuePeek(sensor_queue, (void *) &sensor_queue_data, 0);
        xQueuePeek(lamp_queue, (void *) &lamp_queue_data, 0);

        time(&now);
        setenv("TZ", "UTC-4", 1);
        tzset();

        xQueueOverwrite(time_queue, &now);

        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

        char datastr[255];
        sprintf(datastr,
            "{\"temp\":\"%.1f\",\"humi\":\"%.1f\",\"lamp\":\"%s\",\"time\":\"%s\"}",
            sensor_queue_data.temp, sensor_queue_data.humi, lamp_queue_data.lamp_state, strftime_buf);
        esp_mqtt_client_publish(client, "telemetry", datastr, 0, 2, 0);
        vTaskDelay(100/portTICK_PERIOD_MS);

        // ESP_LOGI(TAG, "Mensagem enviada");
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    char topic[5];
    char data[5];

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_DATA:
            sprintf(topic, "%.*s", event->topic_len, event->topic);
            sprintf(data, "%.*s", event->data_len, event->data);
    
            if(strcmp(topic,"lamp")){
                lamp_queue_data_t lamp_queue_data;
                sprintf(lamp_queue_data.lamp_state, 5, data);
                xQueueOverwrite(lamp_queue, &lamp_queue_data);
            }
            break;
        default:
            break;
    }
}

static void cb_connection_ok(void){
	esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = "mqtt://192.168.15.166",
        .credentials.username = "esp32",
        .credentials.authentication.password = "str20222",
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    vTaskDelay(1000);

    esp_mqtt_client_subscribe(client, "lamp", 1);

    TaskHandle_t publisher_task_handle;
    xTaskCreate(publisher_task, "publisher_task", 10000, NULL, 3, &publisher_task_handle);
}

void mqtt_task(void *ignore)
{
    wifi_init_sta(cb_connection_ok);
    for(;;) vTaskDelay(100000/portTICK_PERIOD_MS);
}
