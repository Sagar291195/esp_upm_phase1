file(GLOB_RECURSE SOURCES *.c) 
file(GLOB_RECURSE INCLUEDES GUI/Images GUI/Language GUI/Widgets GUI/Screens  GUI/lv_lib_qrcode)

idf_component_register( SRCS ${SOURCES}
                        INCLUDE_DIRS ${INCLUEDES} .)

