/**
 * @file main.c
 *
 * @brief Arquivo Principal do programa no microcontrolador STM32.
 * 
 * Este arquivo contém a implementação principal do aplicativo de automação residencial. 
 * Ele configura as filas de comunicação entre as tarefas e cria as tarefas para controlar 
 * a lâmpada, realizar comunicação MQTT, obter dados do sensor, atualizar o display e 
 * lidar interrupções de eventos por meio dos botões
 *
 * 
 */

#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "app_tasks.h"

QueueHandle_t sensor_queue; /**< Fila para comunicação de dados do sensor. */
QueueHandle_t lamp_queue; /**< Fila para comunicação de dados da lâmpada. */
QueueHandle_t time_queue;  /**< Fila para comunicação de dados de tempo. */


/**
 * @brief app_main
 * 
 * Função que inicia o programa no microcontrolador STM32.
 */

void app_main(void)
{
    // Criação das filas para comunicação entre as tarefas do sistema
    sensor_queue = xQueueCreate(1, sizeof(sensor_queue_data_t)); 
    lamp_queue = xQueueCreate(1, sizeof(lamp_queue_data_t)); 
    time_queue = xQueueCreate(1, sizeof(time_t));

/**
 * @brief TaskHandle_t
 * 
 * Tipo de variável para armazenar os identificadores das tarefas
 * 
 */

    TaskHandle_t lamp_task_handle;
    TaskHandle_t mqtt_task_handle;
    TaskHandle_t sensor_task_handle;
    TaskHandle_t display_task_handle;
    TaskHandle_t button_task_handle;
    

/**
 * @brief xTaskCreate'
 * 
 * Essa função é usada para criar as tarefas do sistema
 * 
 * @param[in] nome nome da função da tarefa
 * @param[in] tamanho tamanho da pilha
 * @param[in] adicionais parametros adicionais 
 * @param[in] prioridade ordem de prioridade da tarefa 
 * @param[in] endereco endereço de memória da tarefa 
 *
 */
  
    /**
     * @brief lamp_task
     *
     * Criação de uma tarefa 'lamp_task' responsável pelo controle das lâmpadas.
     *
     */

    xTaskCreate(lamp_task, "lamp_task", 10000, NULL, 4, &lamp_task_handle); // criando a tarefa 'lamp_task' responsável pelo controle das lâmpadas.
    /**
     * @brief mqtt_task
     *
     * Criação de uma tarefa 'mqtt_task' responsável pela comunicação MQTT com um servidor externo.
     *
     */
    xTaskCreate(mqtt_task,"mqtt_task",10000, NULL, 2, &mqtt_task_handle);  // criando a tarefa 'mqtt_task' responsável pela comunicação MQTT com um servidor externo.
      /**
     * @brief sensor_task
     *
     * Criação de uma tarefa 'sensor_task' responsável pela leitura de sensores.
     *
     */
    xTaskCreate(sensor_task, "sensor_task", 10000, NULL, 1, &sensor_task_handle); // criando a tarefa 'sensor_task' responsável pela leitura de sensores.
     /**
     * @brief display_task
     *
     * Criação de uma tarefa responsável pelo controle do display.
     *
     */
    xTaskCreate(display_task, "display_task", 10000, NULL, 3, &display_task_handle); // criando a tarefa 'display_task' esponsável pelo controle do display.
    /**
     * @brief button_task
     * 
     * Criação de uma tarefa de botões responsável pelo tratamento de eventos de botões.
     *
     */
    xTaskCreate(button_task, "button_task", 10000, NULL, 5, &button_task_handle); // criando a tarefa 'button_task'responsável pelo tratamento de eventos de botões.
}
