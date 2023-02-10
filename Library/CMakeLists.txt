cmake_minimum_required(VERSION 3.23)

if (TARGET libocap)
    return()
endif()

project(OCAPLibrary)

if(MINGW)
    message(FATAL_ERROR "MinGW is not supported!")
endif()

set(CMAKE_CXX_STANDARD 20)

add_library(libocap "src/LibOCAP/ocap.cpp" "src/ocap.hpp" "src/LibOCAP/HelperFunctions.hpp" "src/LibOCAP/HelperFunctions.cpp")

target_compile_definitions(libocap PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(libocap PUBLIC "src")