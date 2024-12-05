cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

project( json-cli )

find_package( fmt REQUIRED )

# --- JSON
set( JSON_BuildTests      OFF CACHE INTERNAL "" )
set( JSON_Install         OFF CACHE INTERNAL "" )
set( JSON_MultipleHeaders ON  CACHE INTERNAL "" )
add_subdirectory( external/json )

# --- CLI11
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/external/CLI11 )

set( exec jsoncli )

add_executable( ${exec} )

target_sources( ${exec} PRIVATE
    src/main.cpp
)

target_link_libraries( ${exec} PRIVATE fmt::fmt CLI11::CLI11 nlohmann_json )

set_target_properties( ${exec} PROPERTIES CXX_STANDARD 20 )
target_compile_options( ${exec} PRIVATE -Wall -Wextra )