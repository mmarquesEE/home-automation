#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <time.h>

typedef struct
{
    double temp;
    double humi;
} sensor_queue_data_t;

typedef struct
{
    char lamp_state[5];
} lamp_queue_data_t;

void mqtt_task(void * ignore);
void sensor_task(void * ignore);
void display_task(void * ignore);
void lamp_task(void * ignore);