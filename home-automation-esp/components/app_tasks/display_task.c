/**
 * @file display_task.c
 * 
 * @brief Implementação da tarefa de exibição de informações no display OLED
 */

#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "OLEDDisplay.h"

#include "app_tasks.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL                    /**< Número do pino GPIO para o clock do mestre I2C */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA                    /**< Número do pino GPIO para os dados do mestre I2C */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM)       /**< Número da porta I2C para o mestre */

extern QueueHandle_t sensor_queue;                                  /**< Fila para comunicação com a tarefa de leitura de sensores */
extern QueueHandle_t time_queue;                                    /**< Fila para comunicação com a tarefa de obtenção do tempo atual */


/**
 * @brief display_task
 *
 * Função responsável pela exibição de informações no display OLED.
 * Ela utiliza a biblioteca ´OLEDDisplay.h´ para controlar o display 
 * e as filas 'sensor_queue' e 'time_queue' 
 * Ela obtém os dados do sensor e o tempo atual das filas correspondentes 
 * e os exibe no display periodicamente a cada 100ms
 *
 */

void display_task(void *ignore)
{
    OLEDDisplay_t *oled = OLEDDisplay_init(I2C_MASTER_NUM,0x78,I2C_MASTER_SDA_IO,I2C_MASTER_SCL_IO);
    vTaskDelay(1000/portTICK_PERIOD_MS);

    sensor_queue_data_t sensor_queue_data;

    time_t now;
    char strftime_buf[6];
    struct tm timeinfo;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
        xTaskDelayUntil(&xLastWakeTime, 100/portTICK_PERIOD_MS);

        xQueuePeek(sensor_queue, &sensor_queue_data, 0);
        xQueuePeek(time_queue, &now, 0);

        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%H:%M", &timeinfo);

        char temp_text[10], humi_text[20];
        sprintf(temp_text,"%2.0f %cC", sensor_queue_data.temp, (char) 176);
        sprintf(humi_text,"Humid.: %2.0f %%", sensor_queue_data.humi);

        OLEDDisplay_clear(oled);

        OLEDDisplay_setTextAlignment(oled,TEXT_ALIGN_CENTER);
        OLEDDisplay_setFont(oled,ArialMT_Plain_24);
        OLEDDisplay_drawString(oled,64, 00, strftime_buf);
        OLEDDisplay_display(oled);

        OLEDDisplay_setFont(oled,ArialMT_Plain_16);
        OLEDDisplay_drawString(oled,64, 25, temp_text);
        OLEDDisplay_display(oled);

        OLEDDisplay_setFont(oled,ArialMT_Plain_10);
        OLEDDisplay_drawString(oled,64, 45, humi_text);
        OLEDDisplay_display(oled);
    }
}
