cmake_minimum_required(VERSION 3.15)
project(Languages)

set(CMAKE_CXX_STANDARD 17)

add_executable(Languages 
    src/main.cpp
    src/student.cpp    
    src/languages.cpp
    src/intensity.cpp
)