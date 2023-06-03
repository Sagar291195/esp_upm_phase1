#file(GLOB_RECURSE SOURCES "test_s.c")
#file(GLOB_RECURSE SOURCES lv_examples/lv_examples/src/ESPUPM-SIM_01/GUI/*.c)
set(srcs "src/espupm_tasks.c")
idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include 
                    REQUIRES esp_upm_middleware esp_upm_sensors lv_examples)
                                        