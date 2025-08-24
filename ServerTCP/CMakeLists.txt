cmake_minimum_required(VERSION 3.10)
project(ServerTcpIp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/ServerMain.cpp
    src/SignalHandler.cpp
    src/SignalWorker.cpp
)

# Include директории
include_directories(include)

# Создание исполняемого файла
add_executable(server ${SOURCES})
