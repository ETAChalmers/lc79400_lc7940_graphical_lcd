cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(lcd)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(main)

pico_generate_pio_header(main ${CMAKE_CURRENT_LIST_DIR}/lcd.pio)

target_sources(main PRIVATE main.c lcd.c image.c)

pico_enable_stdio_usb(main ENABLED)
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(main pico_stdlib hardware_pio hardware_dma)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(main)
