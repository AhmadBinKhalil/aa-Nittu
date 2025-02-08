#!/bin/bash
#SBATCH --job-name=mpiwave
#SBATCH --output=mpiwave_%j.out
#SBATCH --error=mpiwave_%j.err
#SBATCH --time=00:20:00
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4
#SBATCH --partition=compute

# Optional: Load MPI module if your system needs it
# module load mpi

# We test multiple matrix sizes
MATRIX_SIZES=(512 1024 2048 4096)
ITERATIONS=5

echo "Starting MPI Wavefront Tests..."

for SIZE in "${MATRIX_SIZES[@]}"; do
  for (( i=1; i<=ITERATIONS; i++ )); do
    echo "----"
    echo "Matrix Size = $SIZE, Iteration = $i"
    # srun automatically launches across the allocated nodes & tasks
    time srun ./mpi_wavefront $SIZE
  done
done

echo "MPI Wavefront Tests Completed."
