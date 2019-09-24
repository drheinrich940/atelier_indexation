cmake_minimum_required(VERSION 3.14)
project(atelier_indexation C)

set(CMAKE_C_STANDARD 99)

add_executable(atelier_indexation main.c masks.h functions.c functions.h nrc/nralloc.c nrc/nrarith.c nrc/nrio.c convolutions/convolutions.c convolutions/convolutions.h)