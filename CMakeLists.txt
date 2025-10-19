cmake_minimum_required(VERSION 3.13...3.27)

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

target_link_libraries(sevenseg pico_stdlib)

pico_add_extra_outputs(sevenseg)