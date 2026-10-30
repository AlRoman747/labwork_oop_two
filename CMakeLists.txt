cmake_minimum_required(VERSION 3.14)
project(PolynomialLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Библиотека с классом Polynomialс
add_library(polynomial STATIC ./src/polynomial.cpp)
target_include_directories(polynomial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(polynomial PRIVATE -Wall -Wextra)
endif()

add_executable(main ./src/main.cpp)
target_link_libraries(main PRIVATE polynomial)

enable_testing()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # Для Windows: использовать общую CRT
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(tests ./tests/test_functions.cpp)
target_link_libraries(tests PRIVATE polynomial GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
