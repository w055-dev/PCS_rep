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