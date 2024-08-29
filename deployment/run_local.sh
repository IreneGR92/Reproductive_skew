#!/bin/bash


declare -a arr=(

"Immigration05_m04_rate0_default.yml"
#"Immigration05_m1_rate03_default.yml"
#"Immigration05_m1_rate0_default.yml"
#"Immigration05_m1_rate0_NoRelatedness.yml"
"Immigration0_m04_rate03_default.yml"
#"Immigration0_m1_rate03_NoRelatedness.yml"
#"Immigration0_m1_rate0_default.yml"
#"Immigration0_m1_rate0_NoRelatedness.yml"
"Immigration1_m04_rate03_default.yml"
"Immigration1_m1_rate03_default.yml" # problem
#"Immigration1_m1_rate0_default.yml"
#"Immigration1_m1_rate0_NoRelatedness.yml"
#"RS05_m025_rate03_default.yml"
#"RS05_m025_rate03_NoRelatedness.yml"
#"RS05_m025_rate05_default.yml"
#"RS05_m025_rate05_NoRelatedness.yml"
#"RS05_m04_rate03_default.yml"
#"RS05_m04_rate03_NoRelatedness.yml"
#"RS05_m04_rate0_default.yml"
#"RS05_m1_rate03_default.yml"
#"RS05_m1_rate03_NoRelatedness.yml"
#"RS05_m1_rate0_default.yml"
#"RS05_m1_rate0_NoRelatedness.yml"
#"RS0_m025_rate03_default.yml"
#"RS0_m025_rate03_NoRelatedness.yml"
#"RS0_m025_rate05_default.yml"
#"RS0_m025_rate05_NoRelatedness.yml"
#"RS0_m04_rate03_default.yml"
#"RS0_m04_rate03_NoRelatedness.yml"
#"RS0_m04_rate0_default.yml"
#"RS0_m04_rate0_NoRelatedness.yml"
#"RS0_m1_rate03_default.yml"
#"RS0_m1_rate03_NoRelatedness.yml"
#"RS0_m1_rate0_default.yml"
#"RS0_m1_rate0_NoRelatedness.yml"



)

for i in "${arr[@]}"; do
  echo $i
  screen -d -S ${i} -m ./build.sh ${i}

done

screen -list
