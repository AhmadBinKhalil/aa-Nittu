#!/bin/bash
#SBATCH --job-name=seqwave
#SBATCH --output=seqwave_%j.out
#SBATCH --error=seqwave_%j.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --partition=compute

# Optional: Load any needed modules or set up environment
# module load gcc

# Define matrix sizes and number of iterations per size
MATRIX_SIZES=(512 1024 2048 4096)
ITERATIONS=5

# Provide a header for clarity
echo "Starting Sequential Wavefront Tests..."

for SIZE in "${MATRIX_SIZES[@]}"; do
  for (( i=1; i<=ITERATIONS; i++ )); do
    echo "----"
    echo "Matrix Size = $SIZE, Iteration = $i"
    # You can measure time with the 'time' command or inside the code itself.
    # Here we use the built-in time (Bash keyword) for simplicity.
    time ./sequential_wavefront $SIZE
  done
done

echo "Sequential Wavefront Tests Completed."
