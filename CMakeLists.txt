cmake_minimum_required(VERSION 3.21)

project(physics LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(main
  main.c
  physProtocol.c
  ball.c
)
target_compile_features(main PRIVATE c_std_11)

find_package(Curses REQUIRED)

target_link_libraries(main PRIVATE ${CURSES_LIBRARIES})
target_include_directories(main PRIVATE ${CURSES_INCLUDE_DIRS})
