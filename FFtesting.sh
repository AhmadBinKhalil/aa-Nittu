#!/bin/bash
#SBATCH --job-name=ffwave
#SBATCH --output=ffwave_%j.out
#SBATCH --error=ffwave_%j.err
#SBATCH --time=00:20:00
#SBATCH --nodes=1
#SBATCH --cpus-per-task=32
#SBATCH --partition=compute

# Optional: Load any needed modules for FastFlow or compilers
# module load gcc

# We test multiple matrix sizes and multiple thread counts
MATRIX_SIZES=(512 1024 2048 4096)
THREADS=(1 2 4 8 16 32)
ITERATIONS=5

echo "Starting FastFlow Wavefront Tests..."

for SIZE in "${MATRIX_SIZES[@]}"; do
  for T in "${THREADS[@]}"; do
    # If your FastFlow code accepts a "threads" parameter,
    # you could pass it as an additional argument after SIZE.
    # e.g.: ./fastflow_wavefront $SIZE $T
    # If not, you might set an environment variable or adapt your code.
    
    for (( i=1; i<=ITERATIONS; i++ )); do
      echo "----"
      echo "Matrix Size = $SIZE, Threads = $T, Iteration = $i"
      
      # Option A: If your code reads OMP_NUM_THREADS (some FastFlow apps do):
      export OMP_NUM_THREADS=$T
      # Option B: If your code takes the thread count as a second argument:
      # ./fastflow_wavefront $SIZE $T
      
      time ./fastflow_wavefront $SIZE
    done
  done
done

echo "FastFlow Wavefront Tests Completed."
