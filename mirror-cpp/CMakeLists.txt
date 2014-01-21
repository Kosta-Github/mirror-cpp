cmake_minimum_required(VERSION 2.8)

project(mirror-cpp CXX)
message("configure: mirror-cpp library")

add_library(
	mirror-cpp SHARED
	mirror-cpp.hpp
	mirror.cpp
	mirror.hpp
)
