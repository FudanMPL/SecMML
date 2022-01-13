cmake_minimum_required(VERSION 3.7)
 
include_directories(../../config)
 
add_library(jsoncpp SHARED 
            reader.h
            json_reader.cpp
            json_value.cpp
            json_writer.cpp)