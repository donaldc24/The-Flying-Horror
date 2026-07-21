cmake_minimum_required(VERSION 3.20)

project(
    flying_horror
    VERSION 0.1.0
    DESCRIPTION "C++ flight-control and quadcopter simulation reconstruction"
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/sim.cpp
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
)

target_compile_features(flying_horror
    PRIVATE
        cxx_std_20
)

set_target_properties(flying_horror
    PROPERTIES
        CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(flying_horror
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(flying_horror
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()