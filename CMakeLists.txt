cmake_minimum_required(VERSION 3.12)
project(MagillaCSChallenge)

set(CMAKE_CXX_STANDARD 17)

add_executable(MagillaCSChallenge CSChallenge.cpp Graph.cpp EdgeSequence.cpp EdgeSequence.h TextGraphParser.cpp TextGraphParser.h)