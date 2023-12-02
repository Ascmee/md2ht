cmake_minimum_required(VERSION 3.26)
project(Cl_MdToHTML)

set(CMAKE_CXX_STANDARD 20)

add_executable(Cl_MdToHTML main.cpp
        name_arr.cpp
        name_arr.h
        console_util.cpp
        console_util.h
        data_convert.cpp
        data_convert.h
        file_util.cpp
        file_util.h)
