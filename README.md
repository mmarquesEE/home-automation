# Hub de Automação Rsidencial
## Projeto final da disciplina de Sistemas em Tempo Real - DEE UFCG

## Descrição
O sistema realiza o controle de lâmpadas via botões de contato e remotamente, através do protocolo MQTT. O sistema também exibe a hora local (sincronizada via servidor ntp) e leituras de temperatura e umidade em um display OLED. Os dados de leitura do sensor de temperatura e umidade bem como o estado atual das lâmpadas são enviados ao broker MQTT.

## Componentes do hardware
- ESP32 Devkit v2
- Sensor de Temp. e Umidade DHT11
- Display OLED 128x64
- Botões de contato
- LEDs
- Capacitores e Resistores

## Tarefas implementadas
- Periódicas: Leitura do sensor, atualização do display, envio de dados do sistema ao broker MQTT, configuração do estado das lâmpadas
- Aperiódicas: Recebimento de comandos através de subscrição ao tópico MQTT, acionamento de lâmpadas via botões de contato.

## Detalhes das tarefas:
- Leitura do sensor: período - 1 s
- Atualização do display: período - 100 ms
- Atualização do estado das lâmpadas: período - 100 ms
- Envio de dados ao broker: período - 5 s

- Acionamento remoto: evento assíncrono disparado pelo driver MQTT (ESP-IDF)
- Acionamento via botões de contato: interrupções de hardware via GPIO.

## Comunicação entre tarefas
As tarefas comunicam-se através das filas:
- Fila de leituras do sensor
- Fila de estado das lâmpadas
- Fila de tempo (sincronizado por ntp)
- Fila de interrupção de hardware

OBS:
A tarefa de acionamento de lâmpadas lê o estado atual das lâmpadas da fila e configura os pinos de acionamento.
A tarefa de acionamento via botão de contato lê a fila de interrupção para produzir o estado adequado das lâmpadas e inseri-lo na fila de estado das lâmpadas.
A tarefa de acionamento remoto adiciona diretamento um novo estado na fila de estado das lâmpadas.

# Link da Simulação no YouTube: https://youtu.be/ibvub_Ab6E4
 
