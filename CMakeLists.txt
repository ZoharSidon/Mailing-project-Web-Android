cmake_minimum_required(VERSION 3.10)
project(BloomFilterProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Enable testing
enable_testing()

# Add GoogleTest
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Prevent GoogleTest from overriding compiler/linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Find all source files
file(GLOB BLOOM_FILTER_SOURCES "BloomFilter/*.cpp")
file(GLOB COMMAND_SOURCES "Command/*.cpp")
file(GLOB HASH_SOURCES "Hash/*.cpp")
file(GLOB INPUT_SOURCES "Input/*.cpp")
file(GLOB USER_INTERACTION_SOURCES "UserInteraction/*.cpp")
file(GLOB SOCKETS_SOURCES "Sockets/*.cpp")
file(GLOB TESTS_SOURCES "Tests/*.cpp")


# Add test executable
add_executable(run_tests
        ${TESTS_SOURCES}
        ${BLOOM_FILTER_SOURCES}
        ${COMMAND_SOURCES}
        ${HASH_SOURCES}
        ${INPUT_SOURCES}
        ${USER_INTERACTION_SOURCES}
        ${SOCKETS_SOURCES}
)
# Link with GoogleTest
target_link_libraries(run_tests gtest_main)

# Add main application executable
add_executable(run_bloomfilter
        main.cpp
        ${BLOOM_FILTER_SOURCES}
        ${COMMAND_SOURCES}
        ${HASH_SOURCES}
        ${INPUT_SOURCES}
        ${USER_INTERACTION_SOURCES}
        ${SOCKETS_SOURCES}
)

include(GoogleTest)
gtest_discover_tests(run_tests)