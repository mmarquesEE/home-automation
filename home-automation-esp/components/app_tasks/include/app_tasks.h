/**
 * @file app_tasks.h
 * 
 * @brief Declaração das estruturas de dados e tarefas do sistema
 */

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <time.h>


/*
    @brief sensor_queue_data_t 

    Estrutura de dados para armazenar informações dos sensores como temperatura e umidade.

    @param temp Temperatura medida pelo sensor
    @param humi Umidade medida pelo sensor
*/
typedef struct
{
    double temp;
    double humi;
} sensor_queue_data_t;


/*
    @brief lamp_queue_data_t

    Estrutura de dados utilizada para armazenar dados referentes ao estado da lâmpada.

    @param lamp_state Estado da lâmpada (ligada/desligada)
*/

typedef struct
{
    char lamp_state[5];
} lamp_queue_data_t;


/*
    @brief mqtt_task
    
    Função responsável por realizar tarefas relacionadas à comunicação MQTT.
   
*/
void mqtt_task(void * ignore);    

/*
    @brief sensor_task
    
    Função para implementar tarefa relacionada à leitura de sensores.

*/
void sensor_task(void * ignore);  

/*
    @brief display_task
    Função responsável por realizar tarefas relacionadas à exibição de informações.

*/
void display_task(void * ignore); 

/*
    @brief lamp_task
    
    Função responsável por realizar tarefas relacionadas ao controle da lâmpada.

*/
void lamp_task(void * ignore);    

/*
    @brief button_task
    
     Função responsável por realizar tarefas relacionadas ao botão.

*/
void button_task(void * ignore);  

