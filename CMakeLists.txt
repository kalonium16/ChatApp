cmake_minimum_required(VERSION 3.20)

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
    add_executable(Server ./server/main.cpp ./server/server.cpp)
    add_executable(Client ./client/main.cpp ./client/client.cpp ./client/inputHandler.cpp ./client/requestHandler.cpp)
endif()

