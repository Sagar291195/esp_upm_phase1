set(srcs "src/middlewareTest.c"
             "src/middlewareTest.c" 
            "src/midFunctions.c")

idf_component_register(SRCS "${srcs}"
                        INCLUDE_DIRS include
                        REQUIRES "nvs_flash" lv_examples)
                            
                            