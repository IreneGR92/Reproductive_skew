#!/bin/bash


declare -a arr=(

"NoBH_m1_rate0_default.yml"
"NoBH_m1_rate0_NoRelatedness.yml"
"NoBH_m1_rate0_Xrs0_default.yml"
"NoBH_m1_rate0_Xrs0_NoRelatedness.yml"
"NoBH_m1_rate03_default.yml"
"NoBH_m1_rate03_NoRelatedness.yml"
"NoBH_m1_rate03_Xrs0_default.yml"
"NoBH_m1_rate03_Xrs0_NoRelatedness.yml"
"NoBH_m04_rate0_default.yml"
"NoBH_m04_rate0_NoRelatedness.yml"
"NoBH_m04_rate0_Xrs0_default.yml"
"NoBH_m04_rate0_Xrs0_NoRelatedness.yml"
"NoBH_m04_rate03_default.yml"
"NoBH_m04_rate03_NoRelatedness.yml"
"NoBH_m04_rate03_Xrs0_default.yml"
"NoBH_m04_rate03_Xrs0_NoRelatedness.yml"
"NoBH_m025_rate0_default.yml"
"NoBH_m025_rate0_NoRelatedness.yml"
"NoBH_m025_rate0_Xrs0_default.yml"
"NoBH_m025_rate0_Xrs0_NoRelatedness.yml"
"NoBH_m025_rate03_default.yml"
"NoBH_m025_rate03_NoRelatedness.yml"
"NoBH_m025_rate03_Xrs0_default.yml"
"NoBH_m025_rate03_Xrs0_NoRelatedness.yml"
"NoBH_m025_rate05_default.yml"
"NoBH_m025_rate05_NoRelatedness.yml"
"NoBH_m025_rate05_Xrs0_default.yml"
"NoBH_m025_rate05_Xrs0_NoRelatedness.yml"

)

for i in "${arr[@]}"; do
  echo $i
  screen -d -S ${i} -m ./build.sh ${i}

done

screen -list
