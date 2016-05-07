cmake_minimum_required(VERSION 3.5)
project(dicp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/dicp/DicpProblem.cpp src/dicp/DicpProblem.h src/dicp/DicpImage.cpp src/dicp/DicpImage.h src/dicp/DicpCommand.cpp src/dicp/DicpCommand.h)
add_executable(dicp ${SOURCE_FILES})