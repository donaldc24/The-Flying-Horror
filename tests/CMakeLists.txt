include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.2
)

FetchContent_MakeAvailable(Catch2)

add_executable(flying_horror_tests
    units_tests.cpp
    motor_output_tests.cpp
    domain_types_tests.cpp
    pid_controller_tests.cpp
    roll_dynamics_tests.cpp
)

target_link_libraries(flying_horror_tests
    PRIVATE
        flying_horror_core
        Catch2::Catch2WithMain
)

target_compile_features(flying_horror_tests
    PRIVATE
        cxx_std_20
)

if(MSVC)
    target_compile_options(flying_horror_tests
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(flying_horror_tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

list(APPEND CMAKE_MODULE_PATH
    "${catch2_SOURCE_DIR}/extras"
)

include(Catch)

catch_discover_tests(flying_horror_tests)