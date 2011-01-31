cmake_minimum_required(VERSION 2.6)
project(Drivers)

file(GLOB sources "*.cpp")

add_library(ieeedrivers ${sources})

