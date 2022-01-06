cmake_minimum_required(VERSION 3.15)
project(MPL)

if (CMAKE_SYSTEM_NAME MATCHES "Windows" )
    set( my_std_pre "/std:" )
else()
    set( my_std_pre "-std=" )
endif()

set( basic_cxx17 "c++17" )
set( str_cxx17 "${my_std_pre}${basic_cxx17}" )

include( CheckCXXCompilerFlag )
check_cxx_compiler_flag( "${str_cxx17}" _cpp_17_flag_supported)
if ( _cpp_17_flag_supported )
    set( CMAKE_CXX_STANDARD 17 )
endif()

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -pthread")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -pthread")

# core files in shared lib
set(CORE_SOURCE_FILES ../core/MathOpCore.cpp ../core/MathOp.h)
add_library(core_lib SHARED ${CORE_SOURCE_FILES}) # do not uncomment this line

include_directories(../config)   #加入json库的路径
add_library(jsoncpp SHARED 
            ../config/json/reader.h
            ../config/json/json_reader.cpp
            ../config/json/json_value.cpp
            ../config/json/json_writer.cpp)

set(SOURCE_FILES_BASE ../Constant.cpp ../Constant.h ../util/SocketOnline.cpp ../util/SocketOnline.h ../util/SocketManager.cpp ../util/SocketManager.h ../core/Mat.cpp ../core/Mat.h ../core/Player.cpp ../core/Player.h ../core/MathOp.cpp ../core/MathOp.h ../core/MathOpCore.cpp ../core/Op.cpp ../core/Op.h ../core/NeuronMat.cpp ../core/NeuronMat.h ../machine_learning/NN.cpp ../machine_learning/NN.h ../machine_learning/BPGraph.cpp ../machine_learning/BPGraph.h ../util/IOManager.cpp ../util/IOManager.h ../config/Config.cpp ../config/Config.hpp ../malicious_lib/reed_solomn_reconstruct.h ../malicious_lib/reed_solomn_reconstruct.cpp ../machine_learning/LSTMGraph.cpp ../machine_learning/Cell.cpp)
set(MALICIOUS_LIB ../malicious_lib/reed_solomn_reconstruct.h ../malicious_lib/reed_solomn_reconstruct.cpp)


add_executable(test_Mat test_Mat.cpp ../core/Mat.h ../core/Mat.cpp ../Constant.h ../Constant.cpp ../config/Config.hpp ../config/Config.cpp)
target_link_libraries(test_Mat jsoncpp) 
target_link_libraries(test_Mat core_lib)

add_executable(test_Config test_Config.cpp ${SOURCE_FILES_BASE}  ${MALICIOUS_LIB})
target_link_libraries(test_Config jsoncpp)
target_link_libraries(test_Config core_lib)

add_executable(test_Cache test_cache.cpp ${SOURCE_FILES_BASE})
target_link_libraries(test_Cache  jsoncpp)  
target_link_libraries(test_Cache core_lib)

add_executable(test_IOManager test_IOManager.cpp ${SOURCE_FILES_BASE}  ${MALICIOUS_LIB}) 
target_link_libraries(test_IOManager jsoncpp)
target_link_libraries(test_IOManager core_lib)

add_executable(test_SocketManager test_SocketManager.cpp ${SOURCE_FILES_BASE}  ${MALICIOUS_LIB})
target_link_libraries(test_SocketManager jsoncpp)
target_link_libraries(test_SocketManager core_lib)

add_executable(test_LSTM test_LSTM.cpp ${SOURCE_FILES_BASE} ${MALICIOUS_LIB})
target_link_libraries(test_LSTM jsoncpp)
target_link_libraries(test_LSTM core_lib)