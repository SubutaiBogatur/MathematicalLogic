cmake_minimum_required(VERSION 3.5.1)
project(FormalInequalities)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES main.cpp prover/prover.cpp)
add_executable(FormalInequalities ${SOURCE_FILES})
