
set(srcs "src/espupm_tasks.c")
idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include 
                    REQUIRES esp_upm_middleware esp_upm_sensors )
                                        