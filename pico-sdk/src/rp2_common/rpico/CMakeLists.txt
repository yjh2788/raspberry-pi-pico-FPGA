set(libname "rpico")

add_library(${libname} INTERFACE)
target_sources(${libname} INTERFACE
${CMAKE_CURRENT_LIST_DIR}/${libname}.c )
