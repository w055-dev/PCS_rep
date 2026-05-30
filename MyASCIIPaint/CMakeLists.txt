cmake_minimum_required(VERSION 3.10)
project(MyASCIIPaint)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif() 

set(NLOHMANN_JSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
    src/Commands.cpp
    src/Tools.cpp
    src/Exporters.cpp
    src/UserInputHandler.cpp
    src/HistoryManager.cpp
    src/Terminal.cpp
)

add_executable(ascii_paint ${SOURCES})

target_include_directories(ascii_paint PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(test_cell
    tests/test_cell.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_cell PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_cell Catch2WithMain)
add_test(NAME test_cell COMMAND test_cell)

add_executable(test_color
    tests/test_color.cpp
    src/Color.cpp
)
target_include_directories(test_color PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_color Catch2WithMain)
add_test(NAME test_color COMMAND test_color)

add_executable(test_layer
    tests/test_layer.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_layer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_layer Catch2WithMain)
add_test(NAME test_layer COMMAND test_layer)

add_executable(test_canvas
    tests/test_canvas.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_canvas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_canvas Catch2WithMain)
add_test(NAME test_canvas COMMAND test_canvas)

add_executable(test_commands
    tests/test_commands.cpp
    src/Commands.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_commands PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_commands Catch2WithMain)
add_test(NAME test_commands COMMAND test_commands)

add_executable(test_history
    tests/test_history.cpp
    src/HistoryManager.cpp
    src/Commands.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_history PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_history Catch2WithMain)
add_test(NAME test_history COMMAND test_history)

add_executable(test_tools
    tests/test_tools.cpp
    src/Tools.cpp
    src/Commands.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_tools PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_tools Catch2WithMain)
add_test(NAME test_tools COMMAND test_tools)

add_executable(test_exporters
    tests/test_exporters.cpp
    src/Exporters.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(test_exporters PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_exporters Catch2WithMain)
add_test(NAME test_exporters COMMAND test_exporters)

add_executable(scenario_drawing
    tests/scenarios/scenario_drawing.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(scenario_drawing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(scenario_drawing Catch2WithMain)
add_test(NAME scenario_drawing COMMAND scenario_drawing)

add_executable(scenario_history
    tests/scenarios/scenario_history.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
    src/Commands.cpp
    src/HistoryManager.cpp
)
target_include_directories(scenario_history PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(scenario_history Catch2WithMain)
add_test(NAME scenario_history COMMAND scenario_history)

add_executable(scenario_undo_redo
    tests/scenarios/scenario_undo_redo.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
    src/Commands.cpp
    src/HistoryManager.cpp
)
target_include_directories(scenario_undo_redo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(scenario_undo_redo Catch2WithMain)
add_test(NAME scenario_undo_redo COMMAND scenario_undo_redo)

add_executable(scenario_layers
    tests/scenarios/scenario_layers.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
)
target_include_directories(scenario_layers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(scenario_layers Catch2WithMain)
add_test(NAME scenario_layers COMMAND scenario_layers)

add_executable(scenario_export
    tests/scenarios/scenario_export.cpp
    src/Canvas.cpp
    src/Layer.cpp
    src/Cell.cpp
    src/Color.cpp
    src/Exporters.cpp
)
target_include_directories(scenario_export PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(scenario_export Catch2WithMain)
add_test(NAME scenario_export COMMAND scenario_export)