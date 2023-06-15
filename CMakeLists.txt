cmake_minimum_required( VERSION 3.0 )

project( Bigint VERSION 1.2.0 )

add_library(${PROJECT_NAME} SHARED
    sources/Bigint.cpp include/Bigint.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/Bigint.h)

target_include_directories(${PROJECT_NAME} PRIVATE .)
