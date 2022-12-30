cmake_minimum_required(VERSION 3.23)

project(AssetPackCompiler)

if(MINGW)
    message(FATAL_ERROR "MinGW is not supported!")
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(AssetPackCompiler "src/main.cpp")