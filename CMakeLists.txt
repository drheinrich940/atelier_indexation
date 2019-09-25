cmake_minimum_required(VERSION 3.13.2)
project(atelier_indexation C)

set(CMAKE_C_STANDARD 99)

add_executable(atelier_indexation main.c  functions.c  nrc/nralloc.c nrc/nrarith.c nrc/nrio.c)