# Minimum CMake version and project details
cmake_minimum_required(VERSION 3.7)
project(reproductive_skew C CXX)
set(CMAKE_CXX_STANDARD 14)


# Include the FetchContent module
include(FetchContent)

# Declare the yaml-cpp library
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.6.3  # replace with the desired version
)

# Fetch the yaml-cpp library
FetchContent_GetProperties(yaml-cpp)
if (NOT yaml-cpp_POPULATED)
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif ()


# Define the source files for the App executable
set(SOURCE_FILES
        src/main/Parameters.h
        src/main/Parameters.cpp
        src/main/Simulation.cpp
        src/main/Simulation.h
        src/main/model/Attribute.h
        src/main/model/Individual.h
        src/main/model/Individual.cpp
        src/main/model/Group.h
        src/main/model/Group.cpp
        src/main/model/Population.cpp
        src/main/model/Population.h
        src/main/stats/Statistics.h
        src/main/stats/Statistics.cpp
        src/main/stats/StatisticalFormulas.h
        src/main/stats/StatisticalFormulas.cpp
        src/main/model/container/IndividualVector.h
        src/main/model/container/IndividualVector.cpp)

# Define the App executable and link the yaml-cpp library
add_executable(App src/main/main.cpp ${SOURCE_FILES})
target_link_libraries(App PUBLIC yaml-cpp)


# Testing


# Declare Google Test
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
add_library(googletest ALIAS gtest)


# Define the source files for the Test executable
set(TEST_FILES
        src/test/model/unit_tests.cpp
        src/test/model/test_group.cpp
        src/test/model/container/test_container.cpp
        src/test/model/stats/test_statistical_formulas.cpp
)

# Define the Test executable and link the yaml-cpp library
add_executable(UNIT_TESTS ${TEST_FILES} ${SOURCE_FILES})
target_link_libraries(UNIT_TESTS PUBLIC gtest yaml-cpp)
include(GoogleTest)
gtest_discover_tests(UNIT_TESTS)