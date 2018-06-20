cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(backApp CXX)

set(SOURCES
  main.cpp
  watcher.cpp
  custom.cpp
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} stdc++fs)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
