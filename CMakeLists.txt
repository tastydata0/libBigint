cmake_minimum_required(VERSION 3.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_subdirectory("src")

# add_library(${PROJECT_NAME} SHARED
# sources/Bigint.cpp include/Bigint.h
# )

# set_target_properties(${PROJECT_NAME} PROPERTIES
# VERSION ${PROJECT_VERSION}
# SOVERSION 1
# PUBLIC_HEADER include/Bigint.h)

# target_include_directories(${PROJECT_NAME} PRIVATE .)
