cmake_minimum_required(VERSION 3.16)

include(sdk/pico_sdk_init.cmake)

project(nanoled)

pico_sdk_init()

add_executable(nanoled
	source/main.c
	source/led.c
	source/descriptors.c
)

target_include_directories(nanoled PRIVATE source)

target_link_libraries(nanoled
	pico_stdlib
	hardware_pio
	tinyusb_device
)

target_compile_options(nanoled PRIVATE -Wall -Wextra)

pico_add_extra_outputs(nanoled)