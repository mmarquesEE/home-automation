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
#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */

extern QueueHandle_t sensor_queue;
extern QueueHandle_t time_queue;

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
