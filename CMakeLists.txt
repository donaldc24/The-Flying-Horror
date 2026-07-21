cmake_minimum_required(VERSION 3.20)

project(
    FlyingHorror
    VERSION 0.1.0
    DESCRIPTION "C++ quadcopter flight-control reconstruction"
    LANGUAGES CXX
)

add_library(flying_horror_core
    src/sim.cpp
)

target_include_directories(flying_horror_core
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_compile_features(flying_horror_core
    PUBLIC
        cxx_std_20
)

set_target_properties(flying_horror_core
    PROPERTIES
        CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(flying_horror_core
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(flying_horror_core
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

add_executable(flying_horror
    src/main.cpp
)

target_link_libraries(flying_horror
    PRIVATE
        flying_horror_core
)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()