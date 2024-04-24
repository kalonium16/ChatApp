cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(Server)
project(Client)


set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.84.0) 
include_directories(${Boost_INCLUDE_DIRS})
include_directories(./common/)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Server 
        ./server/main.cpp 
        ./server/server.cpp ./server/server.h)
    add_executable(Client 
        ./client/main.cpp 
        ./client/client.cpp ./client/client.h 
        ./client/inputHandler.cpp ./client/inputHandler.h 
        ./client/requestHandler.cpp ./client/requestHandler.h
        ./client/common.h)
endif()

