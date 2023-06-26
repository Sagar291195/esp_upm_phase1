if(ESP_PLATFORM)

file(GLOB_RECURSE SOURCES lv_examples/*.c)
file(GLOB_RECURSE INCLUEDES lv_examples/ lv_examples/src/ESPUPM_SIM_01/GUI/Language lv_examples/src/ESPUPM_SIM_01/GUI/Widgets lv_examples/src/ESPUPM_SIM_01/GUI/Screens lv_examples/src/ESPUPM_SIM_01/GUI/lv_lib_qrcode) 
#file(GLOB_RECURSE SOURCES lv_examples/lv_examples/src/ESPUPM-SIM_01/GUI/*.c)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS ${INCLUEDES} .
                       REQUIRES lvgl controller)

else()
    message(FATAL_ERROR "LVGL LV examples: ESP_PLATFORM is not defined. Try reinstalling ESP-IDF.")
endif()
