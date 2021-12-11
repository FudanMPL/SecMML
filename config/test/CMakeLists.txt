cmake_minimum_required(VERSION 3.7)  
 
project(test)
 
include_directories(/home/txy/SecMML/config)   #加入json库的路径
 
add_executable(test test.cpp ../Config.hpp ../Config.cpp)


target_link_libraries(test /home/txy/SecMML/config/json/libjsoncpp.so) 
 
#链接json库文件“libjsoncpp.so”