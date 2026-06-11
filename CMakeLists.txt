cmake_minimum_required(VERSION 3.24)

project(CANDecoder
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CanDecoder Library
add_library(CANDecoderLib
    src/DbcParser.cpp
)

target_include_directories(CANDecoderLib PUBLIC
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/can_decoder
)

# Executable
add_executable(CANDecoder
    src/main.cpp
)

target_link_libraries(CANDecoder PRIVATE CANDecoderLib)