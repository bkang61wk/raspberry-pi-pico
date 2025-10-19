cmake_minimum_required(VERSION 3.13...3.27)

set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/sdk/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(sevenseg)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(sevenseg sevenseg.c)

target_link_libraries(sevenseg pico_stdlib)

pico_add_extra_outputs(sevenseg)