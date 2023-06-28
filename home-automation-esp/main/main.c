#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "app_tasks.h"

QueueHandle_t sensor_queue;
QueueHandle_t lamp_queue;
QueueHandle_t time_queue;

/*
   Função Principal: app_main é a função principal que inicia o programa no microcontrolador STM32.
/*
    
void app_main(void)
{
    // Criação das filas para comunicação entre as tarefas do sistema
    sensor_queue = xQueueCreate(1, sizeof(sensor_queue_data_t));
    lamp_queue = xQueueCreate(1, sizeof(lamp_queue_data_t));
    time_queue = xQueueCreate(1, sizeof(time_t));

    // Variáveis do tipo 'TaskHandle_t' para armazenar os identificadores das tarefas
  
    TaskHandle_t lamp_task_handle;
    TaskHandle_t mqtt_task_handle;
    TaskHandle_t sensor_task_handle;
    TaskHandle_t display_task_handle;
    TaskHandle_t button_task_handle;
    
    // Criação das tarefas

    /*
   Função xTaskCreate: app_main é a função usada para criar as tarefas, passando o nome da função da tarefa, tamanho da pilha, parâmetros adicionais, prioridade da tarefa e o endereço para armazenar o identificador da tarefa.
    /*
    
    xTaskCreate(lamp_task, "lamp_task", 10000, NULL, 4, &lamp_task_handle); // criando a tarefa 'lamp_task' responsável pelo controle das lâmpadas.
    xTaskCreate(mqtt_task,"mqtt_task",10000, NULL, 2, &mqtt_task_handle);  // criando a tarefa 'mqtt_task' responsável pela comunicação MQTT com um servidor externo.
    xTaskCreate(sensor_task, "sensor_task", 10000, NULL, 1, &sensor_task_handle); // criando a tarefa 'sensor_task' responsável pela leitura de sensores.
    xTaskCreate(display_task, "display_task", 10000, NULL, 3, &display_task_handle); // criando a tarefa 'display_task' esponsável pelo controle do display.
    xTaskCreate(button_task, "button_task", 10000, NULL, 5, &button_task_handle); // criando a tarefa 'button_task'responsável pelo tratamento de eventos de botões.
}
