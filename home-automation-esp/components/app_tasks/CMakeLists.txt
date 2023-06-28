idf_component_register(SRCS "display_task.c" "sensor_task.c" "mqtt_task.c" "lamp_task.c" "button_task.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                             mqtt
                             oled
                             dht11
                             wifi_conn
                             )
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
