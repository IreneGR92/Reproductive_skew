# Minimum CMake version and project details
cmake_minimum_required(VERSION 3.5)
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
if(NOT yaml-cpp_POPULATED)
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()


# Define the source files for the App executable
set(SOURCE_FILES src/main.cpp src/Parameters.h src/Parameters.cpp src/Simulation.cpp src/Simulation.h src/model/Attribute.h src/model/Individual.h src/model/Individual.cpp src/model/Group.h src/model/Group.cpp src/model/Population.cpp src/model/Population.h src/stats/Statistics.h src/stats/Statistics.cpp src/stats/StatisticalFormulas.h src/stats/StatisticalFormulas.cpp src/model/container/IndividualVector.h src/model/container/IndividualVector.cpp)

# Define the App executable and link the yaml-cpp library
add_executable(App ${SOURCE_FILES})
target_link_libraries(App PUBLIC yaml-cpp)

# Define the source files for the Test executable
set(TEST_FILES src/test.cpp src/Parameters.h src/Parameters.cpp src/Simulation.cpp src/Simulation.h src/model/container/Container.h src/model/Attribute.h src/model/Individual.h src/model/Individual.cpp src/model/Group.h src/model/Group.cpp src/model/Population.cpp src/model/Population.h src/stats/Statistics.h src/stats/Statistics.cpp src/stats/StatisticalFormulas.h src/stats/StatisticalFormulas.cpp src/model/container/IndividualVector.h src/model/container/IndividualVector.cpp)

# Define the Test executable and link the yaml-cpp library
add_executable(Test ${TEST_FILES})
target_link_libraries(Test PUBLIC yaml-cpp)