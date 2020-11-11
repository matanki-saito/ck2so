cmake_minimum_required(VERSION 3.8)
project(hookProject CXX ASM_NASM)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(Boost::boost)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
set(TARGETS hooook)

add_library (hooook SHARED 
    "pch.h"
    "plugin_64.h"
    "injector/injector.hpp"
    "byte_search/byte_pattern.h"
    "byte_search/byte_pattern.cpp"
    "version/version.cpp"
    "font/font.cpp"
    "main_text/main_text.cpp"
    "main_text/main_text_asm.asm"
    "date/date.cpp"
    "hooook.cpp")

set_property(TARGET ${TARGETS} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${TARGETS} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${TARGETS} PROPERTY CXX_EXTENSION OFF)
