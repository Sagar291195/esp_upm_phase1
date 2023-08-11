set(COMPONENT_ADD_INCLUDEDIRS "include")

idf_component_register(SRCS "sqlite3.c" "esp32.c" "shox96_0_2.c"
INCLUDE_DIRS "include"
PRIV_INCLUDE_DIRS "private_include"
REQUIRES mbedtls
PRIV_REQUIRES console spiffs)
set_source_files_properties(
sqlite3.c
PROPERTIES COMPILE_FLAGS
"-Wno-unused-function -Wno-unused-variable -Wno-unused-value -Wno-unused-label -Wno-missing-field-initializers -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-cast-function-type"
)
set_source_files_properties(
esp32.c
PROPERTIES COMPILE_FLAGS
"-Wno-unused-function -Wno-unused-variable -Wno-unused-value -Wno-extra -Wno-type-limits -Wno-unused-label -Wno-missing-field-initializers -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-cast-function-type"
)
set_source_files_properties(
shox96_0_2.c
PROPERTIES COMPILE_FLAGS
"-Wno-unused-function -Wno-unused-variable -Wno-char-subscripts -Wno-unused-value -Wno-unused-label -Wno-missing-field-initializers -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-cast-function-type"
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu99 -g3 -fno-stack-protector -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -Wpointer-arith -Wno-error=unused-value -Wno-error=unused-label -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-error=char-subscripts -Wno-error=maybe-uninitialized -Wno-unused-parameter -Wno-sign-compare -Wno-old-style-declaration -MMD -c -DF_CPU=240000000L -DESP32 -DCORE_DEBUG_LEVEL=0 -DNDEBUG)