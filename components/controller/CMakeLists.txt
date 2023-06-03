idf_component_register(SRCS "src/controller.c"
                        INCLUDE_DIRS include
                        REQUIRES "nvs_flash" lv_examples esp_upm_peripherals Management)