set_global_assignment -entity "floatingPoint" -library "lib_floatingPoint" -name IP_TOOL_NAME "Qsys"
set_global_assignment -entity "floatingPoint" -library "lib_floatingPoint" -name IP_TOOL_VERSION "16.1"
set_global_assignment -entity "floatingPoint" -library "lib_floatingPoint" -name IP_TOOL_ENV "Qsys"
set_global_assignment -library "lib_floatingPoint" -name SPD_FILE [file join $::quartus(sip_path) "../floatingPoint.spd"]
set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "../../floatingPoint.qsys"]

set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "floatingPoint.v"]
set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "submodules/fpoint2_combi.vhd"]
set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "submodules/FPMinMaxFused/FPMinMaxFused.vhd"]
set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "submodules/FPCompareFused/FPCompareFused.vhd"]
set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "submodules/FPNeg_Abs/FPNeg.vhd"]
set_global_assignment -library "lib_floatingPoint" -name MISC_FILE [file join $::quartus(sip_path) "submodules/FPNeg_Abs/FPAbs.vhd"]
