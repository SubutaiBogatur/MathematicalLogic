cmake_minimum_required(VERSION 3.5)
project(ClassicalLogicProofChecker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11 -O3")

set(SOURCE_FILES main.cpp parser/parser.cpp parser/postfix_string_builder.cpp axioms.cpp ast.cpp classical_logic_proof_checker.cpp parser/parser_exception.h)

add_executable(ClassicalLogicProofChecker ${SOURCE_FILES})
