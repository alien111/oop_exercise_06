cmake_minimum_required(VERSION 3.2)

project(run)

add_executable(run
	Source.cpp
	)

set_property(TARGET run PROPERTY CXX_STANDARD 17)
