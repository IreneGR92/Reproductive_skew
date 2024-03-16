#!/bin/bash
#
module load CMake/3.20.1-GCCcore-10.3.0

rm -rf build
mkdir build

cd build
cmake ..
make
cd ..

sbatch ubelix.sh