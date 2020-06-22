cmake_minimum_required(VERSION 3.8)
#project(sample3 CXX ASM_NASM)
project(hookProject VERSION 0.1.0)

include(CTest)
enable_testing()

# hook program
find_package(Boost REQUIRED COMPONENTS system)
include_directories(Boost::boost)
#set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
#set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")

add_library (hooook SHARED 
    "pch.h"
    "plugin_64.h"
    "injector/injector.hpp"
    "byte_search/byte_pattern.h"
    "byte_search/byte_pattern.cpp"
    "version/version.cpp"
    "font/font.cpp"
    "date/date.cpp"
    "hooook.cpp")
set(TARGETS hooook)
set_property(TARGET ${TARGETS} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${TARGETS} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${TARGETS} PROPERTY CXX_EXTENSION OFF)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_options(hooook PRIVATE -g -Fdwarf)
#    target_compile_options(target_program PRIVATE -g -Fdwarf)
#endif()

