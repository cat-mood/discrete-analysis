cmake_minimum_required(VERSION 3.22)
project(kp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(tests_dir ${PROJECT_SOURCE_DIR}/tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${source_dir}/*)
file(GLOB tests CONFIGURE_DEPENDS ${tests_dir}/*)

include_directories(${include_dir})

add_library(${PROJECT_NAME}_lib SHARED ${sources})

add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib)

# add_executable(${PROJECT_NAME}_benchmark benchmark.cpp)
# target_link_libraries(${PROJECT_NAME}_benchmark ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_test ${tests})
target_link_libraries(
  ${PROJECT_NAME}_test 
  PRIVATE GTest::gtest_main
  PRIVATE ${PROJECT_NAME}_lib
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
