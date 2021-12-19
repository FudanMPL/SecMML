cmake_minimum_required(VERSION 3.15)
project(MPL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -pthread")

set(SOURCE_FILES_BASE ../Constant.cpp ../Constant.h ../util/SocketOnline.cpp ../util/SocketOnline.h ../util/SocketManager.cpp ../util/SocketManager.h ../core/Mat.cpp ../core/Mat.h ../core/Player.cpp ../core/Player.h ../core/MathOp.cpp ../core/MathOp.h ../core/MathOpCore.cpp ../core/Op.cpp ../core/Op.h ../core/NeuronMat.cpp ../core/NeuronMat.h ../machine_learning/NN.cpp ../machine_learning/NN.h ../machine_learning/BPGraph.cpp ../machine_learning/BPGraph.h ../util/IOManager.cpp ../util/IOManager.h ../config/Config.cpp ../config/Config.hpp ../malicious_lib/reed_solomn_reconstruct.h ../malicious_lib/reed_solomn_reconstruct.cpp)
set(MALICIOUS_LIB ../malicious_lib/reed_solomn_reconstruct.h ../malicious_lib/reed_solomn_reconstruct.cpp)

include_directories(~/SecMML/config)   #加入json库的路径
add_executable(test_Mat test_Mat.cpp ../core/Mat.h ../core/Mat.cpp ../Constant.h ../Constant.cpp ../config/Config.hpp ../config/Config.cpp )
target_link_libraries(test_Mat ~/SecMML/config/json/libjsoncpp.so) 


add_executable(test_Config test_Config.cpp ${SOURCE_FILES_BASE} )
target_link_libraries(test_Config ~/SecMML/config/json/libjsoncpp.so)  

add_executable(test_IOManager test.cpp ${SOURCE_FILES_BASE})
target_link_libraries(test_IOManager ~/SecMML/config/json/libjsoncpp.so)  