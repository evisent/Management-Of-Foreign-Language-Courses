cmake_minimum_required(VERSION 3.15)
project(Languages)

set(CMAKE_CXX_STANDARD 17)

add_executable(Languages 
    src/main.cpp
    src/student.cpp    
    src/languages.cpp
    src/intensity.cpp
    src/group_manager.cpp
    src/simulation.cpp
)

add_definitions(-D_WIN32_WINNT=0x0A00)

if(WIN32)
    target_link_libraries(Languages ws2_32 wsock32)
else()
    target_link_libraries(Languages pthread)
endif()