cmake_minimum_required(VERSION 3.0.0)
include(pico_sdk_import.cmake)

set(projname "camera_Project")
set(project_PATH "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# FreeRTOS kernel source
#add_subdirectory(FreeRTOS-kernel)

include_directories("${project_PATH}/libs/")
pico_sdk_init()
 
add_executable(${projname}
        #camera_project_multicore.cpp
        #camera_project.cpp
        camera_project_PIO.cpp
        #"${project_PATH}/libs/read_ov7670.pio"
        "${project_PATH}/libs/PIO_setting.cpp"
        "${project_PATH}/libs/ov7670.cpp"
        "${project_PATH}/libs/ImageProcessing.cpp"
        "${project_PATH}/libs/myfunc.cpp"
        "${project_PATH}/libs/Array.cpp"
        "${project_PATH}/libs/TFT.cpp"
        )

pico_generate_pio_header(${projname} ${project_PATH}/libs/read_ov7670.pio)
pico_enable_stdio_usb(${projname} 1)
pico_enable_stdio_uart(${projname} 0)

target_link_libraries(${projname} 
        pico_stdlib 
        pico_stdio_usb 
        hardware_i2c 
        hardware_spi 
        hardware_pwm 
        hardware_clocks 
        hardware_irq 
        hardware_watchdog 
        pico_multicore 
        hardware_sync
        hardware_pio
        #FreeRTOS-Kernel
        #FreeRTOS-Kernel-Heap4
)
pico_add_extra_outputs(${projname})


