cmake_minimum_required(VERSION 2.6)
project(Controls)

file(GLOB sources "*.cpp")

add_library(ieeecontrols ${sources})

