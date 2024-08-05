cmake_minimum_required(VERSION 3.28)

project(BankArdisTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# RapidJSON
include_directories(${CMAKE_SOURCE_DIR}/extlib/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

configure_file(${CMAKE_SOURCE_DIR}/input.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/input.json COPYONLY)

add_executable(BankArdisTest
    main.cpp
    Bank.cpp
    Bank.h
    Client.cpp
    Client.h
    Department.cpp
    Department.h
    JsonLoader.h
    JsonLoader.cpp
    Logger.h
    Logger.cpp
)
