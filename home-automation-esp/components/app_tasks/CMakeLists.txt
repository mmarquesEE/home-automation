idf_component_register(SRCS "display_task.c" "mqtt_task.c" "sensor_task.c" "display_task"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                             mqtt
                             oled
                             dht11
                             wifi_conn)
