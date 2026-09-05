cmake_minimum_required(VERSION 3.10)
project(shell)
 
# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
 
# Include directory
include_directories(inc)
 
# Collect all .c files from src recursively
file(GLOB_RECURSE SOURCES "src/**/*.c")
 
# Create executable
add_executable(${PROJECT_NAME} main.c ${SOURCES})