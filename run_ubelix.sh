#!/bin/bash
#
module load CMake/3.20.1-GCCcore-10.3.0
module load GCC/11.3.0
rm -rf build
mkdir build

cd build
cmake ..
make App
cd ..

sbatch ubelix.sh