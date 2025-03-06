cmake_minimum_required(VERSION 3.12)

IF (CMAKE_VERSION VERSION_LESS "3.20")
    SET(CMAKE_CXX_STANDARD 20)
ELSE()
    SET(CMAKE_CXX_STANDARD 23)
ENDIF()


project(http1.0)

add_executable(${PROJECT_NAME} main.cpp)