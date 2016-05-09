cmake_minimum_required(VERSION 3.5)
project(dicp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(/usr/local/include/gecode)

link_directories(/usr/local/lib)

set(SOURCE_FILES src/main.cpp src/dicp/DicpProblem.cpp src/dicp/DicpProblem.h src/dicp/DicpImage.cpp src/dicp/DicpImage.h src/dicp/DicpCommand.cpp src/dicp/DicpCommand.h src/dicp/solvers/DicpSolver.cpp src/dicp/solvers/DicpSolver.h src/dicp/solvers/DicpGecodeExtensiveSolver.cpp src/dicp/solvers/DicpGecodeExtensiveSolver.h src/dicp/solvers/DicpSolver.cpp src/dicp/solvers/DicpSolver.h src/dicp/spaces/DicpGecodeExtensiveSpace.cpp src/dicp/spaces/DicpGecodeExtensiveSpace.h)
add_executable(dicp ${SOURCE_FILES})

target_link_libraries(dicp gecodeint gecodekernel gecodesearch gecodesupport)