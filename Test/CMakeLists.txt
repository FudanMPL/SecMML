cmake_minimum_required(VERSION 3.15)
project(MPL)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_Mat test_Mat.cpp ../core/Mat.h ../core/Mat.cpp ../Constant.h ../Constant.cpp)