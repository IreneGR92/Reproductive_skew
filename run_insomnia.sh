#!/bin/bash
#
srun -A e3b --pty -t 0-02:00 -X /bin/bash

rm -rf build
mkdir build
cd build
cmake ..
make
cd ..
exit

sbatch insomnia.sh