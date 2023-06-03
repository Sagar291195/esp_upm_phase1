set(srcs "src/bmp280.c"
        "src/ds3231.c"
        "src/ina3221.c"
        "src/PID.c"
        "src/sdp32.c"
        "src/bme680.c"
             )
idf_component_register(SRCS "${srcs}"
                            INCLUDE_DIRS include
                            REQUIRES esp_upm_middleware lvgl_esp32_drivers lv_examples)