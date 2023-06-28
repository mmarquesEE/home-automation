/**
 * @file mqtt_task.c
 * 
 * @brief Arquivo Responsável por implementar as tarefas de comunicação com um servidor externo
 *  
 * Este arquivo contém a implementação da tarefa 'mqtt_task', responsável 
 * pela inicialização da conexão WiFi e configuração do cliente MQTT.
 * 
*/

#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_partition.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include <sys/param.h>
#include <time.h>
#include <sys/time.h>
#include "esp_sntp.h"

#include "wifi_conn.h"
#include "app_tasks.h"

const char * TAG = "mqtt_task";

extern const uint8_t mqtt_eclipseprojects_io_pem_start[]   asm("_binary_mqtt_eclipseprojects_io_pem_start");
extern const uint8_t mqtt_eclipseprojects_io_pem_end[]   asm("_binary_mqtt_eclipseprojects_io_pem_end");


extern QueueHandle_t sensor_queue;  /**< Fila para comunicação com a tarefa do sensor */
extern QueueHandle_t lamp_queue;    /**< Fila para comunicação com a tarefa da lâmpada */
extern QueueHandle_t time_queue;    /**< Fila para comunicação com a tarefa de tempo */

esp_mqtt_client_handle_t client;

/**
 * @brief publisher_task
 *
 *  A tarefa publisher_task é responsável por coletar os dados das filas de sensores e lâmpadas usando
 * `xQueuePeek´ e publica esses dados via MQTT usando `esp_mqtt_client_publish`
 * e também envia a hora atual para a fila de tempo.
 * 
 */

static void publisher_task(void *ignore)
{
    sensor_queue_data_t sensor_queue_data;
    lamp_queue_data_t lamp_queue_data;
    
    esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    setenv("TZ", "UTC+3", 1);
    tzset();

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
        xTaskDelayUntil(&xLastWakeTime, 5000/portTICK_PERIOD_MS);
        
        xQueuePeek(sensor_queue, (void *) &sensor_queue_data, 0);
        xQueuePeek(lamp_queue, (void *) &lamp_queue_data, 0);

        time(&now);

        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", &timeinfo);

        xQueueOverwrite(time_queue, &now);

        char datastr[255];
        sprintf(datastr,
            "{\"temp\":\"%.1f\",\"humi\":\"%.1f\",\"lamp\":\"%s\",\"time\":\"%s\"}",
            sensor_queue_data.temp, sensor_queue_data.humi, lamp_queue_data.lamp_state, strftime_buf);
        esp_mqtt_client_publish(client, "telemetry", datastr, 0, 2, 0);
        vTaskDelay(100/portTICK_PERIOD_MS);

    }
}

/**
 * @brief mqtt_event_handler
 *
 * O manipulador de eventos MQTT é chamado sempre que um evento MQTT é disparado. 
 * Neste caso, ele lida com o evento de recebimento de dados MQTT e atualiza o 
 * estado da lâmpada na fila da tarefa da lâmpada.
 *
 * @param[in] handler_args Argumentos do manipulador (não utilizados neste caso)
 * @param[in] base A base do evento
 * @param[in] event_id O ID do evento
 * @param[in] event_data Os dados do evento
 */

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

            ESP_LOGI("SUB","SUB");

            lamp_queue_data_t lamp_queue_data;
            sprintf(lamp_queue_data.lamp_state, data);
            xQueueOverwrite(lamp_queue, &lamp_queue_data);
            break;
        default:
            break;
    }
}

/**
 * @brief cb_connection_ok
 *
 * A função callback quando a conexão WiFi é estabelecida com sucesso. 
 * Ela configura o cliente MQTT com as informações necessárias como: 
 *  1: endereço do broker
 *  2: credenciais de autenticação 
 *  3: certificado de verificação
 * Em seguida registra a manipulação dos eventos MQTT, recebendo os dados. 
 * Por fim, inicia o cliente MQTT e função `esp_mqtt_client_subscribe` 
 * é chamada para se inscrever no tópico "lamp"
 * 
 * 
 */


static void cb_connection_ok(void){
	esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = "mqtts://b08cfa9706f74c31a53bf2a549d8c1f3.s1.eu.hivemq.cloud:8883",
        .credentials.username = "str20222",
        .credentials.authentication.password = "Str20222",
        .broker.verification.certificate = (const char *)mqtt_eclipseprojects_io_pem_start,
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    vTaskDelay(1000);

    esp_mqtt_client_subscribe(client, "lamp", 2);

    TaskHandle_t publisher_task_handle;
    xTaskCreate(publisher_task, "publisher_task", 10000, NULL, 3, &publisher_task_handle);
}

/**
 * @brief mqtt_task
 *
 * A tarefa mqtt_task é responsável pela inicialização da conexão WiFi e  conexão WiFi e configuração do cliente MQTT
 * A inicialização é feita por meio da função `wifi_init_sta` e (se estabelecida com sucesso)
 * retorna ´cb_connection_ok´
 *
 */

void mqtt_task(void *ignore)
{
    wifi_init_sta(cb_connection_ok);
    for(;;) vTaskDelay(100000/portTICK_PERIOD_MS);
}
