#
# Component Makefile
#

CFLAGS += -DLV_LVGL_H_INCLUDE_SIMPLE

COMPONENT_SRCDIRS := lv_examples          
        #lv_examples/src/screens

COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) .
