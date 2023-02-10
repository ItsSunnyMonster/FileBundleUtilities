cmake_minimum_required(VERSION 3.23)

project(OptiCraftAssetPackCLI)

if(MINGW)
    message(FATAL_ERROR "MinGW is not supported!")
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(ocapcli "src/main.cpp")

add_subdirectory(../Library libocap)

target_link_libraries(ocapcli PRIVATE libocap)

target_compile_definitions(ocapcli PRIVATE $<$<CONFIG:Debug>:DEBUG>)