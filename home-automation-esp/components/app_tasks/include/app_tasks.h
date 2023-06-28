/**
 * @file app_tasks.h
 * 
 * @brief Declaração das estruturas de dados e tarefas do sistema
 */

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <time.h>


/*
    @brief Estrutura de dados para armazenar informações dos sensores

    A estrutura `sensor_queue_data_t` é utilizada para armazenar dados referentes aos sensores,
    como temperatura e umidade.

    @param temp Temperatura medida pelo sensor
    @param humi Umidade medida pelo sensor
*/
typedef struct
{
    double temp;
    double humi;
} sensor_queue_data_t;


/*
    @brief Estrutura de dados para armazenar informações do estado da lâmpada

    A estrutura `lamp_queue_data_t` é utilizada para armazenar dados referentes ao estado da lâmpada.

    @param lamp_state Estado da lâmpada (ligada/desligada)
*/
typedef struct
{
    char lamp_state[5];
} lamp_queue_data_t;


/*
    @brief Função para implementar tarefa relacionada à comunicação MQTT

    A função `mqtt_task` é responsável por realizar tarefas relacionadas à comunicação MQTT.
   
*/
void mqtt_task(void * ignore);    

/*
    @brief Função para implementar tarefa relacionada à leitura de sensores

    A função `sensor_task` é responsável por realizar tarefas relacionadas à leitura de sensores.

*/
void sensor_task(void * ignore);  

/*
    @brief Função para implementar tarefa relacionada à exibição de informações

    A função `display_task` é responsável por realizar tarefas relacionadas à exibição de informações.

*/
void display_task(void * ignore); 

/*
    @brief Função para implementar tarefa relacionada ao controle da lâmpada

    A função `lamp_task` é responsável por realizar tarefas relacionadas ao controle da lâmpada.

*/
void lamp_task(void * ignore);    

/*
    @brief Função para implementar tarefa relacionada ao botão

    A função `button_task` é responsável por realizar tarefas relacionadas ao botão.

*/
void button_task(void * ignore);  
