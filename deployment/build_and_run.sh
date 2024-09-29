#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# Define variables
BUILD_DIR="build"
EXECUTABLE="./App"

# Navigate to the project root directory
cd ..

# Remove the build directory if it exists
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

# Create a new build directory
mkdir "$BUILD_DIR"

# Navigate to the build directory
cd "$BUILD_DIR"

# Run CMake to configure the project
cmake ..

# Build the project
make App

# Run the executable
$EXECUTABLE

# Print a completion message
echo "done"