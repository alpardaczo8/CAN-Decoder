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

# GoogleTest

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(CANDecoderTests
    tests/DbcParserText.cpp
)

target_compile_definitions(CANDecoderTests PRIVATE
    TEST_FILES_DIR="${CMAKE_SOURCE_DIR}/files"
)

target_link_libraries(CANDecoderTests PRIVATE
    CANDecoderLib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(CANDecoderTests)