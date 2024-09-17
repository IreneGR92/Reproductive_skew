#!/bin/bash


declare -a arr=(

"RS0_m1_rate0_Xe02_Y1_default.yml"
"RS0_m1_rate0_Xe02_Y1_NoRelatedness.yml"
"RS0_m1_rate03_Xe02_Y1_default.yml"
"RS0_m1_rate03_Xe02_Y1_NoRelatedness.yml"
"RS0_m04_rate0_Xe02_Y1_default.yml"
"RS0_m04_rate0_Xe02_Y1_NoRelatedness.yml"
"RS0_m04_rate03_Xe02_Y1_default.yml"
"RS0_m04_rate03_Xe02_Y1_NoRelatedness.yml"
"RS0_m025_rate0_Xe02_Y1_default.yml"
"RS0_m025_rate0_Xe02_Y1_NoRelatedness.yml"
"RS0_m025_rate03_Xe02_Y1_default.yml"
"RS0_m025_rate03_Xe02_Y1_NoRelatedness.yml"
"RS1_m1_rate0_Xe02_Y1_default.yml"
"RS1_m1_rate0_Xe02_Y1_NoRelatedness.yml"
"RS1_m1_rate03_Xe02_Y1_default.yml"
"RS1_m1_rate03_Xe02_Y1_NoRelatedness.yml"
"RS1_m04_rate0_Xe02_Y1_default.yml"
"RS1_m04_rate0_Xe02_Y1_NoRelatedness.yml"
"RS1_m04_rate03_Xe02_Y1_default.yml"
"RS1_m04_rate03_Xe02_Y1_NoRelatedness.yml"
"RS1_m025_rate0_Xe02_Y1_default.yml"
"RS1_m025_rate0_Xe02_Y1_NoRelatedness.yml"
"RS1_m025_rate03_Xe02_Y1_default.yml"
"RS1_m025_rate03_Xe02_Y1_NoRelatedness.yml"
"RS05_m1_rate0_Xe02_Y1_default.yml"
"RS05_m1_rate0_Xe02_Y1_NoRelatedness.yml"
"RS05_m1_rate03_Xe02_Y1_default.yml"
"RS05_m1_rate03_Xe02_Y1_NoRelatedness.yml"
"RS05_m04_rate0_Xe02_Y1_default.yml"
"RS05_m04_rate0_Xe02_Y1_NoRelatedness.yml"
"RS05_m04_rate03_Xe02_Y1_default.yml"
"RS05_m04_rate03_Xe02_Y1_NoRelatedness.yml"
"RS05_m025_rate0_Xe02_Y1_default.yml"
"RS05_m025_rate0_Xe02_Y1_NoRelatedness.yml"
"RS05_m025_rate03_Xe02_Y1_default.yml"
"RS05_m025_rate03_Xe02_Y1_NoRelatedness.yml"


)

for i in "${arr[@]}"; do
  echo $i
  screen -d -S ${i} -m ./build.sh ${i}

done

screen -list
