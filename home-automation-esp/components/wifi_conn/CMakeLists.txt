idf_component_register(SRCS "wifi_conn.c"
                    INCLUDE_DIRS "include"
                    REQUIRES lwip esp_wifi nvs_flash)
