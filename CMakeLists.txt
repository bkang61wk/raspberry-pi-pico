cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)
# Set Raspberry Pi Pico SDK path
set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/sdk/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(sevenseg)

# Use GNU extensions so C binary literals (0b...) are accepted.
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Generate compile_commands.json for VS Code IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(sevenseg sevenseg.c)

target_link_libraries(sevenseg pico_stdlib hardware_spi pico_cyw43_arch_none pico_multicore hardware_uart)

pico_enable_stdio_usb(sevenseg 1)
pico_enable_stdio_uart(sevenseg 0)

pico_add_extra_outputs(sevenseg)
