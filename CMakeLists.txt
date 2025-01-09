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

project(minishell)

option(BUILD_TEST "Build tests" ON)
option(BUILD_MINISHELL "Build minishell" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -fsanitize=address")

add_subdirectory(libft)
file(GLOB_RECURSE MINISHELL_SOURCES "src/*.c")

if(BUILD_TEST)
  file(GLOB UNITTEST_FILES "tests/unittests/*.cpp")
  set(UNITTEST_EXECUTABLE "run_unittests")

  add_executable(${UNITTEST_EXECUTABLE} ${MINISHELL_SOURCES} ${UNITTEST_FILES})
  target_link_libraries(${UNITTEST_EXECUTABLE} gtest gtest_main pthread gmock)
  target_link_libraries(${UNITTEST_EXECUTABLE} ft readline)

  target_include_directories(${UNITTEST_EXECUTABLE} PRIVATE libft)
  target_include_directories(${UNITTEST_EXECUTABLE} PRIVATE include)

  include(GoogleTest)
  gtest_discover_tests(${UNITTEST_EXECUTABLE})
endif()

file(GLOB PARSER_SOURCES "src/parser/*.c")
if(BUILD_MINISHELL)
  set(MINISHELL "minishell")

  add_executable(${MINISHELL} ${MINISHELL_SOURCES} ${PARSER_SOURCES} run/main.c)
  target_link_libraries(${MINISHELL} ft readline)

  target_include_directories(${MINISHELL} PRIVATE libft)
  target_include_directories(${MINISHELL} PRIVATE include)
endif()
