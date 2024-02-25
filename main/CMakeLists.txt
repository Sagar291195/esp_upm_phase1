file(GLOB_RECURSE SOURCES *.c) 
file(GLOB_RECURSE INCLUDES  GUI/custom GUI/generated GUI/guider_fonts GUI/images GUI/guider_customer_fonts peripherals)

idf_component_register( SRCS ${SOURCES}
                        INCLUDE_DIRS ${INCLUDES} .
                       )

