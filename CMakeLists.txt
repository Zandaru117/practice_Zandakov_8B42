cmake_minimum_required(VERSION 3.10.0)
project(pong VERSION 0.1.0 LANGUAGES C CXX)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
add_executable(pong main.cpp)
target_link_libraries(pong PRIVATE sfml-graphics sfml-window sfml-system)
