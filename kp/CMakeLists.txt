cmake_minimum_required(VERSION 3.22)
project(kp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}_exe main.cpp)
add_executable(${PROJECT_NAME}_benchmark benchmark.cpp)