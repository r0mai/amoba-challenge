cmake_minimum_required(VERSION 2.8)

project(sample-cxx)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O2 -g")
endif()

aux_source_directory(src SRC_SOURCES)
aux_source_directory(bin BIN_SOURCES)

set(SRC_LIB_NAME ${CMAKE_PROJECT_NAME}-src)

add_executable(${SRC_LIB_NAME} ${SRC_SOURCES})
