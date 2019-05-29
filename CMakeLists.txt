cmake_minimum_required(VERSION 2.8)
project(file)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(file
#        main.cpp
#        test.cpp
#        test1.cpp
#        Singleton.cpp
        readFile.cpp
#        dataTransfer.cpp
#        readTransfer.cpp
        )
target_link_libraries( file ${OpenCV_LIBS} )