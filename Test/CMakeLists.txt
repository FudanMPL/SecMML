cmake_minimum_required(VERSION 3.15)
project(MPL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -pthread")
include_directories(~/SecMML/config)   #加入json库的路径
add_executable(test_Mat test_Mat.cpp ../core/Mat.h ../core/Mat.cpp ../Constant.h ../Constant.cpp ../config/Config.hpp ../config/Config.cpp )
target_link_libraries(test_Mat ~/SecMML/config/json/libjsoncpp.so) 


add_executable(test_Config test_Config.cpp ../config/Config.hpp ../config/Config.cpp ../core/Player.h ../core/Player.cpp ../core/Mat.h ../core/Mat.cpp ../Constant.h ../Constant.cpp ../util/IOManager.h ../util/IOManager.cpp ../util/SocketManager.h ../util/SocketManager.cpp ../util/SocketOnline.cpp ../util/SocketOnline.h)
target_link_libraries(test_Config ~/SecMML/config/json/libjsoncpp.so) 