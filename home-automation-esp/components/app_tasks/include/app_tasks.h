#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <time.h>


/*
    São criadas duas estruturas de dados: 
    1:  sensor_queue_data_t armazena dados referentes aos sensores, 
        contém duas variáveis double, referentes à temperatura (temp) e humidade (humi)
    2:  lamp_queue_data_t armazena dados referentes ao estado da lâmpada
*/
typedef struct
{
    double temp;
    double humi;
} sensor_queue_data_t;

typedef struct
{
    char lamp_state[5];
} lamp_queue_data_t;

/*
    São criadas funções para implementa tarefas específicas do sistema de automação residencial como:
    1:Comunicação MQTT
    2:Leitura de sensores,
    3:Leitura de informações
    4:Controle da lâmpada 
    5:Manipulação de eventos por meio de um botão.
*/

void mqtt_task(void * ignore);     // Função responsável por realizar tarefas relacionadas à comunicação MQTT.
void sensor_task(void * ignore);   // Função responsável por realizar tarefas relacionadas à leitura de sensores.
void display_task(void * ignore);  // Função responsável por realizar tarefas relacionadas à exibição de informações.
void lamp_task(void * ignore);     // Função responsável por realizar tarefas relacionadas ao controle da lâmpada.
void button_task(void * ignore);   // Função responsável por realizar tarefas relacionadas ao botão.
