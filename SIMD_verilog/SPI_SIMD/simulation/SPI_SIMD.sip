set_global_assignment -entity "SPI_SIMD" -library "lib_SPI_SIMD" -name IP_TOOL_NAME "Qsys"
set_global_assignment -entity "SPI_SIMD" -library "lib_SPI_SIMD" -name IP_TOOL_VERSION "16.1"
set_global_assignment -entity "SPI_SIMD" -library "lib_SPI_SIMD" -name IP_TOOL_ENV "Qsys"
set_global_assignment -library "lib_SPI_SIMD" -name SPD_FILE [file join $::quartus(sip_path) "../SPI_SIMD.spd"]
set_global_assignment -library "lib_SPI_SIMD" -name MISC_FILE [file join $::quartus(sip_path) "../../SPI_SIMD.qsys"]

set_global_assignment -library "lib_SPI_SIMD" -name MISC_FILE [file join $::quartus(sip_path) "SPI_SIMD.v"]
set_global_assignment -library "lib_SPI_SIMD" -name MISC_FILE [file join $::quartus(sip_path) "submodules/spiphyslave.v"]
