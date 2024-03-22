#!/bin/bash

#SBATCH --account=e3b

# Mail on NONE, BEGIN, END, FAIL, REQUEUE, ALL
#SBATCH --mail-user=ig2500@columbia.edu
#SBATCH --mail-type=end,fail

# Job name
#SBATCH --job-name="ReprSkew"

# Runtime and memory
#SBATCH --time=120:00:00
#SBATCH --mem-per-cpu=2G
#SBATCH --cpus-per-task=1


##SBATCH --output=/path/to/outfile
##SBATCH --error=/path/to/errfile

# For array jobs
# Indicate how many input files you have
#SBATCH --array=0-9

#### Your shell commands below this line ####

declare -a arr=(

"m1_rate0_NoRelatedness.yml"
"m1_rate0_h0_NoRelatedness.yml"
"m1_rate03_NoRelatedness.yml"
"m1_rate03_h0_NoRelatedness.yml"
"m04_rate0_NoRelatedness.yml"
"m04_rate0_h0_NoRelatedness.yml"
"m025_rate03_NoRelatedness.yml"
"m025_rate03_h0_NoRelatedness.yml"
"m025_rate05_NoRelatedness.yml"
"m025_rate05_h0_NoRelatedness.yml"

		)

srun ./build/App "parameters/${arr[$SLURM_ARRAY_TASK_ID]}"
