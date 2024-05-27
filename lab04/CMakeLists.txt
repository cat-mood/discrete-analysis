cmake_minimum_required(VERSION 3.22)
project(lab04)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(tests_dir ${PROJECT_SOURCE_DIR}/tests/unit)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${source_dir}/*)
file(GLOB_RECURSE tests CONFIGURE_DEPENDS ${tests_dir}/*)

include_directories(${include_dir})

add_library(${PROJECT_NAME}_lib SHARED ${sources})

add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_benchmark benchmark.cpp)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_test ${tests})
target_link_libraries(
  ${PROJECT_NAME}_test 
  PRIVATE GTest::gtest_main
  PRIVATE ${PROJECT_NAME}_lib
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
