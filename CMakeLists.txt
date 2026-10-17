cmake_minimum_required(VERSION 3.16)
project(uml_sequence_tool CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(core STATIC
    src/json_parser.cpp
    src/class_diagram_parser.cpp
    src/sequence_diagram_parser.cpp
    src/cycle_analyzer.cpp
    src/markdown_exporter.cpp
    src/logger.cpp
)
target_include_directories(core PUBLIC src)

add_executable(uml_tool src/main.cpp src/plugin_loader.cpp)
target_link_libraries(uml_tool PRIVATE core ${CMAKE_DL_LIBS})
target_link_options(uml_tool PRIVATE -rdynamic)
set_target_properties(uml_tool PROPERTIES ENABLE_EXPORTS ON)

# ---- Плагины ----
set(PLUGIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/plugins")

add_library(export_json MODULE plugins/export_json.cpp)
target_include_directories(export_json PRIVATE src)
set_target_properties(export_json PROPERTIES
    PREFIX "lib"
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}")

add_library(export_html MODULE plugins/export_html.cpp)
target_include_directories(export_html PRIVATE src)
set_target_properties(export_html PROPERTIES
    PREFIX "lib"
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}")