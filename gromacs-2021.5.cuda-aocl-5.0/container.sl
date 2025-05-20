#!/bin/bash -e 

#SBATCH --account nesi99999
#SBATCH --job-name=gromacs_bench
#SBATCH --nodes=5                  # Start with 20 nodes, adjust as needed up to 30
#SBATCH --ntasks-per-node=42       # Set to number of cores per node on your system
#SBATCH --time=02:00:00   
#SBATCH --output=slog/%j.out

module purge >/dev/null 2>&1
# Load GROMACS module if needed
#module load GROMACS/2021.5-gimkl-2022a-cuda-11.6.2-hybrid

export CMD="apptainer exec --env OMP_NUM_THREADS=4 ../openmpi_fix.aimg"
# Run the benchmark
srun ${CMD} gmx_mpi mdrun -v -s benchMEM.tpr -ntomp 4 -pin auto -resethway -nsteps 1000000
