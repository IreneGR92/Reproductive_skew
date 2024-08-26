#!/bin/bash


declare -a arr=(


"Immigration0_m1_rate0_default.yml"
"Immigration0_m1_rate0_NoRelatedness.yml"
"Immigration0_m1_rate03_default.yml"
"Immigration0_m1_rate03_NoRelatedness.yml"
"Immigration1_m1_rate0_default.yml"
"Immigration1_m1_rate0_NoRelatedness.yml"
"Immigration1_m1_rate03_default.yml"
"Immigration1_m1_rate03_NoRelatedness.yml"
"Immigration05_m1_rate0_default.yml"
"Immigration05_m1_rate0_NoRelatedness.yml"
"Immigration05_m1_rate03_default.yml"
"Immigration05_m1_rate03_NoRelatedness.yml"


)

for i in "${arr[@]}"; do
  echo $i
  screen -d -S ${i} -m ./build.sh ${i}

done

screen -list
