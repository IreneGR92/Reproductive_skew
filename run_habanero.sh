#!/bin/bash
#
module load cmake/3.17.4

rm -rf build
mkdir build

cd build
cmake ..
make
cd ..

sbatch habanero.sh