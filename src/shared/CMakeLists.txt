cmake_minimum_required(VERSION 2.6)
project(IEEEPathShared)

file(GLOB sources "*.cpp")

add_library(ieeepathshared ${sources})

