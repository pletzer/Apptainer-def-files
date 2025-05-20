#!/bin/bash -e

#SBATCH --job-name=apptainer_build
#SBATCH --partition=milan
#SBATCH --time=00:30:00
#SBATCH --mem=24GB
#SBATCH --cpus-per-task=4
#SBATCH --output slog/%j.out

module purge
module load Apptainer/1.2.5

export APPTAINER_CACHEDIR=
export APPTAINER_TMPDIR=

setfacl -b "$APPTAINER_TMPDIR"
unset APPTAINER_BIND

apptainer build --force --fakeroot openmpi-4.1.5.aimg openmpi-4.1.5.def
