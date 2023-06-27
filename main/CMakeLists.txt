idf_component_register(SRCS "wifi.c" "ota.c" "main.c"
                        INCLUDE_DIRS "."
                        EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)

list(APPEND SOURCES "scanneri2c.c")
list(APPEND SOURCES "scanneri2c.h")
