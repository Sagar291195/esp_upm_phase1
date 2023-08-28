
idf_component_register(SRCS "src/sampleManagement.c"
                            "src/sequenceManagement.c"
                            "src/counters.c"
                            "src/timeManagement.c"
                            "src/stateMachine.c"
                            "src/dataMangement.c"
                            "src/sensorManagement.c"
                            "src/manuCompensationLayer.c"
                            "src/featureData.c"
                            "src/userCompensationLayer.c"
                    INCLUDE_DIRS include 
                    REQUIRES esp_upm_middleware esp_upm_sensors lv_examples nvs_flash)