cmake_minimum_required(VERSION 2.6)
project(Shared)

file(GLOB sources "*.cpp")

add_library(ieeeshared ${sources})

