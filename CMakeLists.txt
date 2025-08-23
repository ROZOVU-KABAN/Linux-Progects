cmake_minimum_required(VERSION 3.10)
project(KeyLogger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KeyLogger 
	       KeyLogger/srv/KeyLogger.cpp
)
