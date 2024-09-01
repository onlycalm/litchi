cmake_minimum_required(VERSION 3.8)

project(litchi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(litchi main.cpp)

target_link_libraries(litchi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
