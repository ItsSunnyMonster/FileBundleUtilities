cmake_minimum_required(VERSION 3.23)

project(AssetPackCompiler)

if(MINGW)
    message(FATAL_ERROR "MinGW is not supported!")
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/fmt)

add_executable(AssetPackCompiler "src/main.cpp" "src/HelperFunctions.hpp")

target_link_libraries(AssetPackCompiler PRIVATE fmt::fmt)

target_compile_definitions(AssetPackCompiler PRIVATE $<$<CONFIG:Debug>:DEBUG>)