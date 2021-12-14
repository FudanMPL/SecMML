cmake_minimum_required(VERSION 3.15)
project(MPL)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_Mat test_Mat.cpp ../core/Mat.h ../core/Mat.cpp ../Constant.h ../Constant.cpp)




project(testConfig)
include_directories(/home/txy/SecMML/config)   #加入json库的路径
add_executable(test_Config test_Config.cpp ../config/Config.hpp ../config/Config.cpp)
target_link_libraries(test_Config /home/txy/SecMML/config/json/libjsoncpp.so) 