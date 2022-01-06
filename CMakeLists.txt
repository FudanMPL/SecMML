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

set(MALICIOUS_LIB malicious_lib/reed_solomn_reconstruct.h malicious_lib/reed_solomn_reconstruct.cpp malicious_lib/feldman_vss.cpp malicious_lib/feldman_vss.h)
set(SOURCE_FILES_BASE Constant.cpp Constant.h util/SocketOnline.cpp util/SocketOnline.h util/SocketManager.cpp util/SocketManager.h core/Mat.cpp core/Mat.h core/Player.cpp core/Player.h core/Op.cpp core/Op.h core/NeuronMat.cpp core/NeuronMat.h machine_learning/NN.cpp machine_learning/NN.h machine_learning/BPGraph.cpp machine_learning/BPGraph.h core/MathOp.cpp core/MathOp.h util/IOManager.cpp util/IOManager.h config/Config.cpp config/Config.hpp machine_learning/LSTMGraph.cpp machine_learning/Cell.cpp)

include_directories(config)   #加入json库的路径
# core files in shared lib
set(CORE_SOURCE_FILES core/MathOpCore.cpp core/MathOp.h)
set(JSON_SOURCE_FILES config/json/reader.h config/json/json_reader.cpp config/json/json_value.cpp config/json/json_writer.cpp)
add_library(core_lib SHARED ${CORE_SOURCE_FILES}) # do not uncomment this line
add_library(jsoncpp SHARED ${JSON_SOURCE_FILES})



#add_executable(reed malicious_lib/reed_solomn_reconstruct.cpp malicious_lib/reed_solomn_reconstruct.h Constant.h Constant.cpp malicious_lib/feldman_vss.h malicious_lib/feldman_vss.cpp)
#add_executable(feldman malicious_lib/feldman_vss.cpp malicious_lib/feldman_vss.h)
add_executable(MPL main.cpp ${SOURCE_FILES_BASE} ${MALICIOUS_LIB} )
target_link_libraries(MPL core_lib)
target_link_libraries(MPL jsoncpp)
# target_link_libraries(MPL ws2_32)

option(queqiao "Option to provide python interfaces." OFF)
if(queqiao)
    find_package(pybind11 REQUIRED)
    file(GLOB_RECURSE WRAPPER_FILES RELATIVE ${PROJECT_SOURCE_DIR} *wrapper.cpp)

    add_library(queqiao_lib SHARED ${MALICIOUS_LIB} ${SOURCE_FILES_BASE} machine_learning/LSTMGraph.cpp machine_learning/Cell.cpp)
    pybind11_add_module(queqiao ${WRAPPER_FILES})
    target_link_libraries(queqiao PUBLIC queqiao_lib core_lib jsoncpp)
endif()
