cmake_minimum_required(VERSION 3.10)
project(StrategyLibrary)

set(CMAKE_CXX_STANDARD 11)

add_library(strategy SHARED strategy.cpp)
