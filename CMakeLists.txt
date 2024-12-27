cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(ft_executor)

option(BUILD_TEST "Build tests" ON)
option(BUILD_EXECUTOR "Build executor (for_compile_commands)" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -fsanitize=address")

file(GLOB EXECUTOR_SOURCES "src/*.c")

if(BUILD_TEST)
  file(GLOB EXECUTOR_TEST_FILES "tests/*.cpp")
  set(EXECUTOR_TEST_NAME "run_tests_executor")

  add_executable(${EXECUTOR_TEST_NAME} ${EXECUTOR_SOURCES} ${EXECUTOR_TEST_FILES})
  target_link_libraries(${EXECUTOR_TEST_NAME} gtest gtest_main pthread gmock)

  target_include_directories(${EXECUTOR_TEST_NAME} PRIVATE .)

  include(GoogleTest)
  gtest_discover_tests(${EXECUTOR_TEST_NAME})
endif()

if(BUILD_EXECUTOR)
   add_library(ft_executor STATIC ${EXECUTOR_SOURCES})
endif()
