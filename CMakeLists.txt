cmake_minimum_required(VERSION 3.20)
project(NFA)

set(CMAKE_CXX_STANDARD 14)

add_executable(NFA main.cpp NFA.cpp NFA.h NFAState.cpp NFAState.h DFA.h DFA.cpp DFAState.cpp DFAState.h json.hpp ENFA.cpp ENFA.h RE.cpp RE.h)
