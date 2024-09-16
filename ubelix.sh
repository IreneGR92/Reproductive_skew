#!/bin/bash

# You must specify a valid email address!
#SBATCH --mail-user=ig2500@columbia.edu

# Mail on NONE, BEGIN, END, FAIL, REQUEUE, ALL
#SBATCH --mail-type=end,fail

# Job name
#SBATCH --job-name="ReprSkew"

# Runtime and memory
#SBATCH --time=96:00:00
#SBATCH --mem-per-cpu=2G
#SBATCH --cpus-per-task=1

# Partition
#SBATCH --partition=epyc2

# For parallel jobs

##SBATCH --nodes=5
##SBATCH --ntasks=5
##SBATCH --ntasks-per-node=1


##SBATCH --output=/path/to/outfile
##SBATCH --error=/path/to/errfile

# For array jobs
# Indicate how many input files you have
#SBATCH --array=0-41

#### Your shell commands below this line ####

declare -a arr=(

"RS0_m1_rate0_Xe05_Y1_default.yml"
"RS0_m1_rate0_Xe05_Y1_NoRelatedness.yml"
"RS0_m1_rate03_Xe05_Y1_default.yml"
"RS0_m1_rate03_Xe05_Y1_NoRelatedness.yml"
"RS0_m04_rate0_Xe05_Y1_default.yml"
"RS0_m04_rate0_Xe05_Y1_NoRelatedness.yml"
"RS0_m04_rate03_Xe05_Y1_default.yml"
"RS0_m04_rate03_Xe05_Y1_NoRelatedness.yml"
"RS0_m025_rate0_Xe05_Y1_default.yml"
"RS0_m025_rate0_Xe05_Y1_NoRelatedness.yml"
"RS0_m025_rate03_Xe05_Y1_default.yml"
"RS0_m025_rate03_Xe05_Y1_NoRelatedness.yml"
"RS0_m025_rate05_Xe05_Y1_default.yml"
"RS0_m025_rate05_Xe05_Y1_NoRelatedness.yml"
"RS1_m1_rate0_Xe05_Y1_default.yml"
"RS1_m1_rate0_Xe05_Y1_NoRelatedness.yml"
"RS1_m1_rate03_Xe05_Y1_default.yml"
"RS1_m1_rate03_Xe05_Y1_NoRelatedness.yml"
"RS1_m04_rate0_Xe05_Y1_default.yml"
"RS1_m04_rate0_Xe05_Y1_NoRelatedness.yml"
"RS1_m04_rate03_Xe05_Y1_default.yml"
"RS1_m04_rate03_Xe05_Y1_NoRelatedness.yml"
"RS1_m025_rate0_Xe05_Y1_default.yml"
"RS1_m025_rate0_Xe05_Y1_NoRelatedness.yml"
"RS1_m025_rate03_Xe05_Y1_default.yml"
"RS1_m025_rate03_Xe05_Y1_NoRelatedness.yml"
"RS1_m025_rate05_Xe05_Y1_default.yml"
"RS1_m025_rate05_Xe05_Y1_NoRelatedness.yml"
"RS05_m1_rate0_Xe05_Y1_default.yml"
"RS05_m1_rate0_Xe05_Y1_NoRelatedness.yml"
"RS05_m1_rate03_Xe05_Y1_default.yml"
"RS05_m1_rate03_Xe05_Y1_NoRelatedness.yml"
"RS05_m04_rate0_Xe05_Y1_default.yml"
"RS05_m04_rate0_Xe05_Y1_NoRelatedness.yml"
"RS05_m04_rate03_Xe05_Y1_default.yml"
"RS05_m04_rate03_Xe05_Y1_NoRelatedness.yml"
"RS05_m025_rate0_Xe05_Y1_default.yml"
"RS05_m025_rate0_Xe05_Y1_NoRelatedness.yml"
"RS05_m025_rate03_Xe05_Y1_default.yml"
"RS05_m025_rate03_Xe05_Y1_NoRelatedness.yml"
"RS05_m025_rate05_Xe05_Y1_default.yml"
"RS05_m025_rate05_Xe05_Y1_NoRelatedness.yml"

		)

srun ./build/App "parameters/${arr[$SLURM_ARRAY_TASK_ID]}"
