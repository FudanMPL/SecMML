cmake_minimum_required(VERSION 3.15)
project(MPL)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

# core files in shared lib
# set(CORE_SOURCE_FILES core/MathOpCore.cpp core/MathOp.h)
# add_library(core_lib SHARED ${CORE_SOURCE_FILES}) # do not uncomment this line
link_directories(${PROJECT_SOURCE_DIR})


set(MALICIOUS_LIB malicious_lib/reed_solomn_reconstruct.h malicious_lib/reed_solomn_reconstruct.cpp malicious_lib/feldman_vss.cpp malicious_lib/feldman_vss.h)
set(SOURCE_FILES_BASE Constant.cpp Constant.h util/SocketOnline.cpp util/SocketOnline.h util/SocketManager.cpp util/SocketManager.h core/Mat.cpp core/Mat.h core/Player.cpp core/Player.h core/Op.cpp core/Op.h core/NeuronMat.cpp core/NeuronMat.h machine_learning/NN.cpp machine_learning/NN.h machine_learning/BPGraph.cpp machine_learning/BPGraph.h core/MathOp.cpp core/MathOp.h util/IOManager.cpp util/IOManager.h)


#add_executable(reed malicious_lib/reed_solomn_reconstruct.cpp malicious_lib/reed_solomn_reconstruct.h Constant.h Constant.cpp malicious_lib/feldman_vss.h malicious_lib/feldman_vss.cpp)
#add_executable(feldman malicious_lib/feldman_vss.cpp malicious_lib/feldman_vss.h)
add_executable(MPL main.cpp ${SOURCE_FILES_BASE} ${MALICIOUS_LIB} machine_learning/LSTMGraph.cpp machine_learning/Cell.cpp)
target_link_libraries(MPL core_lib)
# target_link_libraries(MPL ws2_32)