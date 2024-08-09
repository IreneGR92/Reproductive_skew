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
#SBATCH --array=0-23

#### Your shell commands below this line ####

declare -a arr=(

"Immigration0_m04_rate03_default.yml"
"Immigration0_m04_rate03_NoRelatedness.yml"
"Immigration0_m025_rate0_default.yml"
"Immigration0_m025_rate0_NoRelatedness.yml"
"Immigration1_m04_rate03_default.yml"
"Immigration1_m04_rate03_NoRelatedness.yml"
"Immigration1_m025_rate0_default.yml"
"Immigration1_m025_rate0_NoRelatedness.yml"
"Immigration05_m04_rate03_default.yml"
"Immigration05_m04_rate03_NoRelatedness.yml"
"Immigration05_m025_rate0_default.yml"
"Immigration05_m025_rate0_NoRelatedness.yml"

"RS0_m04_rate03_default.yml"
"RS0_m025_rate0_NoRelatedness.yml"
"RS0_m025_rate0_default.yml"
"RS0_m04_rate03_NoRelatedness.yml"
"RS1_m04_rate03_default.yml"
"RS1_m04_rate03_NoRelatedness.yml"
"RS1_m025_rate0_NoRelatedness.yml"
"RS1_m025_rate0_default.yml"
"RS05_m04_rate03_default.yml"
"RS05_m04_rate03_NoRelatedness.yml"
"RS05_m025_rate0_default.yml"
"RS05_m025_rate0_NoRelatedness.yml"


		)

srun ./build/App "parameters/${arr[$SLURM_ARRAY_TASK_ID]}"
