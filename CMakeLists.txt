cmake_minimum_required(VERSION 3.20)
project(21f_pa03_caleb)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.40 COMPONENTS locale graph REQUIRED)

set (input01 "data/football.graphml")
set (input02 "data/karate.graphml")
set (input03 "data/dolphin.graphml")


foreach(file IN LISTS input01 input02 input03)
    configure_file(${file} ${file} COPYONLY)
endforeach()

add_executable(21f_pa03_caleb src/main.cpp src/graf.cpp src/graf.h)

target_link_libraries(21f_pa03_caleb Boost::locale Boost::graph)