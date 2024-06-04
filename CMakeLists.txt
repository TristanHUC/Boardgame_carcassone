cmake_minimum_required(VERSION 3.5)

project(projet_LO21 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

include_directories(include)
FILE(GLOB SOURCES src/* include/*)

add_executable(projet_LO21
    main.cpp
    ${SOURCES}
)
target_include_directories(projet_LO21 PRIVATE include)
target_link_libraries(projet_LO21 PRIVATE Qt6::Core Qt6::Widgets)
target_compile_options(projet_LO21 PRIVATE -Wextra -Wall -pedantic)
