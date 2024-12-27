cmake_minimum_required(VERSION 3.14)
project(Libft)

file(GLOB LIBFT_SOURCES "*.c")

add_library(ft STATIC
  # List all your source files in libft here
  ${LIBFT_SOURCES}
  # Add more files as needed
)
target_include_directories(ft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
