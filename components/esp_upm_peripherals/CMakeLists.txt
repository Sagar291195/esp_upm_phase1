set(srcs "src/buzzer.c"
          "src/ws2812.c"
          "src/motor.c")
idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include
                        REQUIRES esp_upm_middleware esp_upm_sensors esp_upm_tasks lv_examples)
                                        