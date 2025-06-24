# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(FreeRTOS-Reborn C CXX ASM)

# --- Patro: Configuração e Inclusão do FreeRTOS ---

# 1. Verifica se a variável de ambiente existe para evitar erros
if(NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
    message(FATAL_ERROR "A variável de ambiente FREERTOS_KERNEL_PATH não foi definida.")
endif()

# 2. Inclui o script de importação do FreeRTOS que você encontrou.
#    Este script define os alvos (targets) 'FreeRTOS-Kernel', 'FreeRTOS-Kernel-Heap4', etc.
#    Usamos "/" para compatibilidade entre Windows, Linux e macOS.
string(REPLACE "\\" "/" FREERTOS_PATH_CMAKE "$ENV{FREERTOS_KERNEL_PATH}")
include(${FREERTOS_PATH_CMAKE}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# --- Fim da Configuração do FreeRTOS ---

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(FreeRTOS-Reborn main.c )

pico_set_program_name(FreeRTOS-Reborn "FreeRTOS-Reborn")
pico_set_program_version(FreeRTOS-Reborn "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(FreeRTOS-Reborn 0)
pico_enable_stdio_usb(FreeRTOS-Reborn 1)

# Linka todas as bibliotecas necessárias de uma só vez
target_link_libraries(FreeRTOS-Reborn PRIVATE
    # Biblioteca padrão do Pico SDK para funções de GPIO, tempo, etc.
    pico_stdlib

    # Bibliotecas do FreeRTOS definidas pelo script de importação
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# Add the standard include files to the build
target_include_directories(FreeRTOS-Reborn PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(FreeRTOS-Reborn)

