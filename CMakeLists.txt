cmake_minimum_required(VERSION 3.16)

project(MyProject
    VERSION 1.0
    LANGUAGES CXX
)

# ============================================================
# C++ standard
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Для compile_commands.json
# Полезно для clangd, IDE, статического анализа
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ============================================================
# Options
# ============================================================

option(ML_ENABLE_WARNINGS "Enable strict compiler warnings" ON)
option(ML_ENABLE_SANITIZERS "Enable sanitizers in Debug builds" ON)
option(ML_ENABLE_TESTS "Build tests" ON)


# ============================================================
# Main ML library
#
# Пока твои Vector/Matrix/Tensor являются header-only.
# ============================================================

add_library(ml INTERFACE)

target_include_directories(ml
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(ml
    INTERFACE
        cxx_std_17
)


# ============================================================
# Compiler warnings
# ============================================================

if(ML_ENABLE_WARNINGS)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

        target_compile_options(ml
            INTERFACE
                -Wall
                -Wextra
                -Wpedantic

                -Wconversion
                -Wsign-conversion

                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Wcast-align
                -Wunused
                -Woverloaded-virtual
                -Wnull-dereference
                -Wdouble-promotion
                -Wformat=2
        )

    elseif(MSVC)

        target_compile_options(ml
            INTERFACE
                /W4
                /permissive-
        )

    endif()

endif()


# ============================================================
# Sanitizers
#
# Только Debug.
# AddressSanitizer + UndefinedBehaviorSanitizer.
# Очень полезно для собственного Vector/Tensor.
# ============================================================

if(
    ML_ENABLE_SANITIZERS
    AND CMAKE_BUILD_TYPE STREQUAL "Debug"
    AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"
)

    target_compile_options(ml
        INTERFACE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
    )

    target_link_options(ml
        INTERFACE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
    )

endif()


# ============================================================
# Main executable
# ============================================================

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ml
)


# ============================================================
# Tests
# ============================================================

if(ML_ENABLE_TESTS)

    include(CTest)

    if(BUILD_TESTING)

        include(FetchContent)

        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

        FetchContent_MakeAvailable(googletest)

        file(GLOB_RECURSE TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        )

        if(TEST_SOURCES)

            add_executable(ml_tests
                ${TEST_SOURCES}
            )

            target_link_libraries(ml_tests
                PRIVATE
                    ml
                    GTest::gtest_main
            )

            include(GoogleTest)

            gtest_discover_tests(ml_tests)

        else()

            message(STATUS "No test files found in test/")

        endif()

    endif()

endif()


# ============================================================
# clang-format
# ============================================================

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)

    file(GLOB_RECURSE ALL_SOURCE_FILES
        CONFIGURE_DEPENDS

        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.tpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.h"
    )

    if(ALL_SOURCE_FILES)

        add_custom_target(format
            COMMAND
                ${CLANG_FORMAT}
                -style=file
                -i
                ${ALL_SOURCE_FILES}

            COMMENT "Formatting source files"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        add_custom_target(format-check
            COMMAND
                ${CLANG_FORMAT}
                -style=file
                --dry-run
                --Werror
                ${ALL_SOURCE_FILES}

            COMMENT "Checking source formatting"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

    endif()

else()

    message(
        STATUS
        "clang-format not found: format targets disabled"
    )

endif()


option(ML_ENABLE_COVERAGE "Enable code coverage" OFF)

if(ML_ENABLE_COVERAGE
    AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

    target_compile_options(ml INTERFACE
        --coverage
        -O0
        -g
    )

    target_link_options(ml INTERFACE
        --coverage
    )

endif()

# ============================================================
# Run tests
# ============================================================

if(TARGET ml_tests)

    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND}
            --output-on-failure
        DEPENDS ml_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building and running all tests"
    )

endif()

# ============================================================
# Coverage
# ============================================================

if(ML_ENABLE_COVERAGE)

    find_program(GCOVR gcovr)

    if(GCOVR AND TARGET ml_tests)

        set(COVERAGE_DIR
            ${CMAKE_BINARY_DIR}/coverage
        )

        add_custom_target(coverage

            # Создаём папку
            COMMAND ${CMAKE_COMMAND}
                -E make_directory
                ${COVERAGE_DIR}

            # Сначала запускаем тесты
            COMMAND ${CMAKE_CTEST_COMMAND}
                --output-on-failure

            # Генерируем HTML
            COMMAND ${GCOVR}
                --root ${CMAKE_SOURCE_DIR}
                --filter ${CMAKE_SOURCE_DIR}/include
                --filter ${CMAKE_SOURCE_DIR}/src
                --exclude ${CMAKE_SOURCE_DIR}/test
                --html-details
                --output ${COVERAGE_DIR}/index.html

            DEPENDS ml_tests

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

            COMMENT "Running tests and generating coverage report"
        )

    endif()

endif()

if(TARGET ml_tests)

    add_custom_target(all-checks

        COMMAND ${CMAKE_CTEST_COMMAND}
            --output-on-failure

        DEPENDS
            ml_tests

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

        COMMENT "Running project checks"
    )

endif()