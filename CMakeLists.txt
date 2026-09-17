cmake_minimum_required(VERSION 3.15)
project(CPP_Decimal VERSION 1.0.0 LANGUAGES CXX)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# HEADER-ONLY interface target
add_library(decimal INTERFACE)
target_include_directories(decimal INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compiler warning flags
if(MSVC)
    target_compile_options(decimal INTERFACE
        /W4
        /utf-8
        /Zc:__cplusplus
    )
else()
    target_compile_options(decimal INTERFACE
        -Wall
        -Wextra
    )
endif()

# Testing target: decimal_test (tests/*.cpp)
enable_testing()

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
if(TEST_SOURCES)
    add_executable(decimal_test ${TEST_SOURCES})
    target_link_libraries(decimal_test PRIVATE decimal)
    add_test(NAME decimal_unit_tests COMMAND decimal_test)
endif()
