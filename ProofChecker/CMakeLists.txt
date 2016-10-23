cmake_minimum_required(VERSION 3.5)
project(ProofChecker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp parser/parser.cpp parser/parser.h parser/postfix_string_builder.h parser/postfix_string_builder.cpp axioms.h axioms.cpp ast.cpp ast.h hash.h proof_checker.cpp proof_checker.h)
add_executable(ProofChecker ${SOURCE_FILES})