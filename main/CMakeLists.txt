file(GLOB_RECURSE SOURCES *.c) 
file(GLOB_RECURSE INCLUEDES gui/images gui/widgets gui/screens  gui/lv_lib_qrcode sensors external storage)

idf_component_register( SRCS ${SOURCES}
                        INCLUDE_DIRS ${INCLUEDES} .)

