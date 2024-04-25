file(GLOB_RECURSE SOURCES *.c) 
file(GLOB_RECURSE INCLUEDES gui/images gui/widgets gui/screens  gui/lv_lib_qrcode peripherals storage)

idf_component_register( SRCS ${SOURCES}
                        INCLUDE_DIRS ${INCLUEDES} .
                        EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)

